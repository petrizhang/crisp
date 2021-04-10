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

#ifndef CRISP_INTERPRETIF_HPP
#define CRISP_INTERPRETIF_HPP
#include "Common.hpp"

// TODO: Refine if implementation
namespace crisp {
using namespace crisp;
using namespace util;

/**
 * Interpret an if-then-else expression
 * @tparam Environ
 * @tparam CondInterpretd
 * @tparam Body
 * @tparam ElseBody
 */
template <typename Environ, typename CondInterpretd, typename Body,
          typename ElseBody>
struct DelayIf {
  using BodyInterp = Interpret<Body, Environ>;
  using type = typename BodyInterp::type;
};

template <typename Environ, typename Body, typename ElseBody>
struct DelayIf<Environ, Bool<false>, Body, ElseBody> {
  using ElseBodyInterp = Interpret<ElseBody, Environ>;
  using type = typename ElseBodyInterp::type;
};

template <typename Environ, typename Cond, typename Body, typename ElseBody>
struct Interpret<If<Cond, Body, ElseBody>, Environ> {
  using env = Environ;

  using CondInterp = Interpret<Cond, Environ>;
  using CondValue = typename CondInterp::type;

  using DelayIfInterp = DelayIf<Environ, CondValue, Body, ElseBody>;
  using type = typename DelayIfInterp::type;
};
}  // namespace crisp
#endif  //CRISP_INTERPRETIF_HPP
