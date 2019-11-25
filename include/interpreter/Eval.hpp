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

#ifndef CRISP_EVAL_HPP
#define CRISP_EVAL_HPP
#include "Common.h"

namespace crisp {
using namespace ast;
using namespace util;

/// -------------------------------------------------------------------------------------------
/// Interpret eval expression (evaluate an expression with current environment).
// TODO: fix quote here
template <typename Environ, typename Expr>
struct Interp<Eval<Expr>, Environ> {
  using ExprInterp = Interp<Expr, Environ>;
  using InterpEnv = typename ExprInterp::env;
  using InterpResult = typename ExprInterp::type;

  static_assert(IsTemplateOf<Quote, InterpResult>::value,
                "eval could only be applied to a quoted expression");
  using AST = typename GetQuoteAST<InterpResult>::type;
  using ASTInterp = Interp<AST, Environ>;

  using type = typename ASTInterp::type;
  using env = typename ASTInterp::env;

  static decltype(type::c_value()) Run() {
    ExprInterp::Run();
    ASTInterp::Run();
    return type::c_value();
  }
};
}  // namespace crisp
#endif  //CRISP_EVAL_HPP
