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

#ifndef CRISP_INTERPRETEVAL_HPP
#define CRISP_INTERPRETEVAL_HPP
#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

// TODO: fix quote here
/**
 * Interpret an eval expression (evaluate an expression with current environment).
 * @tparam Environ
 * @tparam Expr
 */
template <typename Environ, typename Expr>
struct Interpret<Eval<Expr>, Environ> {
  // First evaluate the given expression to get a valid AST
  using ExprInterp = Interpret<Expr, Environ>;
  using AST = typename ExprInterp::type;

  // Then evaluate the AST in current environment
  using ASTInterp = Interpret<AST, Environ>;
  using type = typename ASTInterp::type;
  using env = typename ASTInterp::env;

  static decltype(type::c_value()) Run() {
    ExprInterp::Run();
    ASTInterp::Run();
    return type::c_value();
  }
};
}  // namespace crisp
#endif  //CRISP_INTERPRETEVAL_HPP
