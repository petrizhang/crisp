/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
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

namespace crisp {

template <typename T>
inline void output(T v) {
  std::cout << v;
}

inline void output(bool v) {
  std::cout << (v ? "true" : "false");
}

/// ****************************************************************************
/// * AST nodes.
/// ****************************************************************************

/// ----------------------------------------------------------------------------
/// Boolean value type.
template <bool V>
struct Bool {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "bool";
  using c_type = bool;
  static constexpr c_type c_value() { return V; };
};

/// ----------------------------------------------------------------------------
/// Char value type.
template <char V>
struct Char {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "char";
  using c_type = char;
  static constexpr c_type c_value() { return V; };
};

/// ----------------------------------------------------------------------------
/// Int value type.
template <int V>
struct Int {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "int";
  using c_type = int;
  static constexpr c_type c_value() { return V; };
};

/// ----------------------------------------------------------------------------
/// Symbol value type.
template <char... args>
struct Symbol;

template <char c>
struct Symbol<c> {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "symbol";
  using c_type = std::string;
  static const c_type c_value() { return std::string(1, c); }
};

template <char c, char... args>
struct Symbol<c, args...> {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "symbol";
  using c_type = std::string;
  static const c_type c_value() { return std::string(1, c) + Symbol<args...>::c_value(); }
};

/// ----------------------------------------------------------------------------
/// Variable reference or an identifier used in function parameters and variable definition.
template <char... args>
struct Var : Symbol<args...> {};

/// ----------------------------------------------------------------------------
/// Pair(tuple2) value type.
template <typename L, typename R>
struct Pair {};

/// ----------------------------------------------------------------------------
/// Nil type. It likes the `void` type in C++.
struct Nil {
  static constexpr const char *repr = "Nil";
  using c_type = void;
  static constexpr const char *c_value() { return "#nil"; };
};

/// ----------------------------------------------------------------------------
/// Undefined type. We use this value when we cannot resolve a variable reference.
struct Undefined {
  static constexpr const char *repr = "Undefined";
  using c_type = void;
  static constexpr const char *c_value() { return "#undefined"; };
};

/// ----------------------------------------------------------------------------
/// Closure type. We use this type to represent a function value
template <typename Environ, typename Func>
struct Closure {
  static constexpr const char *repr = "Closure";
  static constexpr const char *c_value() { return "#closure"; };
};

/// ----------------------------------------------------------------------------
/// We use this type to represent a `println` event
template <typename... Args>
struct Println {};

template <typename... Args>
struct Block {};

template <typename... Params>
struct ParamList {};

template <typename Cond, typename Body, typename ElseBody>
struct If {};

template <typename... Args>
struct Define {};

template <typename... Args>
struct Call {};

template <typename Params, typename Body>
struct Lambda {};

/// ----------------------------------------------------------------------------
/// List(x,x,x,...)
/// e.g. List<Int<1>, Int<2>> will be Pair<Int<1>,Pair<Int<2>, Nil>>.
template <typename T, typename... Args>
struct List {
  static constexpr const char *repr = "list";
};

/// ----------------------------------------------------------------------------
/// cons: construct Pair<L,R> from L and R.
template <typename L, typename R>
struct Cons {
  static constexpr const char *repr = "cons";
};

/// ----------------------------------------------------------------------------
/// car: get the first element of a Pair, e.g. Car<Pair<L,R>> will be L.
template <typename T>
struct Car {
  static constexpr const char *repr = "car";
};

/// ----------------------------------------------------------------------------
/// cdr: get the second element of a Pair, e.g. Cdr<Pair<L,R>> will be R.
template <typename T>
struct Cdr {
  static constexpr const char *repr = "cdr";
};

/// ----------------------------------------------------------------------------
/// +
template <typename L, typename R, typename... Args>
struct Add {
  static constexpr const char *repr = "+";
};

/// ----------------------------------------------------------------------------
/// -
template <typename L, typename R, typename... Args>
struct Sub {
  static constexpr const char *repr = "-";
};

/// ----------------------------------------------------------------------------
/// *
template <typename L, typename R, typename... Args>
struct Mul {
  static constexpr const char *repr = "*";
};

/// ----------------------------------------------------------------------------
/// /
template <typename L, typename R, typename... Args>
struct Mod {
  static constexpr const char *repr = "/";
};

/// ----------------------------------------------------------------------------
/// &&
template <typename L, typename R, typename... Args>
struct And {
  static constexpr const char *repr = "and";
};

/// ----------------------------------------------------------------------------
/// ||
template <typename L, typename R, typename... Args>
struct Or {
  static constexpr const char *repr = "or";
};

/// ----------------------------------------------------------------------------
/// ==
template <typename L, typename R>
struct IsEqual {
  static constexpr const char *repr = "==";
};

/// ----------------------------------------------------------------------------
/// >
template <typename L, typename R>
struct IsGreaterThan {
  static constexpr const char *repr = ">";
};

/// ----------------------------------------------------------------------------
/// <
template <typename L, typename R>
struct IsLessThan {
  static constexpr const char *repr = "<";
};

/// ----------------------------------------------------------------------------
/// >=
template <typename L, typename R>
struct IsGreaterEqual {
  static constexpr const char *repr = ">=";
};

/// ----------------------------------------------------------------------------
/// <=
template <typename L, typename R>
struct IsLessEqual {
  static constexpr const char *repr = "<=";
};

/// ****************************************************************************
/// * Utilities
/// ****************************************************************************

/// ----------------------------------------------------------------------------
/// Use this type in static_assert to trigger a compiling error.
template <typename...>
struct Error {
  static const bool always_false = false;
};

/// ----------------------------------------------------------------------------
/// Pack a const value(int/char/bool) into Value type.
/// e.g. PackToValue<int, 1>::packed_type will be Value<Int<1>>.
template <typename T, T V>
struct PackToType;

template <bool V>
struct PackToType<bool, V> {
  using type = Bool<V>;
};

template <char V>
struct PackToType<char, V> {
  using type = Char<V>;
};

template <int V>
struct PackToType<int, V> {
  using type = Int<V>;
};

/// ----------------------------------------------------------------------------
/// Check if an expression is callable or not.
template <typename... Args>
struct IsCallable {
  static const bool value = false;
};

template <typename... Args>
struct IsCallable<Closure<Args...>> {
  static const bool value = true;
};

/// ----------------------------------------------------------------------------
/// Get the size of a parameter list.
template <typename... Args>
struct Size;

template <>
struct Size<> {
  static const u_long value = 0;
};

template <typename Head, typename... Tails>
struct Size<Head, Tails...> {
  static const u_long value = 1 + Size<Tails...>::value;
};

/// ----------------------------------------------------------------------------
/// A array-like collection type.
template <typename... Elements>
struct Array {};

template <typename array>
struct ArrayHead;

template <>
struct ArrayHead<Array<>> {
  using type = Nil;
};

template <typename Head, typename... Tail>
struct ArrayHead<Array<Head, Tail...>> {
  using type = Head;
};

template <typename array>
struct ArrayTail;

template <>
struct ArrayTail<Array<>> {
  using type = Array<>;
};

template <typename Head, typename... Tail>
struct ArrayTail<Array<Head, Tail...>> {
  using type = Array<Tail...>;
};

template <typename array, typename Elem>
struct ArrayPushFront;

template <typename Elem, typename... Elements>
struct ArrayPushFront<Array<Elements...>, Elem> {
  using type = Array<Elem, Elements...>;
};

template <typename array, typename Elem>
struct ArrayPushBack;

template <typename Elem, typename... Elements>
struct ArrayPushBack<Array<Elements...>, Elem> {
  using type = Array<Elements..., Elem>;
};

template <typename array>
struct ArrayPopFront;

template <typename Head, typename... Tail>
struct ArrayPopFront<Array<Head, Tail...>> {
  using type = Array<Tail...>;
  using poped = Head;
};

template <typename Environ, typename Extra>
struct ArrayExtendBack;

template <typename... Head, typename... Tail>
struct ArrayExtendBack<Array<Head...>, Array<Tail...>> {
  using type = Array<Head..., Tail...>;
};

/// Zip two arrays to a pair array.
/// An intuitive example: zip([1,2,3],[a,b,c]) => [(1,a),(2,b),(3,c)]
template <typename Keys, typename Values>
struct Zip;

template <typename K, typename V, typename... Keys, typename... Values>
struct Zip<Array<K, Keys...>, Array<V, Values...>> {
  using tailResult = typename Zip<Array<Keys...>, Array<Values...>>::type;
  using type = typename ArrayPushFront<tailResult, Pair<K, V>>::type;
};

template <>
struct Zip<Array<>, Array<>> {
  using type = Array<>;
};

/// ----------------------------------------------------------------------------
/// A map-like collection type.
template <typename... Pairs>
using Dict = Array<Pairs...>;

template <typename dict, typename pair>
using DictPut = ArrayPushFront<dict, pair>;

template <typename dict, typename K>
struct DictGet {
  using type = Nil;
};

template <typename K, typename V>
struct DictGet<Dict<Pair<K, V>>, K> {
  using type = V;
};

template <typename K, typename V, typename... Tail>
struct DictGet<Dict<Pair<K, V>, Tail...>, K> {
  using type = V;
};

template <typename K, typename V, typename T, typename... Tail>
struct DictGet<Dict<Pair<T, V>, Tail...>, K> {
  using type = typename DictGet<Dict<Tail...>, K>::type;
};

/// ----------------------------------------------------------------------------
/// Environment stack implementation.
/// Every element in the stack is a symbol table for a specific lexical scope.
template <typename... Dicts>
using Env = Array<Dicts...>;

template <typename Environ, typename Extra>
using EnvExtendBack = ArrayExtendBack<Environ, Extra>;

template <typename env, typename dict>
using EnvPushFront = ArrayPushFront<env, dict>;

template <typename env>
using EnvPopFront = ArrayPopFront<env>;

template <typename env, typename K, typename V>
struct EnvPut;

template <typename K, typename V>
struct EnvPut<Env<>, K, V> {
  using type = Env<Dict<Pair<K, V>>>;
};

template <typename K, typename V, typename dict, typename... Tail>
struct EnvPut<Env<dict, Tail...>, K, V> {
  using type = Env<typename DictPut<dict, Pair<K, V>>::type, Tail...>;
};

template <typename env, typename K>
struct EnvLookup {
  using type = Undefined;
};

template <typename K, typename dict>
struct EnvLookup<Env<dict>, K> {
  using V = typename DictGet<dict, K>::type;
  using type = typename std::conditional<std::is_same<V, Nil>::value,
                                         Undefined,
                                         V>::type;
};

template <typename K, typename dict, typename... Tail>
struct EnvLookup<Env<dict, Tail...>, K> {
  using current_scope_value = typename DictGet<dict, K>::type;
  using type = typename std::conditional<std::is_same<current_scope_value, Nil>::value,
                                         typename EnvLookup<Env<Tail...>, K>::type,
                                         current_scope_value>::type;
};

/// ----------------------------------------------------------------------------
/// Implementation for `Car`
template <typename T>
struct CarImpl;

template <typename L, typename R>
struct CarImpl<Pair<L, R>> {
  using type = L;
};

/// ----------------------------------------------------------------------------
/// Implementation for `Cdr`
template <typename T>
struct CdrImpl;

template <typename L, typename R>
struct CdrImpl<Pair<L, R>> {
  using type = R;
};

/// ----------------------------------------------------------------------------
/// Implementation for `List`
template <typename T, typename... Args>
struct ListImpl {
  using type = Pair<T, typename List<Args...>::type>;
};

template <typename T>
struct ListImpl<T> {
  using type = Pair<T, Nil>;
};

/// ----------------------------------------------------------------------------
/// Implementation for `Add`
template <typename L, typename R>
struct AddImpl {
  static_assert(Error<L, R>::always_false, "Incompatible types for operation `Add`.");
};

template <int LV, int RV>
struct AddImpl<Int<LV>, Int<RV>> {
  using type = Int<LV + RV>;
};

/// Most binary operators (-,*,%,...) follow the same pattern as
/// `AddImpl`, thus we could implement them with an unified macro.
#define BinaryOperator(OpName, Operator, LeftValueType, LeftType, RightValueType, RightType, \
                       ResultType)                                                           \
  template <typename L, typename R>                                                          \
  struct OpName##Impl {                                                                      \
    static_assert(Error<L, R>::always_false,                                                 \
                  "Incompatible types for operation `" #OpName "`.");                        \
  };                                                                                         \
                                                                                             \
  template <LeftValueType LV, RightValueType RV>                                             \
  struct OpName##Impl<LeftType<LV>, RightType<RV>> {                                         \
    using type = ResultType<(LV Operator RV)>;                                               \
  };

