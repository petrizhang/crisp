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

#ifndef CRISP_INTERPRETUSERDEFINEDAST_HPP
#define CRISP_INTERPRETUSERDEFINEDAST_HPP

#include "Common.hpp"
#include "CrispFunction.hpp"

namespace crisp {

template <typename Environ, typename F, typename... Args>
struct CallCrispFunction {
  using type = Interpret<Call<typename F::__name__, Args...>,
                         Environ>;
};

template <template <typename...> class F, typename Environ, typename... Args>
struct Interpret<F<Args...>, Environ> {
  using FApply = F<Args...>;

  using FApplyInterp = typename ConditionalApply<
      When<Bool<is_base_of<CrispFunction, FApply>::value>,
           DeferApply<CallCrispFunction, Environ, FApply, Args...>>,
      Else<DeferConstruct<Id, FApply>>>::type;

  using env = Environ;
  using type = typename FApplyInterp::type;
};
}  // namespace crisp

#endif  // CRISP_INTERPRETUSERDEFINEDAST_HPP
