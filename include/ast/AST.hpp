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
/**
 * Boolean literal.
 * @tparam V
 */
template <bool V>
struct Bool {
  static constexpr const char *repr = "Bool";
  static constexpr bool c_value() { return V; };
  static const bool value = V;
};

/**
 * Char literal.
 * @tparam V
 */
template <char V>
struct Char {
  static constexpr const char *repr = "Char";
  static constexpr char c_value() { return V; };
  static const char value = V;
};

/**
 * Int literal.
 * @tparam V
 */
template <int V>
struct Int {
  static constexpr const char *repr = "Int";
  static constexpr int c_value() { return V; };
  static const int value = V;
};

/**
 * String literal.
 * @tparam args
 */
template <char... args>
struct Str {
  static constexpr const char *c_value() { return value; };
  static constexpr const char value[sizeof...(args) + 1] = {args..., '\0'};
};

template <char... args>
constexpr const char Str<args...>::value[sizeof...(args) + 1];

/**
 * Convert a string literal to a String<...> instance.
 * e.g.
 * @code
 *   using t = decltype("hello"_s);
 *   std::cout << t::value; // hello
 * @endcode
 * @tparam CharT
 * @tparam Chars
 * @return
 */
template <typename CharT, CharT... Chars>
constexpr auto operator""_s() {
  return Str<static_cast<char>(Chars)...>{};
}

/**
 * Nil literal, which equals to `null` in many programming languages.
 */
struct Nil {
  static constexpr const char *repr = "Nil";
  static constexpr const char *c_value() { return "#nil"; };
};

/**
 * Undefined.
 * We use this value when we cannot resolve a variable reference.
 */
struct Undefined {
  static constexpr const char *repr = "Undefined";
  static constexpr const char *c_value() { return "#undefined"; };
};

/**
 * Variable reference or an identifier used in function parameters and variable definition.
 * @tparam args
 */
template <char... args>
struct Var : Str<args...> {};

/**
 * Convert a string literal to a Var<...> instance.
 * @tparam CharT
 * @tparam Chars
 * @return
 */
template <typename CharT, CharT... Chars>
constexpr auto operator""_v() {
  return Var<static_cast<char>(Chars)...>{};
}

/**
 * Pair(tuple2) value type.
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct Pair {
  static constexpr const char *repr = "Pair";
  static constexpr const char *c_value() { return "#pair"; };
};

// TODO print ast nodes prettily
/**
 * Quote, which will prevent the interpreter's evaluation for given `AST`.
 * @tparam AST
 */
template <typename AST>
struct Quote {
  static constexpr const char *repr = "Quote";
  static constexpr const char *c_value() { return "#quote"; };
};

/**
 * Evaluate a quoted expression in current environment.
 * @tparam QuotedExpr
 */
template <typename QuotedExpr>
struct Eval {
  static constexpr const char *repr = "Eval";
  static constexpr const char *c_value() { return "#eval"; };
};

/**
 * Closure. We use this type to represent a function value.
 * @tparam Environ
 * @tparam Func
 */
template <typename Environ, typename Func>
struct Closure {
  static constexpr const char *repr = "Closure";
  static constexpr const char *c_value() { return "#closure"; };
};

/**
 * We use this type to represent a `println` event
 * @tparam Args
 */
template <typename... Args>
struct Println {
  static constexpr const char *repr = "Println";
  static constexpr const char *c_value() { return "#println"; };
};

/**
 * Block.
 * A sequence of expressions.
 * It's result is the result of it's last expression.
 * @tparam Args
 */
template <typename... Args>
struct Block {
  static constexpr const char *repr = "Block";
  static constexpr const char *c_value() { return "#block"; };
};

/**
 * Parameter list used in function definition.
 * @tparam Params
 */
template <typename... Params>
struct ParamList {
  static constexpr const char *repr = "ParamList";
  static constexpr const char *c_value() { return "#paramList"; };
};

