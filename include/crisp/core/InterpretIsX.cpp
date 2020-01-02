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

#ifndef CRISP_INTERPRETISX_HPP
#define CRISP_INTERPRETISX_HPP
#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

template <typename Environ, typename Expr>
struct Interpret<IsEmpty<Expr>, Environ> {
  using ExprInterp = Interpret<Expr, Environ>;
  using ExprValue = typename ExprInterp::type;

  using env = Environ;
  using type = Bool<IsEmptyImpl<ExprValue>::value>;
};

template <typename Environ, typename Expr>
struct Interpret<IsNil<Expr>, Environ> {
  using ExprInterp = Interpret<Expr, Environ>;
  using ExprValue = typename ExprInterp::type;

  using env = Environ;
  using type = Bool<std::is_same<ExprValue, Nil>::value>;
};
}  // namespace crisp
#endif  //CRISP_INTERPRETISX_HPP
