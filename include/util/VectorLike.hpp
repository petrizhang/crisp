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

#ifndef CRISP_VECTOR_LIKE_HPP
#define CRISP_VECTOR_LIKE_HPP

#include "Size.hpp"

namespace util {

/**
 * Get the first element of a template T<e1,...>.
 * e.g. TemplateHead<T<int,bool>>::type will be `bool`.
 *
 * @tparam V
 */
template <typename V>
struct TemplateHead;

template <template <typename...> class V, typename... Args>
struct TemplateHead<V<Args...>> {
  static_assert(Size<Args...>::value != 0, "Cannot apply head method on an empty vector.");
};

template <template <typename...> class V, typename Head>
struct TemplateHead<V<Head>> {
  using type = Head;
};

template <template <typename...> class V, typename Head, typename... Args>
struct TemplateHead<V<Head, Args...>> {
  using type = Head;
};

/**
 * Get the tail elements of a vector-like template T<e1,...>.
 * e.g. TemplateTail<T<int,bool,char>>::type will be `T<bool,char>`.
 *
 * @tparam V
 */
template <typename V>
struct TemplateTail;

template <template <typename...> class V>
struct TemplateTail<V<>> {
  using type = V<>;
};

template <template <typename...> class V, typename Head, typename... Args>
struct TemplateTail<V<Head, Args...>> {
  using type = V<Args...>;
};

/**
 * Push an element to the front of a template.
 * e.g. VectorLikePushFront<T<t1,t2>,t0>::type will be T<t0,t1,t2>.
 * @tparam V a template type, e.g., T<t1,t2,t3...>
 * @tparam E an element type
 */
template <typename V, typename E>
struct TemplatePushFront;

template <template <typename...> class V, typename... Args, typename E>
struct TemplatePushFront<V<Args...>, E> {
  using type = V<E, Args...>;
};

/**
 * Push an element to the end of a template.
 * e.g. VectorLikePushFront<T<t0,t1>,t2>::type will be T<t0,t1,t2>.
 * @tparam V a template type, e.g., T<t1,t2,t3...>
 * @tparam E an element type
 */
template <typename V, typename E>
struct TemplatePushBack;

template <template <typename...> class V, typename... Args, typename E>
struct TemplatePushBack<V<Args...>, E> {
  using type = V<Args..., E>;
};

/**
 * Concat two template types to one.
 * e.g. TemplateConcat<T<t1,t2>,T<t3,t4>>::type will bel T<t1,t2,t3,t4>.
 * @tparam VL
 * @tparam VR
 */
template <typename VL, typename VR>
struct TemplateConcat;

template <template <typename...> class V, typename... LArgs, typename... RArgs>
struct TemplateConcat<V<LArgs...>, V<RArgs...>> {
  using type = V<LArgs..., RArgs...>;
};
}  // namespace util

#endif  //CRISP_VECTOR_LIKE_HPP
