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

#ifndef CRISP_STR2LIST_HPP
#define CRISP_STR2LIST_HPP
#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

template <typename Environ, typename Expr>
struct Interpret<Str2List<Expr>, Environ> {
  using ExprInterp = Interpret<Expr, Environ>;
  using ExprValue = typename ExprInterp::type;

  using env = Environ;
  using type = typename Str2ListImpl<ExprValue>::type;
};

}  // namespace crisp
#endif  // CRISP_STR2LIST_HPP
