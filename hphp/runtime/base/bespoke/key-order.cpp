
/*
  +----------------------------------------------------------------------+
  | HipHop for PHP                                                       |
  +----------------------------------------------------------------------+
  | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/array-iterator.h"
#include "hphp/runtime/base/bespoke/key-order.h"
#include "hphp/runtime/base/string-data.h"

#include <sstream>

namespace HPHP { namespace bespoke {

namespace {

const StaticString s_extraKey("...");

struct KeyOrderDataHash {
  size_t operator()(const KeyOrder::KeyOrderData& ko) const {
    size_t seed = 0;
    for (auto s : ko) folly::hash::hash_combine(seed, s->hash());
    return seed;
  }
};

using KeyOrderSet =
  std::unordered_set<KeyOrder::KeyOrderData, KeyOrderDataHash>;

KeyOrderSet s_keyOrderSet;
folly::SharedMutex s_keyOrderLock;
}

KeyOrder KeyOrder::insert(const StringData* k) const {
  if (!k->isStatic() || !m_keys) return KeyOrder::MakeInvalid();
  if (std::find(m_keys->begin(), m_keys->end(), k) != m_keys->end()) {
    return *this;
  }
  if (isTooLong()) return *this;
  KeyOrderData newOrder{*m_keys};
  newOrder.push_back(m_keys->size() == kMaxLen ? s_extraKey.get() : k);
  return Make(newOrder);
}

KeyOrder KeyOrder::remove(const StringData* k) const {
  if (!m_keys || isTooLong()) return *this;
  KeyOrderData newOrder{*m_keys};
  newOrder.erase(std::remove_if(newOrder.begin(), newOrder.end(),
                                [&](LowStringPtr key) { return k->same(key); }),
                 newOrder.end());
  return Make(newOrder);
}

KeyOrder KeyOrder::pop() const {
  if (!m_keys || m_keys->empty() || isTooLong()) return *this;
  KeyOrderData newOrder{*m_keys};
  newOrder.pop_back();
  return Make(newOrder);
}

KeyOrder KeyOrder::Make(const KeyOrderData& ko) {
  auto trimmedKeyOrder = trimKeyOrder(ko);
  {
    folly::SharedMutex::ReadHolder rlock{s_keyOrderLock};
    auto it = s_keyOrderSet.find(trimmedKeyOrder);
    if (it != s_keyOrderSet.end()) return KeyOrder{&*it};
  }

  folly::SharedMutex::WriteHolder wlock{s_keyOrderLock};
  auto const ret = s_keyOrderSet.insert(std::move(trimmedKeyOrder));
  return KeyOrder{&*ret.first};
}

KeyOrder::KeyOrder(const KeyOrderData* keys)
  : m_keys(keys)
{}

KeyOrder::KeyOrderData KeyOrder::trimKeyOrder(const KeyOrderData& ko) {
  KeyOrderData res{ko};
  if (res.size() > kMaxLen) {
    res.resize(kMaxLen);
    res.push_back(s_extraKey.get());
  }
  return res;
}

std::string KeyOrder::toString() const {
  if (!m_keys) return "<invalid>";
  std::stringstream ss;
  ss << '[';
  for (auto i = 0; i < m_keys->size(); ++i) {
    if (i > 0) ss << ',';
    ss << '"' << (*m_keys)[i]->data() << '"';
  }
  ss << ']';
  return ss.str();
}

bool KeyOrder::operator==(const KeyOrder& other) const {
  return this->m_keys == other.m_keys;
}

KeyOrder KeyOrder::ForArray(const ArrayData* ad) {
  KeyOrderData ko;
  auto hasStaticStrKeysOnly = true;
  IterateKVNoInc(ad, [&](auto k, auto /*v*/) -> bool {
    if (tvIsString(k) && val(k).pstr->isStatic()) {
      ko.push_back(val(k).pstr);
      return false;
    } else {
      return true;
    }
  });
  return hasStaticStrKeysOnly ? Make(ko) : KeyOrder();
}

KeyOrder KeyOrder::MakeInvalid() {
  return KeyOrder{};
}

bool KeyOrder::isTooLong() const {
  assertx(m_keys);
  return m_keys->size() > kMaxLen;
}
}}
