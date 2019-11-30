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

#ifndef CRISP_DEFER_HPP
#define CRISP_DEFER_HPP

#include "ast/AST.hpp"

namespace util {
using ast::Nil;

/**
 * Save the context of a template `C` and it's arguments `Args`, and apply it later.
 *
 * @tparam C
 * @tparam Args
 */
template <template <typename...> class C, typename... Args>
struct DeferApply {
  template <typename...>
  struct apply {
    using type = typename C<Args...>::type;
  };
};

/**
 * Save the context of a template `C` and it's arguments `Args`, and instantiate it later.
 *
 * @tparam C
 * @tparam Args
 */
template <template <typename...> class C, typename... Args>
struct DeferConstruct {
  template <typename...>
  struct apply {
    using type = C<Args...>;
  };
};

/**
 * Check if given type `T` is an instantiation of `DeferApply`.
 *
 * @tparam T
 */
template <typename T>
struct IsDeferApply {
  static const bool value = false;
};

template <template <typename...> class C, typename... Args>
struct IsDeferApply<DeferApply<C, Args...>> {
  static const bool value = true;
};

/**
 * A template receives a type T and return it unchanged.
 * @tparam T
 */
template <typename T>
struct Id {
  using type = T;
};

/**
 *  A template receives a type T and return it unchanged.
 * @tparam ...
 */
template <typename...>
struct NilF {
  using type = Nil;
};

}  // namespace util

#endif  //CRISP_DEFER_HPP
