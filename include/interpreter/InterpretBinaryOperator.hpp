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

#ifndef CRISP_INTERPRETBINARYOPERATOR_HPP
#define CRISP_INTERPRETBINARYOPERATOR_HPP
#include "Common.hpp"

namespace crisp {
using namespace ast;
using namespace util;

/**
 * Interpret IsEqual<L,R>
 * @tparam Environ 
 * @tparam L 
 * @tparam R 
 */
template <typename Environ, typename L, typename R>
struct Interpret<IsEqual<L, R>, Environ> {
  using env = Environ;

  using LInterp = Interpret<L, Environ>;
  using RInterp = Interpret<R, Environ>;

  using type = typename IsEqualImpl<typename LInterp::type,
                                    typename RInterp::type>::type;

  static decltype(type::c_value()) Run() {
    LInterp::Run();
    RInterp::Run();
    return type::c_value();
  }
};

#define InterpretForBinaryOperator(OpName)                               \
  template <typename Environ, typename L, typename R>                 \
  struct Interpret<OpName<L, R>, Environ> {                              \
    using env = Environ;                                              \
                                                                      \
    using LInterp = Interpret<L, Environ>;                               \
    using RInterp = Interpret<R, Environ>;                               \
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

InterpretForBinaryOperator(IsGreaterThan);
InterpretForBinaryOperator(IsLessThan);
InterpretForBinaryOperator(IsGreaterEqual);
InterpretForBinaryOperator(IsLessEqual);
}  // namespace crisp
#endif  //CRISP_INTERPRETBINARYOPERATOR_HPP
