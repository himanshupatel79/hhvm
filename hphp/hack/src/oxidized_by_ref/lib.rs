// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

pub use oxidized::tany_sentinel;
pub use oxidized::typing_defs_flags;

mod manual;

pub use manual::ast;
pub use manual::doc_comment;
pub use manual::i_map;
pub use manual::i_set;
pub use manual::ident;
pub use manual::internal_type_set;
pub use manual::local_id;
pub use manual::opaque_digest;
pub use manual::phase_map;
pub use manual::pos;
pub use manual::relative_path;
pub use manual::s_map;
pub use manual::s_set;
pub use manual::shape_map;
pub use manual::typing_continuations;
pub use manual::typing_logic;
pub use manual::typing_reason;
pub use manual::typing_set;

mod gen;

pub use gen::aast;
pub use gen::aast_defs;
pub use gen::ast_defs;
pub use gen::decl_defs;
pub use gen::direct_decl_parser;
pub use gen::error_codes;
pub use gen::errors;
pub use gen::file_info;
pub use gen::namespace_env;
pub use gen::naming_types;
pub use gen::nast;
pub use gen::prim_defs;
pub use gen::scoured_comments;
pub use gen::shallow_decl_defs;
pub use gen::tast;
pub use gen::type_parameter_env;
pub use gen::typing_cont_key;
pub use gen::typing_defs;
pub use gen::typing_defs_core;
pub use gen::typing_env_types;
pub use gen::typing_fake_members;
pub use gen::typing_inference_env;
pub use gen::typing_local_types;
pub use gen::typing_mutability_env;
pub use gen::typing_per_cont_env;
pub use gen::typing_tyvar_occurrences;
