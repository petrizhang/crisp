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

#ifndef CRISP_INTERPRETVARIABLE_HPP
#define CRISP_INTERPRETVARIABLE_HPP

#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

/**
 * Interpret a variable reference. e.g. Var<'n'>
 * @tparam args
 * @tparam Environ
 */
template <char... args, typename Environ>
struct Interpret<Var<args...>, Environ> {
  using env = Environ;
  using type = typename EnvLookup<Environ, Var<args...>>::type;

  inline static auto Run() {
    // TODO: fix Run method fails when `type` is a primitive type
    // return type::c_value();
    return "#undefined";
  }
};
}  // namespace crisp

#endif  //CRISP_INTERPRETVARIABLE_HPP
