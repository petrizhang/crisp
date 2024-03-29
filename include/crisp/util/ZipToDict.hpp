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

#ifndef CRISP_ZIPTODICT_HPP
#define CRISP_ZIPTODICT_HPP

#include "ListLike.hpp"

namespace util {

/**
 * Zip two lists into a dictionary.
 * An intuitive example: zip([1,2,3],[a,b,c]) => [(1,a),(2,b),(3,c)]

 * @tparam Keys
 * @tparam Values
 */
template <typename Keys, typename Values>
struct ZipToDict;

template <template <typename...> class C, typename K, typename V, typename... Keys, typename... Values>
struct ZipToDict<C<K, Keys...>, C<V, Values...>> {
  using TailResult = typename ZipToDict<C<Keys...>, C<Values...>>::type;
  using type = typename ListLikePushFront<TailResult, Pair<K, V>>::type;
};

template <template <typename...> class C>
struct ZipToDict<C<>, C<>> {
  using type = Dict<>;
};

}  // namespace util

#endif  //CRISP_ZIPTODICT_HPP