BinaryOperator(Sub, -, int, Int, int, Int, Int);
BinaryOperator(Mul, *, int, Int, int, Int, Int);
BinaryOperator(Mod, %, int, Int, int, Int, Int);
BinaryOperator(And, &&, bool, Bool, bool, Bool, Bool);
BinaryOperator(Or, ||, bool, Bool, bool, Bool, Bool);
BinaryOperator(IsGreaterThan, >, int, Int, int, Int, Bool);
BinaryOperator(IsLessThan, <, int, Int, int, Int, Bool);
BinaryOperator(IsGreaterEqual, >=, int, Int, int, Int, Bool);
BinaryOperator(IsLessEqual, <=, int, Int, int, Int, Bool);

/// ----------------------------------------------------------------------------
/// Implementation for `IsEqual`
/// If two types are the same, then the values they represent are the same.
template <typename L, typename R>
struct IsEqualImpl {
  using type = Bool<std::is_same<L, R>::value>;
};

/// ----------------------------------------------------------------------------
/// Interpreter implementation
template <typename Expr, typename Environ = Env<>>
struct Eval;

template <typename Environ, bool V>
struct Eval<Bool<V>, Environ> {
  using env = Environ;
  using type = Bool<V>;
  static constexpr bool c_value() {
    return V;
  }
};

