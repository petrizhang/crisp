/*
 * Copyright 2019 by Crisp Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CRISP_ENV_HPP
#define CRISP_ENV_HPP

#include "Dict.hpp"
#include "List.hpp"
#include "ast/AST.hpp"

namespace util {
using ast::Pair;
using ast::Undefined;

/**
 * Environment stack implementation.
 * Every element in the stack is a symbol table for a specific lexical scope.
 */
template <typename... Dicts>
using Env = List<Dicts...>;

template <typename Environ, typename Extra>
using EnvExtendBack = ListConcat<Environ, Extra>;

template <typename env, typename dict>
using EnvPushFront = ListPushLeft<env, dict>;

/**
 * Bind a variable name `K` with a value `V` in current scope.
 * @tparam env
 * @tparam K
 * @tparam V
 */
template <typename env, typename K, typename V>
struct EnvPut;

template <typename K, typename V>
struct EnvPut<Env<>, K, V> {
  using type = Env<Dict<Pair<K, V>>>;
};

template <typename K, typename V, typename dict, typename... Tail>
struct EnvPut<Env<dict, Tail...>, K, V> {
  using type = Env<typename DictPut<dict, Pair<K, V>>::type, Tail...>;
};

/**
 * Lookup a variable by name `K` in current environment.
 * @tparam env
 * @tparam K
 */
template <typename env, typename K>
struct EnvLookup {
  using type = Undefined;
};

template <typename K, typename dict>
struct EnvLookup<Env<dict>, K> {
  using V = typename DictGet<dict, K>::type;
  using type = typename std::conditional<std::is_same<V, Nil>::value,
                                         Undefined,
                                         V>::type;
};

template <typename K, typename dict, typename... Tail>
struct EnvLookup<Env<dict, Tail...>, K> {
  using current_scope_value = typename DictGet<dict, K>::type;
  using type = typename std::conditional<std::is_same<current_scope_value, Nil>::value,
                                         typename EnvLookup<Env<Tail...>, K>::type,
                                         current_scope_value>::type;
};
}  // namespace util
#endif  //CRISP_ENV_HPP