/**
 * If-then-else expression.
 * @tparam Cond
 * @tparam Body
 * @tparam ElseBody
 */
template <typename Cond, typename Body, typename ElseBody>
struct If {
  static constexpr const char *repr = "If";
  static constexpr const char *c_value() { return "#if"; };
};

/**
 * Define statement.
 * @tparam Args
 */
template <typename... Args>
struct Define {
  static constexpr const char *repr = "Define";
  static constexpr const char *c_value() { return "#define"; };
};

/**
 * Function call.
 * @tparam Args
 */
template <typename... Args>
struct Call {
  static constexpr const char *repr = "Call";
  static constexpr const char *c_value() { return "#call"; };
};

/**
 * Lambda literal.
 * @tparam Params
 * @tparam Body
 */
template <typename Params, typename Body>
struct Lambda {
  static constexpr const char *repr = "Lambda";
  static constexpr const char *c_value() { return "#lambda"; };
};

/**
 * List
 * @tparam Args
 */
template <typename... Args>
struct List {
  static constexpr const char *repr = "List";
  static constexpr const char *c_value() { return "#list"; };
};

template <typename... Args>
struct Head {
  static constexpr const char *repr = "Head";
  static constexpr const char *c_value() { return "#head"; };
};

template <typename... Args>
struct Tail {
  static constexpr const char *repr = "Tail";
  static constexpr const char *c_value() { return "#tail"; };
};

template <typename... Args>
struct PopHead {
  static constexpr const char *repr = "PopHead";
  static constexpr const char *c_value() { return "#pop_head"; };
};

template <typename... Args>
struct PopLast {
  static constexpr const char *repr = "PopLast";
  static constexpr const char *c_value() { return "#pop_last"; };
};

template <typename... Args>
struct PushHead {
  static constexpr const char *repr = "PushLeft";
  static constexpr const char *c_value() { return "#push_head"; };
};

template <typename... Args>
struct PushLast {
  static constexpr const char *repr = "PushLast";
  static constexpr const char *c_value() { return "#push_last"; };
};

template <typename... Args>
struct DropHead {
  static constexpr const char *repr = "DropHead";
  static constexpr const char *c_value() { return "#drop_head"; };
};

template <typename... Args>
struct DropLast {
  static constexpr const char *repr = "DropLast";
  static constexpr const char *c_value() { return "#drop_last"; };
};

template <typename... Args>
struct Concat {
  static constexpr const char *repr = "Concat";
  static constexpr const char *c_value() { return "#concat"; };
};

/**
 * Cons: construct Pair<L,R> from L and R.
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct Cons {
  static constexpr const char *repr = "Cons";
  static constexpr const char *c_value() { return "#cons"; };
};

/**
 * Car: get the first element of a Pair, e.g. Car<Pair<L,R>> will be L.
 * @tparam T
 */
template <typename T>
struct Car {
  static constexpr const char *repr = "Car";
  static constexpr const char *c_value() { return "#car"; };
};

/**
 * cdr: get the second element of a Pair, e.g. Cdr<Pair<L,R>> will be R.
 */
template <typename T>
struct Cdr {
  static constexpr const char *repr = "Cdr";
  static constexpr const char *c_value() { return "#cdr"; };
};

/**
 * +
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Add {
  static constexpr const char *repr = "+";
  static constexpr const char *c_value() { return "#+"; };
};

/**
 * -
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Sub {
  static constexpr const char *repr = "-";
  static constexpr const char *c_value() { return "#-"; };
};

/**
 * *
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Mul {
  static constexpr const char *repr = "*";
  static constexpr const char *c_value() { return "#*"; };
};

/**
 * /
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Mod {
  static constexpr const char *repr = "/";
  static constexpr const char *c_value() { return "#/"; };
};

/**
 * &&
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct And {
  static constexpr const char *repr = "and";
  static constexpr const char *c_value() { return "#and"; };
};

/**
 * ||
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Or {
  static constexpr const char *repr = "or";
  static constexpr const char *c_value() { return "#or"; };
};

/**
 * ==
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsEqual {
  static constexpr const char *repr = "==";
  static constexpr const char *c_value() { return "#=="; };
};

/**
 * >
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsGreaterThan {
  static constexpr const char *repr = ">";
  static constexpr const char *c_value() { return "#>"; };
};

/**
 * <
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsLessThan {
  static constexpr const char *repr = "<";
  static constexpr const char *c_value() { return "#<"; };
};

/**
 * >=
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsGreaterEqual {
  static constexpr const char *repr = ">=";
};

/**
 * <=
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsLessEqual {
  static constexpr const char *repr = "<=";
  static constexpr const char *c_value() { return "#<="; };
};

/**
 * Placeholder used in match expression which will match any value.
 */
