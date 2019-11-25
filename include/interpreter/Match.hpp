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

#ifndef CRISP_MATCH_HPP
#define CRISP_MATCH_HPP
#include "Common.hpp"

namespace crisp {
using namespace ast;
using namespace util;


// TODO: check and test pattern match carefully.
/**
 * Interpret match expression.
 *
 * @tparam Environ
 * @tparam Expr
 * @tparam CaseBranch
 * @tparam DefaultBranch
 */
template <typename Environ, typename Expr, typename CaseBranch,
          typename DefaultBranch>
struct Interp<Match<Expr, CaseBranch, DefaultBranch>, Environ> {
  static_assert(IsTemplateOf<Quote, Expr>::value,
                "only quote matches are supported now");
  static_assert(IsTemplateOf<Case, CaseBranch>::value,
                "expected a valid case branch");
  static_assert(IsTemplateOf<Default, DefaultBranch>::value,
                "expected a default branch");

  using ExprInterp = Interp<Expr, Environ>;
  using AST = typename ExprInterp::type;

  using CaseCondition = typename GetCaseCondition<CaseBranch>::type;
  using CaseResult = typename GetCaseResult<CaseBranch>::type;
  using DefaultResult = typename GetDefaultResult<DefaultBranch>::type;

  using CaseBranchMatch = QuoteMatchCase<Dict<>, AST, CaseCondition>;
  using _CaseBranchMatched = Bool<CaseBranchMatch::matched>;

  using Result = typename ConditionalImpl<When<_CaseBranchMatched, CaseResult>,
                                          Else<DefaultResult>>::type;

  using CapturedDict = typename ConditionalImpl<
      When<_CaseBranchMatched, typename CaseBranchMatch::dict>,
      Else<Dict<>>>::type;

  using type = typename Replace<Result, CapturedDict>::type;
  using env = typename ExprInterp::env;

  static decltype(type::c_value()) Run() {
    ExprInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Expr, typename Branch1, typename Branch2,
          typename Branch3, typename... Branches>
struct Interp<Match<Expr, Branch1, Branch2, Branch3, Branches...>, Environ> {
  static_assert(IsTemplateOf<Quote, Expr>::value,
                "only quote matches are supported now");
  static_assert(IsTemplateOf<Case, Branch1>::value,
                "expected a valid case branch");
  static_assert(IsTemplateOf<Case, Branch2>::value,
                "expected a valid case branch");

  using AST = typename GetQuoteAST<Expr>::type;

  using CaseCondition = typename GetCaseCondition<Branch1>::type;
  using CaseResult = typename GetCaseResult<Branch1>::type;

  using CaseBranchMatch = QuoteMatchCase<Dict<>, AST, CaseCondition>;
  using _CaseBranchMatched = Bool<CaseBranchMatch::matched>;

  using Result = typename ConditionalImpl<When<_CaseBranchMatched, CaseResult>,
                                          Else<Nil>>::type;

  using CapturedDict = typename ConditionalImpl<
      When<_CaseBranchMatched, typename CaseBranchMatch::dict>,
      Else<Dict<>>>::type;

  using ResultInterp = typename ConditionalApply<
      When<_CaseBranchMatched, DeferConstruct<Replace, Result, CapturedDict>>,
      Else<DeferConstruct<Interp, Match<Expr, Branch2, Branch3, Branches...>,
                          Environ>>>::type;

  using type = typename ResultInterp::type;
  using env = typename ResultInterp::env;

  static decltype(type::c_value()) Run() {
    ResultInterp::Run();
    return type::c_value();
  }
};
}  // namespace crisp
#endif  //CRISP_MATCH_HPP
