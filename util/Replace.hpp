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

#ifndef CRISP_REPLACE_HPP
#define CRISP_REPLACE_HPP

#include <type_traits>
#include "Dict.hpp"
#include "InternalList.hpp"
#include "TemplateUtil.hpp"

namespace util {

// TODO: test Replace template carefully
/**
 * Recursively replace types in `T` according to `ReplaceDict`.
 * e.g.: Replace< T<int,bool>, Dict<Pair<bool, char>>::type will be T<int,char>
 * @tparam T 
 * @tparam ReplaceDict 
 */
template <typename T, typename ReplaceDict>
struct Replace {
  using target = typename DictGetImpl<ReplaceDict, T>::type;
  using type = typename std::conditional<IsNilImpl<target>::value,
                                         T,
                                         target>::type;
};

template <typename T>
struct Replace<T, Dict<>> {
  using type = T;
};

template <typename ReplaceDict>
struct Replace<internal::InternalList<>, ReplaceDict> {
  using type = internal::InternalList<>;
};

template <typename Head, typename... Tail, typename ReplaceDict>
struct Replace<internal::InternalList<Head, Tail...>, ReplaceDict> {
  using HeadReplace = typename Replace<Head, ReplaceDict>::type;
  using TailInternalList = typename Replace<internal::InternalList<Tail...>, ReplaceDict>::type;
  using type = typename internal::InternalListPushFront<TailInternalList, HeadReplace>::type;
};

template <template <typename...> class T, typename... Args, typename ReplaceDict>
struct Replace<T<Args...>, ReplaceDict> {
  using InternalListResult = typename Replace<internal::InternalList<Args...>, ReplaceDict>::type;
  using type = typename Convert<InternalListResult, T>::type;
};

}  // namespace util

#endif  //CRISP_REPLACE_HPP