struct _ {
  static constexpr const char *repr = "_";
  static constexpr const char *c_value() { return "#_"; };
};

/**
 * Placeholder used in match expression which will match any sequence.
 * e.g.
 * @code
 * Match<List<Int<1>, Int<2>, Int<3>, Int<4> >,
 *       Case< List<___, Var<'l','a','s','t'> >, Var<'l','a','s','t'>
 *       Default< Nil >>
 * @endcode
 * will be ```Int<4>```
 */
struct ___ {
  static constexpr const char *repr = "___";
  static constexpr const char *c_value() { return "#___"; };
};

/**
 * Case branch used in `Match` expression.
 *
 * @tparam Condition
 * @tparam Result
 */
template <typename Condition, typename Result>
struct Case {
  static constexpr const char *repr = "Case";
  static constexpr const char *c_value() { return "#case"; };
};

/**
 * Default branch used in `Match` and `Cond` expression.
 * @tparam Expr
 */
template <typename Expr>
struct Default {
  static constexpr const char *repr = "Default";
  static constexpr const char *c_value() { return "#default"; };
};

/**
 * When branch used in `Match` expression.
 * @tparam Condition
 * @tparam Result
 */
template <typename Condition, typename Result>
struct When {
  static constexpr const char *repr = "When";
  static constexpr const char *c_value() { return "#when"; };
};

/**
 * Else branch used in `Match` and `Cond` expression.
 * @tparam Expr
 */
template <typename Expr>
struct Else {
  static constexpr const char *repr = "Else";
  static constexpr const char *c_value() { return "#else"; };
};

/**
 * Pattern match expression.
 * @tparam AST
 * @tparam Branch1
 * @tparam Branch2
 * @tparam Branches
 */
template <typename AST, typename Branch1, typename Branch2, typename... Branches>
struct Match {
  static constexpr const char *repr = "Match";
  static constexpr const char *c_value() { return "#match"; };
};

/**
 * Capture in match expression.
 * @tparam Target
 * @tparam VarName
 */
template <typename Target, typename VarName>
struct Capture {
  static constexpr const char *repr = "capture";
  static constexpr const char *c_value() { return "#capture"; };
};

/**
 * Cond expression.
 *
 * e.g.
 * @code
 * Cond< When<true, Int<1>>,
 *       When<false, Int<2>>,
 *       Else<Int<3>> >
 * @endcode
 * @tparam Branch1
 * @tparam Branch2
 * @tparam Branches
 */
template <typename Branch1, typename Branch2, typename... Branches>
struct Cond {
  static constexpr const char *repr = "Cond";
  static constexpr const char *c_value() { return "#cond"; };
};

template <typename E>
struct IsEmpty {
  static constexpr const char *repr = "IsEmpty";
  static constexpr const char *c_value() { return "#is_empty"; };
};

template <typename E>
struct IsNil {
  static constexpr const char *repr = "IsNil";
  static constexpr const char *c_value() { return "#is_nil"; };
};

template <typename S>
struct Str2List {
  static constexpr const char *repr = "Str2List";
  static constexpr const char *c_value() { return "#str2list"; };
};

template <typename... Pairs>
struct Dict {};

}  // namespace ast

#endif  //CRISP_AST_HPP