template <typename Environ, char V>
struct Eval<Char<V>, Environ> {
  using env = Environ;
  using type = Char<V>;
  static constexpr char c_value() {
    return V;
  }
};

template <typename Environ, int V>
struct Eval<Int<V>, Environ> {
  using env = Environ;
  using type = Int<V>;
  static constexpr int c_value() {
    return V;
  }
};

template <typename Environ, char... chars>
struct Eval<Symbol<chars...>, Environ> {
  using env = Environ;
  using type = Symbol<chars...>;
  static std::string c_value() {
    return type::c_value();
  }
};

/// ----------------------------------------------------------------------------
/// Evaluate variable reference. e.g. Var<'n'>
template <char... args, typename Environ>
struct Eval<Var<args...>, Environ> {
  using env = Environ;
  using type = typename EnvLookup<Environ, Var<args...>>::type;

  static decltype(type::c_value()) c_value() {
    return type::c_value();
  }
};

/// ----------------------------------------------------------------------------
/// Evaluate lambda instantiation.
template <typename Environ, typename Body, typename ParamL>
struct Eval<Lambda<ParamL, Body>, Environ> {
  using env = Environ;
  using type = Closure<Environ, Lambda<ParamL, Body>>;

  static constexpr const char *c_value() {
    return "#closure";
  }
};

