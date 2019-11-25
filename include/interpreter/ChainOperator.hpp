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

#ifndef CRISP_CHAIN_OPERATOR_HPP
#define CRISP_CHAIN_OPERATOR_HPP
#include "Common.h"

namespace crisp {
using namespace ast;
using namespace util;

/// -------------------------------------------------------------------------------------------
/// Interp Add<n1,n2,n3,...>
template <typename Environ, typename L, typename R>
struct Interp<Add<L, R>, Environ> {
  using env = Environ;
  using LInterp = Interp<L, Environ>;
  using RInterp = Interp<R, Environ>;
  typedef typename AddImpl<typename LInterp::type, typename RInterp::type>::type
      type;

  static decltype(type::c_value()) Run() {
    LInterp::Run();
    RInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename L, typename R, typename... Args>
struct Interp<Add<L, R, Args...>, Environ> {
  using LInterp = Interp<L, Environ>;
  using RInterp = Interp<R, Environ>;
  using LT =
      typename AddImpl<typename LInterp::type, typename RInterp::type>::type;

  using TailInterp = Interp<Add<Args...>, Environ>;
  using RT = typename TailInterp::type;

  using env = Environ;
  using type = typename AddImpl<LT, RT>::type;

  static decltype(type::c_value()) Run() {
    LInterp::Run();
    RInterp::Run();
    TailInterp::Run();
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Interp chain operator like Add<n1,n2,n3,...>, Sub<n1,n2,n3,...>, ...
#define InterpForChainOperator(OpName)                                  \
  template <typename Environ, typename L, typename R>                   \
  struct Interp<OpName<L, R>, Environ> {                                \
    using env = Environ;                                                \
    using LInterp = Interp<L, Environ>;                                 \
    using RInterp = Interp<R, Environ>;                                 \
    typedef typename OpName##Impl<typename LInterp::type,               \
                                  typename RInterp::type>::type type;   \
                                                                        \
    static decltype(type::c_value()) Run() {                            \
      LInterp::Run();                                                   \
      RInterp::Run();                                                   \
      return type::c_value();                                           \
    }                                                                   \
  };                                                                    \
                                                                        \
  template <typename Environ, typename L, typename R, typename... Args> \
  struct Interp<OpName<L, R, Args...>, Environ> {                       \
    using LInterp = Interp<L, Environ>;                                 \
    using RInterp = Interp<R, Environ>;                                 \
    using LT = typename OpName##Impl<typename LInterp::type,            \
                                     typename RInterp::type>::type;     \
    using TailInterp = Interp<Add<Args...>, Environ>;                   \
    using RT = typename Interp<OpName<Args...>, Environ>::type;         \
                                                                        \
    using env = Environ;                                                \
    using type = typename OpName##Impl<LT, RT>::type;                   \
                                                                        \
    static decltype(type::c_value()) Run() {                            \
      LInterp::Run();                                                   \
      RInterp::Run();                                                   \
      TailInterp::Run();                                                \
      return type::c_value();                                           \
    }                                                                   \
  };

InterpForChainOperator(Sub);
InterpForChainOperator(Mul);
InterpForChainOperator(Mod);
InterpForChainOperator(And);
InterpForChainOperator(Or);
}  // namespace crisp
#endif  //CRISP_CHAIN_OPERATOR_HPP
