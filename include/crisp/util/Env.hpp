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
#include "crisp/ast/CoreAST.hpp"

namespace util {
using crisp::Pair;
using crisp::Undefined;

/**
 * Environment stack implementation.
 * Every element in the stack is a symbol table for a specific lexical scope.
 */
template <typename... Dicts>
struct Env {};

template <typename EnvA, typename EnvB>
using EnvConcat = ListLikeConcat<EnvA, EnvB>;

template <typename EnvX, typename dict>
using EnvPushHead = ListLikePushHead<EnvX, dict>;

/**
 * Bind a variable name `K` with a value `V` in current scope.
 * @tparam EnvX
 * @tparam K
 * @tparam V
 */
template <typename EnvX, typename K, typename V>
struct EnvPut;

template <typename K, typename V>
struct EnvPut<Env<>, K, V> {
  using type = Env<Dict<Pair<K, V>>>;
};

template <typename K, typename V, typename DictX, typename... Tail>
struct EnvPut<Env<DictX, Tail...>, K, V> {
  using type = Env<typename DictPutImpl<DictX, K, V>::type, Tail...>;
};

/**
 * Lookup a variable by name `K` in current environment.
 * @tparam EnvX
 * @tparam K
 */
template <typename EnvX, typename K>
struct EnvLookup {
  using type = Undefined;
};

// TODO - Fix bugs here: don't use Nil to indicate a invoking failure.
template <typename K, typename DictX>
struct EnvLookup<Env<DictX>, K> {
  using type = typename DictGetImpl<DictX, K>::type;
};

// TODO - replace std::conditional with ConditionalApply
template <typename K, typename DictX, typename... Tail>
struct EnvLookup<Env<DictX, Tail...>, K> {
  using CurrentScopeValue = typename DictGetImpl<DictX, K>::type;
  using type = typename std::conditional<std::is_same<CurrentScopeValue, Undefined>::value,
                                         typename EnvLookup<Env<Tail...>, K>::type,
                                         CurrentScopeValue>::type;
};
}  // namespace util
#endif  //CRISP_ENV_HPP
