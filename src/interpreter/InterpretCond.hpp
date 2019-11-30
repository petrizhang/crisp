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

#ifndef CRISP_INTERPRETCOND_HPP
#define CRISP_INTERPRETCOND_HPP

#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

template <typename Environ, typename WhenBranch, typename ElseBranch>
struct Interpret<Cond<WhenBranch, ElseBranch>, Environ> {
  static_assert(IsTemplateOf<When, WhenBranch>::value, "expected a `When` branch");
  static_assert(IsTemplateOf<Else, ElseBranch>::value, "expected an `Else` branch");

  using WhenConditionExpr = typename ListLikeHead<WhenBranch>::type;
  using WhenResultExpr = typename ListLikeLast<WhenBranch>::type;

  using WhenConditionExprInterp = Interpret<WhenConditionExpr, Environ>;
  using WhenConditionValue = typename WhenConditionExprInterp::type;

  using ElseExpr = typename ListLikeHead<ElseBranch>::type;

  using ResultInterp = typename ConditionalApply<
      When<WhenConditionValue, DeferConstruct<Interpret, WhenResultExpr, Environ>>,
      Else<DeferConstruct<Interpret, ElseExpr, Environ>>>::type;

  using type = typename ResultInterp::type;
  using env = typename ResultInterp::env;

  inline static auto Run() {
    WhenConditionExprInterp::Run();
    ResultInterp::Run();
  }
};

template <typename Environ, typename WhenBranch, typename Branch2, typename... Branches>
struct Interpret<Cond<WhenBranch, Branch2, Branches...>, Environ> {
  static_assert(IsTemplateOf<When, WhenBranch>::value, "expected a `When` branch");

  using WhenConditionExpr = typename ListLikeHead<WhenBranch>::type;
  using WhenResultExpr = typename ListLikeLast<WhenBranch>::type;

  using WhenConditionExprInterp = Interpret<WhenConditionExpr, Environ>;
  using WhenConditionValue = typename WhenConditionExprInterp::type;

  using ResultInterp = typename ConditionalApply<
      When<WhenConditionValue, DeferConstruct<Interpret, WhenResultExpr, Environ>>,
      Else<DeferConstruct<Interpret, Cond<Branch2, Branches...>, Environ>>>::type;

  using type = typename ResultInterp::type;
  using env = typename ResultInterp::env;

  inline static auto Run() {
    WhenConditionExprInterp::Run();
    ResultInterp::Run();
  }
};

}  // namespace crisp
#endif  //CRISP_INTERPRETCOND_HPP
