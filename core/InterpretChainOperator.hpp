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

#ifndef CRISP_INTERPRETCHAINOPERATOR_HPP
#define CRISP_INTERPRETCHAINOPERATOR_HPP
#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

/**
 * Interpret Add<n1,n2,n3,...>
 * @tparam Environ
 * @tparam L
 * @tparam R
 */
template <typename Environ, typename L, typename R>
struct Interpret<Add<L, R>, Environ> {
  using env = Environ;
  using LInterp = Interpret<L, Environ>;
  using RInterp = Interpret<R, Environ>;
  typedef typename AddImpl<typename LInterp::type, typename RInterp::type>::type
      type;

  static decltype(type::c_value()) Run() {
    LInterp::Run();
    RInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename L, typename R, typename... Args>
struct Interpret<Add<L, R, Args...>, Environ> {
  using LInterp = Interpret<L, Environ>;
  using RInterp = Interpret<R, Environ>;
  using LT =
      typename AddImpl<typename LInterp::type, typename RInterp::type>::type;

  using TailInterp = Interpret<Add<Args...>, Environ>;
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

/**
 * Interp chain operator like Add<n1,n2,n3,...>, Sub<n1,n2,n3,...>, ...
 */
#define InterpretChainOperator(OpName)                                  \
  template <typename Environ, typename L, typename R>                   \
  struct Interpret<OpName<L, R>, Environ> {                             \
    using env = Environ;                                                \
    using LInterp = Interpret<L, Environ>;                              \
    using RInterp = Interpret<R, Environ>;                              \
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
  struct Interpret<OpName<L, R, Args...>, Environ> {                    \
    using LInterp = Interpret<L, Environ>;                              \
    using RInterp = Interpret<R, Environ>;                              \
    using LT = typename OpName##Impl<typename LInterp::type,            \
                                     typename RInterp::type>::type;     \
    using TailInterp = Interpret<Add<Args...>, Environ>;                \
    using RT = typename Interpret<OpName<Args...>, Environ>::type;      \
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

InterpretChainOperator(Sub);
InterpretChainOperator(Mul);
InterpretChainOperator(Mod);
InterpretChainOperator(And);
InterpretChainOperator(Or);
}  // namespace crisp
#endif  //CRISP_INTERPRETCHAINOPERATOR_HPP
