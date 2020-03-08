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

#ifndef CRISP_COMMON_HPP
#define CRISP_COMMON_HPP

#include <array>
#include <cassert>
#include <iostream>
#include <type_traits>

#include "crisp/ast/CoreAST.hpp"
#include "crisp/util/Util.hpp"

namespace crisp {
using namespace crisp;
using namespace util;
using std::is_base_of;
using std::is_same;

template <typename Expr, typename Environ = Env<>>
struct Interpret {
  using env = Environ;
  // Return unrecognized types transparently
  using type = Expr;

  inline static auto Run() { return "#omit"; };
};

}  // namespace crisp
#endif  //CRISP_COMMON_HPP
