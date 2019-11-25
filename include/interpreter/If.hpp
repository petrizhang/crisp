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

#ifndef CRISP_IF_HPP
#define CRISP_IF_HPP
#include "Common.h"

// TODO: Refine if implementation
namespace crisp {
using namespace ast;
using namespace util;

/// -------------------------------------------------------------------------------------------
/// Interpret if-then-else expression
template <typename Environ, typename CondInterpretd, typename Body,
          typename ElseBody>
struct DelayIf {
  using BodyInterp = Interp<Body, Environ>;
  using type = typename BodyInterp::type;

  static decltype(type::c_value()) Run() {
    BodyInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Body, typename ElseBody>
struct DelayIf<Environ, Bool<false>, Body, ElseBody> {
  using ElseBodyInterp = Interp<ElseBody, Environ>;
  using type = typename ElseBodyInterp::type;

  static decltype(type::c_value()) Run() {
    ElseBodyInterp::c_value();
    return type::Run();
  }
};

template <typename Environ, typename Cond, typename Body, typename ElseBody>
struct Interp<If<Cond, Body, ElseBody>, Environ> {
  using env = Environ;

  using CondInterp = Interp<Cond, Environ>;
  using CondValue = typename CondInterp::type;

  using DelayIfInterp = DelayIf<Environ, CondValue, Body, ElseBody>;
  using type = typename DelayIfInterp::type;

  static decltype(type::c_value()) Run() {
    CondInterp::Run();
    DelayIfInterp::Run();
    return type::c_value();
  }
};
}  // namespace crisp
#endif  //CRISP_IF_HPP
