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

#ifndef CRISP_INTERNAL_LIST_HPP
#define CRISP_INTERNAL_LIST_HPP

#include "VectorLike.hpp"

namespace util {
namespace internal {

/**
 * To distinguish with `List` in user code, using this type internally in interpreter.
 *
 * @tparam Args
 */
template <typename... Args>
struct InternalList {};

template <typename T, typename Elem>
struct InternalListPushFront;

template <typename... Elements, typename Elem>
struct InternalListPushFront<InternalList<Elements...>, Elem> : TemplatePushFront<InternalList<Elements...>, Elem> {};

template <typename L, typename R>
struct InternalListExtendBack;

template <typename... LElements, typename... RElements>
struct InternalListExtendBack<InternalList<LElements...>, InternalList<RElements...>>
    : TemplateConcat<InternalList<LElements...>, InternalList<RElements...>> {};

}  // namespace internal
}  // namespace util
#endif  //CRISP_INTERNAL_LIST_HPP
