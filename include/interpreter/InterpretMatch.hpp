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

#ifndef CRISP_INTERPRETMATCH_HPP
#define CRISP_INTERPRETMATCH_HPP

#include "Common.hpp"
#include "util/InternalList.hpp"

namespace crisp {
using namespace ast;
using namespace util;

template <typename...>
struct MatchList {};

// TODO: check and test pattern match carefully.
/**
 * Interpret an match expression.
 *
 * @tparam Environ
 * @tparam InterpretedExpr
 * @tparam CaseBranch
 * @tparam DefaultBranch
 */
template <typename Environ, typename Expr, typename... Branches>
struct Interpret<Match<Expr, Branches...>, Environ> {
  using ExprInterp = Interpret<Expr, Environ>;
  using InterpretedExpr = typename ExprInterp::type;

  using type = typename Interpret<MatchList<InterpretedExpr, Branches...>>::type;
  using env = Environ;

  static decltype(type::c_value()) Run() {
    ExprInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename AST, typename CaseBranch, typename DefaultBranch>
struct Interpret<MatchList<AST, CaseBranch, DefaultBranch>, Environ> {
  static_assert(IsTemplateOf<Case, CaseBranch>::value, "expected a valid case branch");
  static_assert(IsTemplateOf<Default, DefaultBranch>::value, "expected a default branch");

  using CaseCondition = typename GetCaseCondition<CaseBranch>::type;
  using CaseResult = typename GetCaseResult<CaseBranch>::type;
  using DefaultResult = typename GetDefaultResult<DefaultBranch>::type;

  using CaseBranchMatch = MatchCase<Dict<>, AST, CaseCondition>;
  using IsCaseBranchMatch = Bool<CaseBranchMatch::matched>;

  using FirstResult = typename Conditional<When<IsCaseBranchMatch, CaseResult>,
                                           Else<DefaultResult>>::type;

  using CapturedDict = typename Conditional<
      When<IsCaseBranchMatch, typename CaseBranchMatch::dict>,
      Else<Dict<>>>::type;

  using type = typename Replace<FirstResult, CapturedDict>::type;
  using env = Environ;

  static decltype(type::c_value()) Run() {
    return type::c_value();
  }
};

template <typename Environ, typename Expr, typename Branch1, typename Branch2,
          typename Branch3, typename... Branches>
struct Interpret<MatchList<Expr, Branch1, Branch2, Branch3, Branches...>, Environ> {
  static_assert(IsTemplateOf<Case, Branch1>::value, "expected a valid case branch");
  static_assert(IsTemplateOf<Case, Branch2>::value, "expected a valid case branch");

  using ExprInterp = Interpret<Expr, Environ>;
  using AST = typename ExprInterp::type;

  using CaseCondition = typename GetCaseCondition<Branch1>::type;
  using CaseResult = typename GetCaseResult<Branch1>::type;

  using CaseBranchMatch = MatchCase<Dict<>, AST, CaseCondition>;
  using IsCaseBranchMatch = Bool<CaseBranchMatch::matched>;

  using FirstResult = typename Conditional<When<IsCaseBranchMatch, CaseResult>,
                                           Else<Nil>>::type;

  using CapturedDict = typename Conditional<
      When<IsCaseBranchMatch, typename CaseBranchMatch::dict>,
      Else<Dict<>>>::type;

  using Result = typename ConditionalApply<
      When<IsCaseBranchMatch, DeferConstruct<Replace, FirstResult, CapturedDict>>,
      Else<DeferConstruct<Interpret, MatchList<Expr, Branch2, Branch3, Branches...>,
                          Environ>>>::type;

  using type = typename Result::type;
  using env = Environ;

  static decltype(type::c_value()) Run() {
    return type::c_value();
  }
};

}  // namespace crisp
#endif  //CRISP_INTERPRETMATCH_HPP
