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

#ifndef CRISP_BINARYOPERATOR_HPP
#define CRISP_BINARYOPERATOR_HPP
#include "Common.h"

namespace crisp {
using namespace ast;
using namespace util;

/// -------------------------------------------------------------------------------------------
/// Interp IsEqual<L,R>
template <typename Environ, typename L, typename R>
struct Interp<IsEqual<L, R>, Environ> {
  using env = Environ;

  using LInterp = Interp<L, Environ>;
  using RInterp = Interp<R, Environ>;

  using type = typename IsEqualImpl<typename LInterp::type,
                                    typename RInterp::type>::type;

  static decltype(type::c_value()) Run() {
    LInterp::Run();
    RInterp::Run();
    return type::c_value();
  }
};

#define InterpForBinaryOperator(OpName)                               \
  template <typename Environ, typename L, typename R>                 \
  struct Interp<OpName<L, R>, Environ> {                              \
    using env = Environ;                                              \
                                                                      \
    using LInterp = Interp<L, Environ>;                               \
    using RInterp = Interp<R, Environ>;                               \
                                                                      \
    using type = typename OpName##Impl<typename LInterp::type,        \
                                       typename RInterp::type>::type; \
                                                                      \
    static decltype(type::c_value()) Run() {                          \
      LInterp::Run();                                                 \
      RInterp::Run();                                                 \
      return type::c_value();                                         \
    }                                                                 \
  };

InterpForBinaryOperator(IsGreaterThan);
InterpForBinaryOperator(IsLessThan);
InterpForBinaryOperator(IsGreaterEqual);
InterpForBinaryOperator(IsLessEqual);
}  // namespace crisp
#endif  //CRISP_BINARYOPERATOR_HPP
