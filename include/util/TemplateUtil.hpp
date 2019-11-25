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

#ifndef CRISP_TEMPLATE_UTIL_HPP
#define CRISP_TEMPLATE_UTIL_HPP

#include "ast/AST.hpp"

namespace util {
using ast::Closure;
using ast::Var;

/**
 * Convert A<Args...> to B<Args...>
 * @tparam A
 * @tparam B
 */
template <typename A, template <typename...> class B>
struct Convert;

template <template <typename...> class A, template <typename...> class B, typename... Args>
struct Convert<A<Args...>, B> {
  using type = B<Args...>;
};

/**
 * Test if given type is a template.
 * e.g. IsTemplate<int>::value will be false.
 *      IsTemplate<T<int>>::value will be true.
 * @tparam T
 */
template <typename T>
struct IsTemplate {
  static const bool value = false;
};

template <template <typename...> class C, typename... Args>
struct IsTemplate<C<Args...>> {
  static const bool value = true;
};

/**
 * Test if the 2 given types are instances of a same template.
 * e.g. IsSameTemplate<A<...>,A<...>>::value will be true.
 *      IsSameTemplate<A<...>,B<...>>::value will be false.
 * @tparam A
 * @tparam B
 */
template <typename A, typename B>
struct IsSameTemplate {
  static const bool value = false;
};

template <template <typename...> class A, typename... Args1, typename... Args2>
struct IsSameTemplate<A<Args1...>, A<Args2...>> {
  static const bool value = true;
};

/**
 * Check if given type `T` is a instance of template `C`.
 * e.g. IsTemplateOf<Array, Array<Int<1>,Int<2>>>::value will be true.
 * @tparam C
 * @tparam T
 */
template <template <typename...> class C, typename T>
struct IsTemplateOf {
  static const bool value = false;
};

template <template <typename...> class C, typename... Args>
struct IsTemplateOf<C, C<Args...>> {
  static const bool value = true;
};

/**
 * Check if T is an instance of T<ValueType...>.
 * e.g. IsValueTemplateOf<int, A, A<1>> will be true.
 *      IsValueTemplateOf<bool, B, b<true>> will be true.
 * @tparam ValueType
 * @tparam C
 * @tparam T
 */
template <typename ValueType, template <ValueType...> class C, typename T>
struct IsValueTemplateOf {
  static const bool value = false;
};

template <typename ValueType, template <ValueType...> class C, ValueType... Args>
struct IsValueTemplateOf<ValueType, C, C<Args...>> {
  static const bool value = true;
};

template <typename T>
struct IsVar : IsValueTemplateOf<char, Var, T> {};

template <typename T>
struct IsNil : std::is_same<T, Nil> {};

/**
 * Check if an expression is callable or not.
 * @tparam Args
 */
template <typename... Args>
struct IsCallable {
  static const bool value = false;
};

template <typename... Args>
struct IsCallable<Closure<Args...>> {
  static const bool value = true;
};

}  // namespace util
#endif  //CRISP_TEMPLATE_UTIL_HPP
