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

#ifndef CRISP_INTERPRETCALL_HPP
#define CRISP_INTERPRETCALL_HPP
#include "Common.hpp"
#include "InterpretInternalList.hpp"
#include "crisp/util/InternalList.hpp"

// TODO: Refine `call` implementation.
namespace crisp {
using namespace crisp;
using namespace util;
using util::internal::InternalList;

/**
 * Interpret function calls.
 * @tparam CallSiteEnviron
 * @tparam ClosureV
 * @tparam Args
 */
template <typename CallSiteEnviron, typename ClosureV, typename... Args>
struct CallClosure;

template <typename CallSiteEnviron, typename ClosureEnviron, typename Body,
          typename... Params, typename... ArgValues>
struct CallClosure<CallSiteEnviron,
                   Closure<ClosureEnviron, Lambda<ParamList<Params...>, Body>>,
                   InternalList<ArgValues...>> {
  // Check arguments number
  static_assert(sizeof...(Params) == sizeof...(ArgValues),
                "Arguments number does't match.");

  // Pack parameters and given arguments to a dict
  using ArgDict = typename ZipToDict<List<Params...>, List<ArgValues...>>::type;

  // Insert the arguments dict to the closest level of the closure's environment
  using ExecutionEnv0 = typename EnvPushHead<ClosureEnviron, ArgDict>::type;
  // Add the call site's environment into the end of the environment list,
  // thus the recursive calls could be supported
  using ExecutionEnv = typename EnvConcat<ExecutionEnv0, CallSiteEnviron>::type;

  using env = CallSiteEnviron;
  // Interpret function body
  using BodyInterp = Interpret<Body, ExecutionEnv>;
  using type = typename BodyInterp::type;
};

template <typename Environ, typename Func, typename... Args>
struct Interpret<Call<Func, Args...>, Environ> {
  // Interpret the expression to get a closure value.
  using ClosureInterp = Interpret<Func, Environ>;
  using ClosureValue = typename ClosureInterp::type;

  // Interpret argument list.
  using ArgInterp = Interpret<InternalList<Args...>, Environ>;
  using ArgValues = typename ArgInterp::type;
  static_assert(IsCallableImpl<ClosureValue>::value,
                "Expected a callable function/closure.");

  using env = Environ;
  // Call the closure.
  using type = typename CallClosure<Environ, ClosureValue, ArgValues>::type;
};

template <typename Environ, typename... Args>
struct Interpret<Closure<Args...>, Environ> {
  using env = Environ;
  using type = Closure<Args...>;
};
}  // namespace crisp
#endif  //CRISP_INTERPRETCALL_HPP
