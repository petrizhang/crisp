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

#include "TemplateUtil.hpp"

namespace util {

template <typename...>
struct ListLike {};

/**
 * Get the first element of a list-like template T<e1,...>.
 * e.g. TemplateHead<T<int,bool>>::type will be `bool`.
 *
 * @tparam V
 */
template <typename V>
struct ListLikeHead;

template <template <typename...> class V, typename... Args>
struct ListLikeHead<V<Args...>> {
  static_assert(sizeof...(Args) != 0, "cannot apply head method on an empty list.");
  using type = typename ListLikeHead<ListLike<Args...>>::type;
};

template <typename Head>
struct ListLikeHead<ListLike<Head>> {
  using type = Head;
};

template <typename Head, typename... Args>
struct ListLikeHead<ListLike<Head, Args...>> {
  using type = Head;
};

/**
 * Get the last element of a list-like template T<e1,...>.
 * e.g. TemplateLast<T<int,bool>>::type will be `bool`.
 *
 * @tparam V
 */
template <typename V>
struct ListLikeLast;

template <template <typename...> class V, typename... Args>
struct ListLikeLast<V<Args...>> {
  static_assert(sizeof...(Args) != 0, "Cannot apply head method on an empty vector.");
  using type = typename ListLikeLast<ListLike<Args...>>::type;
};

template <typename Last>
struct ListLikeLast<ListLike<Last>> {
  using type = Last;
};

template <typename Head, typename... Args>
struct ListLikeLast<ListLike<Head, Args...>> {
  using type = typename ListLikeLast<ListLike<Args...>>::type;
};

/**
 * Get the tail elements of a vector-like template T<e1,...>.
 * e.g. TemplateTail<T<int,bool,char>>::type will be `T<bool,char>`.
 *
 * @tparam V
 */
template <typename V>
struct ListLikeTail;

template <template <typename...> class V>
struct ListLikeTail<V<>> {
  using type = V<>;
};

template <template <typename...> class V, typename Head, typename... Args>
struct ListLikeTail<V<Head, Args...>> {
  using type = V<Args...>;
};

/**
 * Push an element to the front of a list-like template.
 * e.g. ListLikePushLeft<T<t1,t2>,t0>::type will be T<t0,t1,t2>.
 * @tparam V a template type, e.g., T<t1,t2,t3...>
 * @tparam E an element type
 */
template <typename V, typename E>
struct ListLikePushFront;

template <template <typename...> class V, typename... Args, typename E>
struct ListLikePushFront<V<Args...>, E> {
  using type = V<E, Args...>;
};

/**
 * Push an element to the end of a list-like template.
 * e.g. ListLikePushRight<T<t0,t1>,t2>::type will be T<t0,t1,t2>.
 * @tparam V a template type, e.g., T<t1,t2,t3...>
 * @tparam E an element type
 */
template <typename V, typename E>
struct ListLikePushBack;

template <template <typename...> class V, typename... Args, typename E>
struct ListLikePushBack<V<Args...>, E> {
  using type = V<Args..., E>;
};

/**
  * Pop an element from the front of a list-like template.
  * @tparam V
  */
template <typename V>
struct ListLikePopFront {
  static_assert(!IsEmptyImpl<V>::value, "cannot apply pop method on an empty list.");
};

template <template <typename...> class V, typename Head, typename... Args>
struct ListLikePopFront<V<Head, Args...>> {
  using poped = Head;
  using rest = V<Args...>;
};

/**
 * Pop an element from the end of a list-like template.
 * @tparam V
 */
template <typename V>
struct ListLikePopBack {
  static_assert(!IsEmptyImpl<V>::value, "cannot apply pop method on an empty list.");
};

template <template <typename...> class V, typename Head>
struct ListLikePopBack<V<Head>> {
  using poped = Head;
  using rest = V<>;
};

template <template <typename...> class V, typename Head, typename... Args>
struct ListLikePopBack<V<Head, Args...>> {
  using poped = typename ListLikePopBack<V<Args...>>::poped;
  using rest = typename ListLikePushFront<
      typename ListLikePopBack<V<Args...>>::rest,
      Head>::type;
};

/**
 * Concat two template types to one.
 * e.g. TemplateConcat<T<t1,t2>,T<t3,t4>>::type will bel T<t1,t2,t3,t4>.
 * @tparam VL
 * @tparam VR
 */
template <typename VL, typename VR>
struct ListLikeConcat;

template <template <typename...> class V, typename... LArgs, typename... RArgs>
struct ListLikeConcat<V<LArgs...>, V<RArgs...>> {
  using type = V<LArgs..., RArgs...>;
};
}  // namespace util

#endif  //CRISP_VECTOR_LIKE_HPP
