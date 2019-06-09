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

#ifndef CRISP_HPP
#define CRISP_HPP

#include <array>
#include <cassert>
#include <iostream>
#include <type_traits>
#include "ast.hpp"
#include "util.hpp"

namespace crisp {

using namespace ast;
using namespace util;

/// *******************************************************************************************
/// * Interpreter implementation
/// *******************************************************************************************

template <typename Expr, typename Environ = Env<>>
struct Eval;

/// -------------------------------------------------------------------------------------------
/// Evaluate value types.
template <typename Environ, bool V>
struct Eval<Bool<V>, Environ> {
  using env = Environ;
  using type = Bool<V>;
  static constexpr bool Run() {
    return V;
  }
};

template <typename Environ, char V>
struct Eval<Char<V>, Environ> {
  using env = Environ;
  using type = Char<V>;
  static constexpr char Run() {
    return V;
  }
};

template <typename Environ, int V>
struct Eval<Int<V>, Environ> {
  using env = Environ;
  using type = Int<V>;
  static constexpr int Run() {
    return V;
  }
};

template <typename Environ, char... chars>
struct Eval<String<chars...>, Environ> {
  using env = Environ;
  using type = String<chars...>;
  static std::string Run() {
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Evaluate variable reference. e.g. Var<'n'>
template <char... args, typename Environ>
struct Eval<Var<args...>, Environ> {
  using env = Environ;
  using type = typename EnvLookup<Environ, Var<args...>>::type;

  static decltype(type::c_value()) Run() {
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Evaluate lambda instantiation.
template <typename Environ, typename Body, typename ParamL>
struct Eval<Lambda<ParamL, Body>, Environ> {
  using env = Environ;
  using type = Closure<Environ, Lambda<ParamL, Body>>;

  static constexpr const char *Run() {
    return "#closure";
  }
};

/// -------------------------------------------------------------------------------------------
/// Evaluate println
template <typename Environ, typename Head, typename... Args>
struct Eval<Println<Head, Args...>, Environ> {
  static const char *Run() {
    auto value = Eval<Head, Environ>::Run();
    output(value);
    std::cout << " ";
    Eval<Println<Args...>, Environ>::Run();
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ, typename Head>
struct Eval<Println<Head>, Environ> {
  static const char *Run() {
    auto value = Eval<Head, Environ>::Run();
    output(value);
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ>
struct Eval<Println<>, Environ> {
  static const char *Run() {
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

/// -------------------------------------------------------------------------------------------
/// Evaluate variable definition. e.g. Define<Var<'a'>,Int<1>>
template <typename Environ, typename Ident, typename Value>
struct Eval<Define<Ident, Value>, Environ> {
  using ValueEval = Eval<Value, Environ>;

  using env = typename EnvPut<Environ, Ident, typename ValueEval::type>::type;
  using type = Undefined;

  static decltype(type::c_value()) Run() {
    ValueEval::Run();
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Eval Add<n1,n2,n3,...>
template <typename Environ, typename L, typename R>
struct Eval<Add<L, R>, Environ> {
  using env = Environ;
  using LEval = Eval<L, Environ>;
  using REval = Eval<R, Environ>;
  typedef typename AddImpl<typename LEval::type,
                           typename REval::type>::type type;

  static decltype(type::c_value()) Run() {
    LEval::Run();
    REval::Run();
    return type::c_value();
  }
};

template <typename Environ, typename L, typename R, typename... Args>
struct Eval<Add<L, R, Args...>, Environ> {
  using LEval = Eval<L, Environ>;
  using REval = Eval<R, Environ>;
  using LT = typename AddImpl<typename LEval::type,
                              typename REval::type>::type;

  using TailEval = Eval<Add<Args...>, Environ>;
  using RT = typename TailEval::type;

  using env = Environ;
  using type = typename AddImpl<LT, RT>::type;

  static decltype(type::c_value()) Run() {
    LEval::Run();
    REval::Run();
    TailEval::Run();
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Eval chain operator like Add<n1,n2,n3,...>, Sub<n1,n2,n3,...>, ...
#define EvalForChainOperator(OpName)                                    \
  template <typename Environ, typename L, typename R>                   \
  struct Eval<OpName<L, R>, Environ> {                                  \
    using env = Environ;                                                \
    using LEval = Eval<L, Environ>;                                     \
    using REval = Eval<R, Environ>;                                     \
    typedef typename OpName##Impl<typename LEval::type,                 \
                                  typename REval::type>::type type;     \
                                                                        \
    static decltype(type::c_value()) Run() {                            \
      LEval::Run();                                                     \
      REval::Run();                                                     \
      return type::c_value();                                           \
    }                                                                   \
  };                                                                    \
                                                                        \
  template <typename Environ, typename L, typename R, typename... Args> \
  struct Eval<OpName<L, R, Args...>, Environ> {                         \
    using LEval = Eval<L, Environ>;                                     \
    using REval = Eval<R, Environ>;                                     \
    using LT = typename OpName##Impl<typename LEval::type,              \
                                     typename REval::type>::type;       \
    using TailEval = Eval<Add<Args...>, Environ>;                       \
    using RT = typename Eval<OpName<Args...>, Environ>::type;           \
                                                                        \
    using env = Environ;                                                \
    using type = typename OpName##Impl<LT, RT>::type;                   \
                                                                        \
    static decltype(type::c_value()) Run() {                            \
      LEval::Run();                                                     \
      REval::Run();                                                     \
      TailEval::Run();                                                  \
      return type::c_value();                                           \
    }                                                                   \
  };

EvalForChainOperator(Sub);
EvalForChainOperator(Mul);
EvalForChainOperator(Mod);
EvalForChainOperator(And);
EvalForChainOperator(Or);

/// -------------------------------------------------------------------------------------------
/// Eval IsEqual<L,R>
template <typename Environ, typename L, typename R>
struct Eval<IsEqual<L, R>, Environ> {
  using env = Environ;

  using LEval = Eval<L, Environ>;
  using REval = Eval<R, Environ>;

  using type = typename IsEqualImpl<typename LEval::type,
                                    typename REval::type>::type;

  static decltype(type::c_value()) Run() {
    LEval::Run();
    REval::Run();
    return type::c_value();
  }
};

#define EvalForBinaryOperator(OpName)                               \
  template <typename Environ, typename L, typename R>               \
  struct Eval<OpName<L, R>, Environ> {                              \
    using env = Environ;                                            \
                                                                    \
    using LEval = Eval<L, Environ>;                                 \
    using REval = Eval<R, Environ>;                                 \
                                                                    \
    using type = typename OpName##Impl<typename LEval::type,        \
                                       typename REval::type>::type; \
                                                                    \
    static decltype(type::c_value()) Run() {                        \
      LEval::Run();                                                 \
      REval::Run();                                                 \
      return type::c_value();                                       \
    }                                                               \
  };

EvalForBinaryOperator(IsGreaterThan);
EvalForBinaryOperator(IsLessThan);
EvalForBinaryOperator(IsGreaterEqual);
EvalForBinaryOperator(IsLessEqual);

/// -------------------------------------------------------------------------------------------
/// Evaluate if-then-else expression
template <typename Environ, typename CondEvaluated, typename Body, typename ElseBody>
struct DelayIf {
  using BodyEval = Eval<Body, Environ>;
  using type = typename BodyEval::type;

  static decltype(type::c_value()) Run() {
    BodyEval::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Body, typename ElseBody>
struct DelayIf<Environ, Bool<false>, Body, ElseBody> {
  using ElseBodyEval = Eval<ElseBody, Environ>;
  using type = typename ElseBodyEval::type;

  static decltype(type::c_value()) Run() {
    ElseBodyEval::c_value();
    return type::Run();
  }
};

template <typename Environ, typename Cond, typename Body, typename ElseBody>
struct Eval<If<Cond, Body, ElseBody>, Environ> {
  using env = Environ;

  using CondEval = Eval<Cond, Environ>;
  using CondValue = typename CondEval::type;

  using DelayIfEval = DelayIf<Environ, CondValue, Body, ElseBody>;
  using type = typename DelayIfEval::type;

  static decltype(type::c_value()) Run() {
    CondEval::Run();
    DelayIfEval::Run();
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Evaluate a sequence of expressions. e.g Block< Define<Var<'n'>,1>, Var<'n'>>
/// The result of a block is the result of the last expression int this block
template <typename Environ, typename Head, typename... Tail>
struct Eval<Block<Head, Tail...>, Environ> {
  using env = Environ;

  using HeadEval = Eval<Head, Environ>;
  // Pass the resulted env from `Head` to the next expression's execution
  using TailEval = Eval<Block<Tail...>, typename HeadEval::env>;
  using type = typename TailEval::type;

  static decltype(type::c_value()) Run() {
    HeadEval::Run();
    TailEval::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Head>
struct Eval<Block<Head>, Environ> {
  using HeadEval = Eval<Head, Environ>;

  using env = typename HeadEval::env;
  using type = typename HeadEval::type;

  static decltype(type::c_value()) Run() {
    HeadEval::Run();
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Evaluate argument list for function calls.
template <typename Environ>
struct Eval<Array<>, Environ> {
  using env = Environ;
  using type = Array<>;

  static constexpr const char *Run() {
    return "#undefined";
  }
};

template <typename Environ, typename Head, typename... Tail>
struct Eval<Array<Head, Tail...>, Environ> {
  using HeadEval = Eval<Head, Environ>;
  using HeadValue = typename HeadEval::type;

  using TailEval = Eval<Array<Tail...>, Environ>;
  using TailValue = typename TailEval::type;

  using env = Environ;
  using type = typename ArrayPushFront<TailValue, HeadValue>::type;

  static const char *Run() {
    HeadEval::Run();
    TailEval::Run();
    return "#undefined";
  }
};

/// -------------------------------------------------------------------------------------------
/// Evaluate function calls.
template <typename CallSiteEnviron, typename ClosureV, typename... Args>
struct CallClosure;

template <typename CallSiteEnviron, typename ClosureEnviron,
          typename Body, typename... Params, typename... ArgValues>
struct CallClosure<CallSiteEnviron, Closure<ClosureEnviron, Lambda<ParamList<Params...>, Body>>,
                   Array<ArgValues...>> {
  // Check arguments number
  static_assert(Size<Params...>::value == Size<ArgValues...>::value,
                "Arguments number does't match.");

  // Pack parameters and given arguments to a dict
  using argDict = typename Zip<Array<Params...>, Array<ArgValues...>>::type;

  // Insert the arguments dict to the closest level of the closure's environment
  using executionEnv0 = typename ArrayPushFront<ClosureEnviron, argDict>::type;
  // Add the call site's environment into the end of the environment list,
  // thus the recursive calls could be supported
  using executionEnv = typename ArrayExtendBack<executionEnv0, CallSiteEnviron>::type;

  using env = CallSiteEnviron;
  // Evaluate function body
  using BodyEval = Eval<Body, executionEnv>;
  using type = typename BodyEval::type;

  static decltype(type::c_value()) Run() {
    BodyEval::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Func, typename... Args>
struct Eval<Call<Func, Args...>, Environ> {
  // Evaluate the expression to get a closure value.
  using ClosureEval = Eval<Func, Environ>;
  using ClosureValue = typename ClosureEval::type;

  // Evaluate argument list.
  using ArgEval = Eval<Array<Args...>, Environ>;
  using ArgValues = typename ArgEval::type;
  static_assert(IsCallable<ClosureValue>::value, "Expected a callable function/closure.");

  using env = Environ;
  // Call the closure.
  using type = typename CallClosure<Environ, ClosureValue, ArgValues>::type;

  static decltype(type::c_value()) Run() {
    ClosureEval::Run();
    ArgEval::Run();
    return type::c_value();
  }
};

template <typename Environ, typename... Args>
struct Eval<Closure<Args...>, Environ> {
  using env = Environ;
  using type = Closure<Args...>;

  static constexpr const char *Run() {
    return "#closure";
  }
};

/// -------------------------------------------------------------------------------------------
/// Evaluate quote expression.
template <typename Environ, typename AST>
struct Eval<Quote<AST>, Environ> {
  using env = Environ;
  using type = Quote<AST>;

  static decltype(type::c_value()) *Run() {
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Implementation for match expression.

template <typename Environ, typename... Args>
struct MatchImpl {
  static_assert(Error<Args...>::always_false, "");
};

template <typename Environ, template <typename...> class Op, typename... Args>
struct MatchImpl<Environ, Op<Args...>> {
};

/// -------------------------------------------------------------------------------------------
/// Evaluate match expression.
template <typename Environ, typename Expr, typename... Branches>
struct Eval<Match<Expr, Branches...>, Environ> {
  using env = Environ;
  using ExprEval = Eval<Expr, Environ>;
  using ResultExpr = typename MatchImpl<typename ExprEval::type, Environ>::type;
  using ResultEval = Eval<ResultExpr, Environ>;
  using type = typename ResultEval::type;

  static decltype(type::c_value()) *Run() {
    ExprEval::Run();
    ResultEval::Run();
    return type::c_value();
  }
};

}  // namespace crisp
#endif  // CRISP_HPP
