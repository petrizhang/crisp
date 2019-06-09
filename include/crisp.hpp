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

/// -------------------------------------------------------------------------------------------
/// This class is used to bypass the compiler's type checking
/// when we need to evaluate an expression depends on some specific conditions.
template <typename...>
struct EmptyInterp {
  using env = void;
  static void Run(){};
};

template <typename Expr, typename Environ = Env<>>
struct Interp;

/// -------------------------------------------------------------------------------------------
/// Interpuate value types.
template <typename Environ, bool V>
struct Interp<Bool<V>, Environ> {
  using env = Environ;
  using type = Bool<V>;
  static constexpr bool Run() {
    return V;
  }
};

template <typename Environ, char V>
struct Interp<Char<V>, Environ> {
  using env = Environ;
  using type = Char<V>;
  static constexpr char Run() {
    return V;
  }
};

template <typename Environ, int V>
struct Interp<Int<V>, Environ> {
  using env = Environ;
  using type = Int<V>;
  static constexpr int Run() {
    return V;
  }
};

template <typename Environ, char... chars>
struct Interp<String<chars...>, Environ> {
  using env = Environ;
  using type = String<chars...>;
  static std::string Run() {
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Interpuate variable reference. e.g. Var<'n'>
template <char... args, typename Environ>
struct Interp<Var<args...>, Environ> {
  using env = Environ;
  using type = typename EnvLookup<Environ, Var<args...>>::type;

  static decltype(type::c_value()) Run() {
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Interpuate lambda instantiation.
template <typename Environ, typename Body, typename ParamL>
struct Interp<Lambda<ParamL, Body>, Environ> {
  using env = Environ;
  using type = Closure<Environ, Lambda<ParamL, Body>>;

  static constexpr const char *Run() {
    return "#closure";
  }
};

/// -------------------------------------------------------------------------------------------
/// Interpuate println
template <typename Environ, typename Head, typename... Args>
struct Interp<Println<Head, Args...>, Environ> {
  static const char *Run() {
    auto value = Interp<Head, Environ>::Run();
    output(value);
    std::cout << " ";
    Interp<Println<Args...>, Environ>::Run();
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ, typename Head>
struct Interp<Println<Head>, Environ> {
  static const char *Run() {
    auto value = Interp<Head, Environ>::Run();
    output(value);
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ>
struct Interp<Println<>, Environ> {
  static const char *Run() {
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

/// -------------------------------------------------------------------------------------------
/// Interpuate variable definition. e.g. Define<Var<'a'>,Int<1>>
template <typename Environ, typename Ident, typename Value>
struct Interp<Define<Ident, Value>, Environ> {
  using ValueInterp = Interp<Value, Environ>;

  using env = typename EnvPut<Environ, Ident, typename ValueInterp::type>::type;
  using type = Undefined;

  static decltype(type::c_value()) Run() {
    ValueInterp::Run();
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Interp Add<n1,n2,n3,...>
template <typename Environ, typename L, typename R>
struct Interp<Add<L, R>, Environ> {
  using env = Environ;
  using LInterp = Interp<L, Environ>;
  using RInterp = Interp<R, Environ>;
  typedef typename AddImpl<typename LInterp::type,
                           typename RInterp::type>::type type;

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
  using LT = typename AddImpl<typename LInterp::type,
                              typename RInterp::type>::type;

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
#define InterpForChainOperator(OpName)                                    \
  template <typename Environ, typename L, typename R>                   \
  struct Interp<OpName<L, R>, Environ> {                                  \
    using env = Environ;                                                \
    using LInterp = Interp<L, Environ>;                                     \
    using RInterp = Interp<R, Environ>;                                     \
    typedef typename OpName##Impl<typename LInterp::type,                 \
                                  typename RInterp::type>::type type;     \
                                                                        \
    static decltype(type::c_value()) Run() {                            \
      LInterp::Run();                                                     \
      RInterp::Run();                                                     \
      return type::c_value();                                           \
    }                                                                   \
  };                                                                    \
                                                                        \
  template <typename Environ, typename L, typename R, typename... Args> \
  struct Interp<OpName<L, R, Args...>, Environ> {                         \
    using LInterp = Interp<L, Environ>;                                     \
    using RInterp = Interp<R, Environ>;                                     \
    using LT = typename OpName##Impl<typename LInterp::type,              \
                                     typename RInterp::type>::type;       \
    using TailInterp = Interp<Add<Args...>, Environ>;                       \
    using RT = typename Interp<OpName<Args...>, Environ>::type;           \
                                                                        \
    using env = Environ;                                                \
    using type = typename OpName##Impl<LT, RT>::type;                   \
                                                                        \
    static decltype(type::c_value()) Run() {                            \
      LInterp::Run();                                                     \
      RInterp::Run();                                                     \
      TailInterp::Run();                                                  \
      return type::c_value();                                           \
    }                                                                   \
  };

InterpForChainOperator(Sub);
InterpForChainOperator(Mul);
InterpForChainOperator(Mod);
InterpForChainOperator(And);
InterpForChainOperator(Or);

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
  template <typename Environ, typename L, typename R>               \
  struct Interp<OpName<L, R>, Environ> {                              \
    using env = Environ;                                            \
                                                                    \
    using LInterp = Interp<L, Environ>;                                 \
    using RInterp = Interp<R, Environ>;                                 \
                                                                    \
    using type = typename OpName##Impl<typename LInterp::type,        \
                                       typename RInterp::type>::type; \
                                                                    \
    static decltype(type::c_value()) Run() {                        \
      LInterp::Run();                                                 \
      RInterp::Run();                                                 \
      return type::c_value();                                       \
    }                                                               \
  };

InterpForBinaryOperator(IsGreaterThan);
InterpForBinaryOperator(IsLessThan);
InterpForBinaryOperator(IsGreaterEqual);
InterpForBinaryOperator(IsLessEqual);

/// -------------------------------------------------------------------------------------------
/// Interpuate if-then-else expression
template <typename Environ, typename CondInterpuated, typename Body, typename ElseBody>
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

/// -------------------------------------------------------------------------------------------
/// Interpuate a sequence of expressions. e.g Block< Define<Var<'n'>,1>, Var<'n'>>
/// The result of a block is the result of the last expression int this block
template <typename Environ, typename Head, typename... Tail>
struct Interp<Block<Head, Tail...>, Environ> {
  using env = Environ;

  using HeadInterp = Interp<Head, Environ>;
  // Pass the resulted env from `Head` to the next expression's execution
  using TailInterp = Interp<Block<Tail...>, typename HeadInterp::env>;
  using type = typename TailInterp::type;

  static decltype(type::c_value()) Run() {
    HeadInterp::Run();
    TailInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Head>
struct Interp<Block<Head>, Environ> {
  using HeadInterp = Interp<Head, Environ>;

  using env = typename HeadInterp::env;
  using type = typename HeadInterp::type;

  static decltype(type::c_value()) Run() {
    HeadInterp::Run();
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Interpuate argument list for function calls.
template <typename Environ>
struct Interp<Array<>, Environ> {
  using env = Environ;
  using type = Array<>;

  static constexpr const char *Run() {
    return "#undefined";
  }
};

template <typename Environ, typename Head, typename... Tail>
struct Interp<Array<Head, Tail...>, Environ> {
  using HeadInterp = Interp<Head, Environ>;
  using HeadValue = typename HeadInterp::type;

  using TailInterp = Interp<Array<Tail...>, Environ>;
  using TailValue = typename TailInterp::type;

  using env = Environ;
  using type = typename ArrayPushFront<TailValue, HeadValue>::type;

  static const char *Run() {
    HeadInterp::Run();
    TailInterp::Run();
    return "#undefined";
  }
};

/// -------------------------------------------------------------------------------------------
/// Interpuate function calls.
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
  // Interpuate function body
  using BodyInterp = Interp<Body, executionEnv>;
  using type = typename BodyInterp::type;

  static decltype(type::c_value()) Run() {
    BodyInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Func, typename... Args>
struct Interp<Call<Func, Args...>, Environ> {
  // Interpuate the expression to get a closure value.
  using ClosureInterp = Interp<Func, Environ>;
  using ClosureValue = typename ClosureInterp::type;

  // Interpuate argument list.
  using ArgInterp = Interp<Array<Args...>, Environ>;
  using ArgValues = typename ArgInterp::type;
  static_assert(IsCallable<ClosureValue>::value, "Expected a callable function/closure.");

  using env = Environ;
  // Call the closure.
  using type = typename CallClosure<Environ, ClosureValue, ArgValues>::type;

  static decltype(type::c_value()) Run() {
    ClosureInterp::Run();
    ArgInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename... Args>
struct Interp<Closure<Args...>, Environ> {
  using env = Environ;
  using type = Closure<Args...>;

  static constexpr const char *Run() {
    return "#closure";
  }
};

/// -------------------------------------------------------------------------------------------
/// Interpuate quote expression.
template <typename Environ, typename AST>
struct Interp<Quote<AST>, Environ> {
  using env = Environ;
  using type = Quote<AST>;

  static decltype(type::c_value()) *Run() {
    return type::c_value();
  }
};

/// -------------------------------------------------------------------------------------------
/// Interpuate match expression.
template <typename Environ, typename Expr,
          typename CaseBranch, typename DefaultBranch>
struct Interp<Match<Expr, CaseBranch, DefaultBranch>, Environ> {
  static_assert(IsTemplateOf<Quote, Expr>::value, "only quote matches are supported now");
  static_assert(IsTemplateOf<Case, CaseBranch>::value, "expected a valid case branch");
  static_assert(IsTemplateOf<Default, DefaultBranch>::value, "expected a default branch");

  using AST = typename GetQuoteAST<Expr>::type;
  //  using ExprInterp = Interp<Expr, Environ>;
  //  using ExprInterpResult = typename ExprInterp::type;

  using CaseCondition = typename GetCaseCondition<CaseBranch>::type;
  using CaseResult = typename GetCaseResult<CaseBranch>::type;
  using DefaultResult = typename GetDefaultResult<DefaultBranch>::type;

  using CaseBranchMatch = QuoteMatchCase<Environ, AST, CaseCondition>;
  using _CaseBranchMatched = Bool<CaseBranchMatch::matched>;

  using Result = typename ConditionalImpl<
      When<_CaseBranchMatched, CaseResult>,
      Else<DefaultResult>>::type;

  using MatchedEnv = typename ConditionalImpl<
      When<_CaseBranchMatched, typename CaseBranchMatch::env>,
      Else<Environ>>::type;

  using ResultInterp = Interp<Result, MatchedEnv>;
  using type = typename ResultInterp::type;
  using env = typename ResultInterp::env;

  static decltype(type::c_value()) Run() {
    ResultInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Expr,
          typename Branch1, typename Branch2, typename Branch3, typename... Branches>
struct Interp<Match<Expr, Branch1, Branch2, Branch3, Branches...>, Environ> {
  static_assert(IsTemplateOf<Quote, Expr>::value, "only quote matches are supported now");
  static_assert(IsTemplateOf<Case, Branch1>::value, "expected a valid case branch");
  static_assert(IsTemplateOf<Case, Branch2>::value, "expected a valid case branch");

  using AST = typename GetQuoteAST<Expr>::type;

  using CaseCondition = typename GetCaseCondition<Branch1>::type;
  using CaseResult = typename GetCaseResult<Branch1>::type;

  using CaseBranchMatch = QuoteMatchCase<Environ, AST, CaseCondition>;
  using _CaseBranchMatched = Bool<CaseBranchMatch::matched>;

  using Result = typename ConditionalImpl<
      When<_CaseBranchMatched, CaseResult>,
      Else<Nil>>::type;

  using MatchedEnv = typename ConditionalImpl<
      When<_CaseBranchMatched, typename CaseBranchMatch::env>,
      Else<Environ>>::type;

  using ResultInterp = typename ConditionalApply<
      When<_CaseBranchMatched,
           DeferConstruct<Interp, Result, MatchedEnv>>,
      Else<DeferConstruct<Interp, Match<Expr, Branch2, Branch3, Branches...>, Environ>>>::type;

  using type = typename ResultInterp::type;
  using env = typename ResultInterp::env;

  static decltype(type::c_value()) Run() {
    ResultInterp::Run();
    return type::c_value();
  }
};

}  // namespace crisp
#endif  // CRISP_HPP
