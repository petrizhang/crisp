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

#ifndef CRISP_DICT_HPP
#define CRISP_DICT_HPP

#include "List.hpp"
#include "ast/CoreAST.hpp"

namespace util {
using crisp::Dict;
using crisp::Pair;
using crisp::Undefined;

/**
 * Get the value associated with K in a dict.
 * @tparam D
 * @tparam K
 */
template <typename D, typename K>
struct DictGetImpl {
  using type = Undefined;
};

template <typename K, typename V>
struct DictGetImpl<Dict<Pair<K, V>>, K> {
  using type = V;
};

template <typename K, typename V, typename... Tail>
struct DictGetImpl<Dict<Pair<K, V>, Tail...>, K> {
  using type = V;
};

template <typename K, typename V, typename AnyK, typename... Tail>
struct DictGetImpl<Dict<Pair<AnyK, V>, Tail...>, K> {
  using type = typename DictGetImpl<Dict<Tail...>, K>::type;
};

template <typename Collected, typename Rest, typename K, typename V>
struct DictPutImplHelper;

template <typename... CollectedPairs, typename... RestPairs,
          typename K, typename V, typename AnyV>
struct DictPutImplHelper<Dict<CollectedPairs...>,
                         Dict<Pair<K, AnyV>, RestPairs...>,
                         K,
                         V> {
  using type = Dict<CollectedPairs..., Pair<K, V>, RestPairs...>;
};

template <typename... CollectedPairs, typename... RestPairs,
          typename AnyV, typename AnyK, typename K, typename V>
struct DictPutImplHelper<Dict<CollectedPairs...>,
                         Dict<Pair<AnyK, AnyV>, RestPairs...>,
                         K,
                         V> {
  using type = typename DictPutImplHelper<Dict<CollectedPairs..., Pair<AnyK, AnyV>>,
                                          Dict<RestPairs...>,
                                          K,
                                          V>::type;
};

template <typename... CollectedPairs, typename K, typename V>
struct DictPutImplHelper<Dict<CollectedPairs...>,
                         Dict<>,
                         K,
                         V> {
  using type = Dict<Pair<K, V>, CollectedPairs...>;
};

template <typename D, typename K, typename V>
struct DictPutImpl {
  static_assert(IsTemplateOf<Dict, D>::value, "expected a `Dict` instantiation");
  using type = typename DictPutImplHelper<Dict<>, D, K, V>::type;
};

}  // namespace util

#endif  //CRISP_DICT_HPP
