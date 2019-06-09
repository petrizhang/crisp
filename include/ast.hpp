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

namespace ast {
/// -------------------------------------------------------------------------------------------
/// Boolean value type.
template <bool V>
struct Bool {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "bool";
  using c_type = bool;
  static constexpr c_type c_value() { return V; };
};

/// -------------------------------------------------------------------------------------------
/// Char value type.
template <char V>
struct Char {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "char";
  using c_type = char;
  static constexpr c_type c_value() { return V; };
};

/// -------------------------------------------------------------------------------------------
/// Int value type.
template <int V>
struct Int {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "int";
  using c_type = int;
  static constexpr c_type c_value() { return V; };
};

/// -------------------------------------------------------------------------------------------
/// String value type.
template <char... args>
struct String;

template <char c>
struct String<c> {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "symbol";
  using c_type = std::string;
  static const c_type c_value() { return std::string(1, c); }
};

template <char c, char... args>
struct String<c, args...> {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *repr = "symbol";
  using c_type = std::string;
  static const c_type c_value() { return std::string(1, c) + String<args...>::c_value(); }
};

/// -------------------------------------------------------------------------------------------
/// Nil type. It likes the `void` type in C++.
struct Nil {
  static constexpr const char *repr = "Nil";
  using c_type = void;
  static constexpr const char *c_value() { return "#nil"; };
};

/// -------------------------------------------------------------------------------------------
/// Undefined type. We use this value when we cannot resolve a variable reference.
struct Undefined {
  static constexpr const char *repr = "Undefined";
  using c_type = void;
  static constexpr const char *c_value() { return "#undefined"; };
};

/// -------------------------------------------------------------------------------------------
/// Pair(tuple2) value type.
template <typename L, typename R>
struct Pair {};

/// -------------------------------------------------------------------------------------------
/// quote, which will prevent the interpreter's evaluation for given `AST`.
/// Note: Quote< Var<...> > is equivalent to String<...>
// TODO print ast nodes prettily
template <typename AST>
struct Quote {
  static constexpr const char *repr = "quote";
  using c_type = std::string;
  static c_type c_value() { return "#quote"; };
};

/// -------------------------------------------------------------------------------------------
/// Variable reference or an identifier used in function parameters and variable definition.
template <char... args>
struct Var : String<args...> {};

/// -------------------------------------------------------------------------------------------
/// Closure type. We use this type to represent a function value
template <typename Environ, typename Func>
struct Closure {
  static constexpr const char *repr = "closure";
  static constexpr const char *c_value() { return "#closure"; };
};

/// -------------------------------------------------------------------------------------------
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

/// -------------------------------------------------------------------------------------------
/// List(x,x,x,...)
/// e.g. List<Int<1>, Int<2>> will be Pair<Int<1>,Pair<Int<2>, Nil>>.
template <typename T, typename... Args>
struct List {
  static constexpr const char *repr = "list";
};

/// -------------------------------------------------------------------------------------------
/// cons: construct Pair<L,R> from L and R.
template <typename L, typename R>
struct Cons {
  static constexpr const char *repr = "cons";
};

/// -------------------------------------------------------------------------------------------
/// car: get the first element of a Pair, e.g. Car<Pair<L,R>> will be L.
template <typename T>
struct Car {
  static constexpr const char *repr = "car";
};

/// -------------------------------------------------------------------------------------------
/// cdr: get the second element of a Pair, e.g. Cdr<Pair<L,R>> will be R.
template <typename T>
struct Cdr {
  static constexpr const char *repr = "cdr";
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
struct _ {};

/// -------------------------------------------------------------------------------------------
/// Placeholder used in match expression which will match any sequence
/// e.g.
/// ```
///    Match< List<Int<1>, Int<2>, Int<3>, Int<4> >,
///           Case< List<___, Var<'l','a','s','t'> >, Var<'l','a','s','t'> >
/// ```
/// will be ```Int<4>```
struct ___ {};

/// -------------------------------------------------------------------------------------------
/// case branch used in `Match` expression
template <typename Condition, typename Result>
struct Case {
  static constexpr const char *repr = "case";
};

/// -------------------------------------------------------------------------------------------
/// default branch used in `Match` and `Cond` expression
template <typename Expr>
struct Default {
  static constexpr const char *repr = "default";
};

/// -------------------------------------------------------------------------------------------
/// when, branch used in `Match` expression
template <typename Condition, typename Result>
struct When {
  static constexpr const char *repr = "when";
};

/// -------------------------------------------------------------------------------------------
/// else, branch used in `Match` and `Cond` expression
template <typename Expr>
struct Else {
  static constexpr const char *repr = "else";
};

/// -------------------------------------------------------------------------------------------
/// match, e.g. Match< Add< Int<1>, Int<2> >,
///                    Case< Add<_,_>, Str<'+'> >,
///                    Case< Mul<_,_>, Str<'*'> >,
///                    Default< Str<' '>> >
template <typename AST, typename... Branches>
struct Match {
  static constexpr const char *repr = "match";
};

/// -------------------------------------------------------------------------------------------
/// cond, e.g. Cond< When<true, Int<1>>,
///                  When<false, Int<2>>,
///                  Else<Int<3>> >
template <typename... Branches>
struct Cond {
  static constexpr const char *repr = "cond";
};
}  // namespace ast

#endif  //CRISP_AST_HPP
