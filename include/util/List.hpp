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

#include "ListLike.hpp"
#include "ast/AST.hpp"

namespace util {
using ast::List;

/**
 *  List operations.
 */

template <typename T>
struct ListHead;

template <typename... Elements>
struct ListHead<List<Elements...>> : TemplateHead<List<Elements...>> {};

template <typename T>
struct ListTail;

template <typename... Elements>
struct ListTail<List<Elements...>> : TemplateTail<List<Elements...>> {};

template <typename T, typename Elem>
struct ListPushFront;

template <typename... Elements, typename Elem>
struct ListPushFront<List<Elements...>, Elem> : TemplatePushFront<List<Elements...>, Elem> {};

template <typename T, typename Elem>
struct ListPushBack;

template <typename... Elements, typename Elem>
struct ListPushBack<List<Elements...>, Elem> : TemplatePushBack<List<Elements...>, Elem> {};

template <typename L, typename R>
struct ListExtendBack;

template <typename... LElements, typename... RElements>
struct ListExtendBack<List<LElements...>, List<RElements...>>
    : TemplateConcat<List<LElements...>, List<RElements...>> {};

}  // namespace util

#endif  //CRISP_LIST_HPP