/// ----------------------------------------------------------------------------
/// Evaluate println
template <typename Environ, typename Head, typename... Args>
struct Eval<Println<Head, Args...>, Environ> {
  static const char *c_value() {
    auto value = Eval<Head, Environ>::c_value();
    output(value);
    std::cout << " ";
    Eval<Println<Args...>, Environ>::c_value();
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ, typename Head>
struct Eval<Println<Head>, Environ> {
  static const char *c_value() {
    auto value = Eval<Head, Environ>::c_value();
    output(value);
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ>
struct Eval<Println<>, Environ> {
  static const char *c_value() {
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

/// ----------------------------------------------------------------------------
/// Evaluate variable definition. e.g. Define<Var<'a'>,Int<1>>
template <typename Environ, typename Ident, typename Value>
struct Eval<Define<Ident, Value>, Environ> {
  using ValueEval = Eval<Value, Environ>;

  using env = typename EnvPut<Environ, Ident, typename ValueEval::type>::type;
  using type = Undefined;

  static decltype(type::c_value()) c_value() {
    ValueEval::c_value();
    return type::c_value();
  }
};

/// ----------------------------------------------------------------------------
/// Eval Add<n1,n2,n3,...>
template <typename Environ, typename L, typename R>
struct Eval<Add<L, R>, Environ> {
  using env = Environ;
  using LEval = Eval<L, Environ>;
  using REval = Eval<R, Environ>;
  typedef typename AddImpl<typename LEval::type,
                           typename REval::type>::type type;

  static decltype(type::c_value()) c_value() {
    LEval::c_value();
    REval::c_value();
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

  static decltype(type::c_value()) c_value() {
    LEval::c_value();
    REval::c_value();
    TailEval::c_value();
    return type::c_value();
  }
};

/// ----------------------------------------------------------------------------
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
    static decltype(type::c_value()) c_value() {                        \
      LEval::c_value();                                                 \
      REval::c_value();                                                 \
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
    static decltype(type::c_value()) c_value() {                        \
      LEval::c_value();                                                 \
      REval::c_value();                                                 \
      TailEval::c_value();                                              \
      return type::c_value();                                           \
    }                                                                   \
  };

EvalForChainOperator(Sub);
EvalForChainOperator(Mul);
EvalForChainOperator(Mod);
EvalForChainOperator(And);
EvalForChainOperator(Or);

/// ----------------------------------------------------------------------------
/// Eval IsEqual<L,R>
template <typename Environ, typename L, typename R>
struct Eval<IsEqual<L, R>, Environ> {
  using env = Environ;

  using LEval = Eval<L, Environ>;
  using REval = Eval<R, Environ>;

  using type = typename IsEqualImpl<typename LEval::type,
                                    typename REval::type>::type;

  static decltype(type::c_value()) c_value() {
    LEval::c_value();
    REval::c_value();
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
    static decltype(type::c_value()) c_value() {                    \
      LEval::c_value();                                             \
      REval::c_value();                                             \
      return type::c_value();                                       \
    }                                                               \
  };

EvalForBinaryOperator(IsGreaterThan);
EvalForBinaryOperator(IsLessThan);
EvalForBinaryOperator(IsGreaterEqual);
EvalForBinaryOperator(IsLessEqual);

/// ----------------------------------------------------------------------------
/// Evaluate if-then-else expression
template <typename Environ, typename CondEvaluated, typename Body, typename ElseBody>
struct DelayIf {
  using BodyEval = Eval<Body, Environ>;
  using type = typename BodyEval::type;

  static decltype(type::c_value()) c_value() {
    BodyEval::c_value();
    return type::c_value();
  }
};

template <typename Environ, typename Body, typename ElseBody>
struct DelayIf<Environ, Bool<false>, Body, ElseBody> {
  using ElseBodyEval = Eval<ElseBody, Environ>;
  using type = typename ElseBodyEval::type;

  static decltype(type::c_value()) c_value() {
    ElseBodyEval::c_value();
    return type::c_value();
  }
};

template <typename Environ, typename Cond, typename Body, typename ElseBody>
struct Eval<If<Cond, Body, ElseBody>, Environ> {
  using env = Environ;

  using CondEval = Eval<Cond, Environ>;
  using CondValue = typename CondEval::type;

  using DelayIfEval = DelayIf<Environ, CondValue, Body, ElseBody>;
  using type = typename DelayIfEval::type;

  static decltype(type::c_value()) c_value() {
    CondEval::c_value();
    DelayIfEval::c_value();
    return type::c_value();
  }
};

/// ----------------------------------------------------------------------------
/// Evaluate a sequence of expressions. e.g Block< Define<Var<'n'>,1>, Var<'n'>>
/// The result of a block is the result of the last expression int this block
template <typename Environ, typename Head, typename... Tail>
struct Eval<Block<Head, Tail...>, Environ> {
  using env = Environ;

  using HeadEval = Eval<Head, Environ>;
  // Pass the resulted env from `Head` to the next expression's execution
  using TailEval = Eval<Block<Tail...>, typename HeadEval::env>;
  using type = typename TailEval::type;

  static decltype(type::c_value()) c_value() {
    HeadEval::c_value();
    TailEval::c_value();
    return type::c_value();
  }
};

template <typename Environ, typename Head>
struct Eval<Block<Head>, Environ> {
  using HeadEval = Eval<Head, Environ>;

  using env = typename HeadEval::env;
  using type = typename HeadEval::type;

  static decltype(type::c_value()) c_value() {
    HeadEval::c_value();
    return type::c_value();
  }
};

/// ----------------------------------------------------------------------------
/// Evaluate argument list for function calls.
template <typename Environ>
struct Eval<Array<>, Environ> {
  using env = Environ;
  using type = Array<>;

  static constexpr const char *c_value() {
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

  static const char *c_value() {
    HeadEval::c_value();
    TailEval::c_value();
    return "#undefined";
  }
};

/// ----------------------------------------------------------------------------
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

  static decltype(type::c_value()) c_value() {
    BodyEval::c_value();
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

  static decltype(type::c_value()) c_value() {
    ClosureEval::c_value();
    ArgEval::c_value();
    return type::c_value();
  }
};

template <typename Environ, typename... Args>
struct Eval<Closure<Args...>, Environ> {
  using env = Environ;
  using type = Closure<Args...>;

  static constexpr const char *c_value() {
    return "#closure";
  }
};
}  // namespace crisp
#endif  // CRISP_HPP