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

#ifndef CRISP_AST_HPP
#define CRISP_AST_HPP

#include <string>

namespace ast {
/// -------------------------------------------------------------------------------------------
/// Boolean value type.
template <bool V>
struct Bool {
  static constexpr const char *repr = "Bool";
  static constexpr bool c_value() { return V; };
  static const bool value = V;
};

/// -------------------------------------------------------------------------------------------
/// Char value type.
template <char V>
struct Char {
  static constexpr const char *repr = "Char";
  static constexpr char c_value() { return V; };
  static const char value = V;
};

/// -------------------------------------------------------------------------------------------
/// Int value type.
template <int V>
struct Int {
  static constexpr const char *repr = "Int";
  static constexpr int c_value() { return V; };
  static const int value = V;
};

/// -------------------------------------------------------------------------------------------
/// String value type.
template <char... args>
struct String;

template <char c>
struct String<c> {
  static constexpr const char *repr = "String";
  static const std::string c_value() { return std::string(1, c); }
};

template <char c, char... args>
struct String<c, args...> {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "String";
  static const std::string c_value() { return std::string(1, c) + String<args...>::c_value(); }
};

/// -------------------------------------------------------------------------------------------
/// Nil type. It likes the `void` type in C++.
struct Nil {
  static constexpr const char *repr = "Nil";
  static constexpr const char *c_value() { return "#nil"; };
};

/// -------------------------------------------------------------------------------------------
/// Undefined type. We use this value when we cannot resolve a variable reference.
struct Undefined {
  static constexpr const char *repr = "Undefined";
  static constexpr const char *c_value() { return "#undefined"; };
};

/// -------------------------------------------------------------------------------------------
/// Variable reference or an identifier used in function parameters and variable definition.
template <char... args>
struct Var : String<args...> {};

/// -------------------------------------------------------------------------------------------
/// Pair(tuple2) value type.
template <typename L, typename R>
struct Pair {
  static constexpr const char *repr = "Pair";
  static constexpr const char *c_value() { return "#pair"; };
};

/// -------------------------------------------------------------------------------------------
/// quote, which will prevent the interpreter's evaluation for given `AST`.
/// Note: Quote< Var<...> > is equivalent to String<...>
// TODO print ast nodes prettily
template <typename AST>
struct Quote {
  static constexpr const char *repr = "Quote";
  static constexpr const char *c_value() { return "#quote"; };
};

/// -------------------------------------------------------------------------------------------
/// Closure type. We use this type to represent a function value
template <typename Environ, typename Func>
struct Closure {
  static constexpr const char *repr = "Closure";
  static constexpr const char *c_value() { return "#closure"; };
};

/// -------------------------------------------------------------------------------------------
/// We use this type to represent a `println` event
template <typename... Args>
struct Println {
  static constexpr const char *repr = "Println";
};

template <typename... Args>
struct Block {
  static constexpr const char *repr = "Block";
};

template <typename... Params>
struct ParamList {
  static constexpr const char *repr = "ParamList";
};

template <typename Cond, typename Body, typename ElseBody>
struct If {
  static constexpr const char *repr = "If";
};

template <typename... Args>
struct Define {
  static constexpr const char *repr = "Define";
};

template <typename... Args>
struct Call {
  static constexpr const char *repr = "Call";
};

template <typename Params, typename Body>
struct Lambda {
  static constexpr const char *repr = "Lambda";
};

/// -------------------------------------------------------------------------------------------
/// List(x,x,x,...)
/// e.g. List<Int<1>, Int<2>> will be Pair<Int<1>,Pair<Int<2>, Nil>>.
template <typename T, typename... Args>
struct List {
  static constexpr const char *repr = "List";
};

/// -------------------------------------------------------------------------------------------
/// cons: construct Pair<L,R> from L and R.
template <typename L, typename R>
struct Cons {
  static constexpr const char *repr = "Cons";
};

/// -------------------------------------------------------------------------------------------
/// car: get the first element of a Pair, e.g. Car<Pair<L,R>> will be L.
template <typename T>
struct Car {
  static constexpr const char *repr = "Car";
};

/// -------------------------------------------------------------------------------------------
/// cdr: get the second element of a Pair, e.g. Cdr<Pair<L,R>> will be R.
template <typename T>
struct Cdr {
  static constexpr const char *repr = "Cdr";
};

/// -------------------------------------------------------------------------------------------
/// +
template <typename L, typename R, typename... Args>
struct Add {
  static constexpr const char *repr = "+";
};

/// -------------------------------------------------------------------------------------------
/// -
template <typename L, typename R, typename... Args>
struct Sub {
  static constexpr const char *repr = "-";
};

/// -------------------------------------------------------------------------------------------
/// *
template <typename L, typename R, typename... Args>
struct Mul {
  static constexpr const char *repr = "*";
};

/// -------------------------------------------------------------------------------------------
/// /
template <typename L, typename R, typename... Args>
struct Mod {
  static constexpr const char *repr = "/";
};

/// -------------------------------------------------------------------------------------------
/// &&
template <typename L, typename R, typename... Args>
struct And {
  static constexpr const char *repr = "and";
};

/// -------------------------------------------------------------------------------------------
/// ||
template <typename L, typename R, typename... Args>
struct Or {
  static constexpr const char *repr = "or";
};

/// -------------------------------------------------------------------------------------------
/// ==
template <typename L, typename R>
struct IsEqual {
  static constexpr const char *repr = "==";
};

/// -------------------------------------------------------------------------------------------
/// >
template <typename L, typename R>
struct IsGreaterThan {
  static constexpr const char *repr = ">";
};

/// -------------------------------------------------------------------------------------------
/// <
template <typename L, typename R>
struct IsLessThan {
  static constexpr const char *repr = "<";
};

/// -------------------------------------------------------------------------------------------
/// >=
template <typename L, typename R>
struct IsGreaterEqual {
  static constexpr const char *repr = ">=";
};

/// -------------------------------------------------------------------------------------------
/// <=
template <typename L, typename R>
struct IsLessEqual {
  static constexpr const char *repr = "<=";
};

/// -------------------------------------------------------------------------------------------
/// Placeholder used in match expression which will match any value
struct _ {
  static constexpr const char *repr = "_";
};

/// -------------------------------------------------------------------------------------------
/// Placeholder used in match expression which will match any sequence
/// e.g.
/// ```
///    Match< List<Int<1>, Int<2>, Int<3>, Int<4> >,
///           Case< List<___, Var<'l','a','s','t'> >, Var<'l','a','s','t'> >
/// ```
/// will be ```Int<4>```
struct ___ {
  static constexpr const char *repr = "___";
};

/// -------------------------------------------------------------------------------------------
/// case branch used in `Match` expression
template <typename Condition, typename Result>
struct Case {
  static constexpr const char *repr = "Case";
};

/// -------------------------------------------------------------------------------------------
/// default branch used in `Match` and `Cond` expression
template <typename Expr>
struct Default {
  static constexpr const char *repr = "Default";
};

/// -------------------------------------------------------------------------------------------
/// when, branch used in `Match` expression
template <typename Condition, typename Result>
struct When {
  static constexpr const char *repr = "When";
};

/// -------------------------------------------------------------------------------------------
/// else, branch used in `Match` and `Cond` expression
template <typename Expr>
struct Else {
  static constexpr const char *repr = "Else";
};

/// -------------------------------------------------------------------------------------------
/// match, e.g. Match< Add< Int<1>, Int<2> >,
///                    Case< Add<_,_>, Str<'+'> >,
///                    Case< Mul<_,_>, Str<'*'> >,
///                    Default< Str<' '>> >
template <typename AST, typename... Branches>
struct Match {
  static constexpr const char *repr = "Match";
};

/// -------------------------------------------------------------------------------------------
/// capture in match expression
template <typename Target, typename VarName>
struct Capture {
  static constexpr const char *repr = "capture";
};

/// -------------------------------------------------------------------------------------------
/// cond, e.g. Cond< When<true, Int<1>>,
///                  When<false, Int<2>>,
///                  Else<Int<3>> >
template <typename... Branches>
struct Cond {
  static constexpr const char *repr = "Cond";
};
}  // namespace ast

#endif  //CRISP_AST_HPP
