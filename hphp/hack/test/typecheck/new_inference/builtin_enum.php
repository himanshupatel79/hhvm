<?hh // strict
// Copyright 2004-present Facebook. All Rights Reserved.

enum E: string as string {
  AA = 'AA';
}

/* HH_IGNORE_ERROR[2053] */
function foo<T as HH\BuiltinEnum<T>>(classname<T> $class_name): void { }

function testit(): void {
  foo(E::class);
}
