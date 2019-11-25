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

#ifndef CRISP_LIST_HPP
#define CRISP_LIST_HPP

#include "VectorLike.hpp"

namespace util {

/**
 *  A vector-like collection type.
 */
template <typename... Elements>
struct Vector {};

template <typename T>
struct VectorHead;

template <typename... Elements>
struct VectorHead<Vector<Elements...>> : TemplateHead<Vector<Elements...>> {};

template <typename T>
struct VectorTail;

template <typename... Elements>
struct VectorTail<Vector<Elements...>> : TemplateTail<Vector<Elements...>> {};

template <typename T, typename Elem>
struct VectorPushFront;

template <typename... Elements, typename Elem>
struct VectorPushFront<Vector<Elements...>, Elem> : TemplatePushFront<Vector<Elements...>, Elem> {};

template <typename T, typename Elem>
struct VectorPushBack;

template <typename... Elements, typename Elem>
struct VectorPushBack<Vector<Elements...>, Elem> : TemplatePushBack<Vector<Elements...>, Elem> {};

template <typename L, typename R>
struct VectorExtendBack;

template <typename... LElements, typename... RElements>
struct VectorExtendBack<Vector<LElements...>, Vector<RElements...>>
    : TemplateConcat<Vector<LElements...>, Vector<RElements...>> {};

}  // namespace util

#endif  //CRISP_LIST_HPP
