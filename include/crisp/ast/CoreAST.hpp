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

#ifndef CRISP_COREAST_HPP
#define CRISP_COREAST_HPP

#include <string>

namespace crisp {
/**
 * Boolean literal.
 * @tparam V
 */
template <bool V>
struct Bool {
  static const bool value = V;
};

/**
 * Char literal.
 * @tparam V
 */
template <char V>
struct Char {
  static const char value = V;
};

/**
 * Int literal.
 * @tparam V
 */
template <int V>
struct Int {
  static const int value = V;
};

/**
 * String literal.
 * @tparam args
 */
template <char... args>
struct Str {
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
struct Nil {};

/**
 * Undefined.
 * We use this value when we cannot resolve a variable reference.
 */
struct Undefined {};

/**
 * Variable reference or an identifier used in function parameters and variable
 * definition.
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
  static const char* repr() { return "pair"; };
};

// TODO print ast nodes prettily
/**
 * Quote, which will prevent the interpreter's evaluation for given `AST`.
 * @tparam AST
 */
template <typename AST>
struct Quote {
  static const char* repr() { return "quote"; };
};

/**
 * Evaluate a quoted expression in current environment.
 * @tparam QuotedExpr
 */
template <typename QuotedExpr>
struct Eval {
  static const char* repr() { return "eval"; };
};

/**
 * Closure. We use this type to represent a function value.
 * @tparam Environ
 * @tparam Func
 */
template <typename Environ, typename Func>
struct Closure {
  static const char* repr() { return "closure"; };
};

/**
 * Block.
 * A sequence of expressions.
 * It's result is the result of it's last expression.
 * @tparam Args
 */
template <typename... Args>
struct Block {
  static const char* repr() { return "block"; };
};

/**
 * Parameter list used in function definition.
 * @tparam Params
 */
template <typename... Params>
struct ParamList {
  static const char* repr() { return "params"; };
};

/**
 * If-then-else expression.
 * @tparam Cond
 * @tparam Body
 * @tparam ElseBody
 */
template <typename Cond, typename Body, typename ElseBody>
struct If {
  static const char* repr() { return "if"; };
};

/**
 * Define statement.
 * @tparam Args
 */
template <typename... Args>
struct Define {
  static const char* repr() { return "define"; };
};

/**
 * Function call.
 * @tparam Args
 */
template <typename... Args>
struct Call {
  static const char* repr() { return "call"; };
};

/**
 * Lambda literal.
 * @tparam Params
 * @tparam Body
 */
template <typename Params, typename Body>
struct Lambda {
  static const char* repr() { return "lambda"; };
};

/**
 * List
 * @tparam Args
 */
template <typename... Args>
struct List {
  static const char* repr() { return "list"; };
};

template <typename... Args>
struct Head {
  static const char* repr() { return "head"; };
};

template <typename... Args>
struct Tail {
  static const char* repr() { return "tail"; };
};

template <typename... Args>
struct PopFront {
  static const char* repr() { return "pop_front"; };
};

template <typename... Args>
struct PopBack {
  static const char* repr() { return "pop_back"; };
};

template <typename... Args>
struct PushFront {
  static const char* repr() { return "push_front"; };
};

template <typename... Args>
struct PushBack {
  static const char* repr() { return "push_back"; };
};

template <typename... Args>
struct DropHead {
  static const char* repr() { return "drop_head"; };
};

template <typename... Args>
struct DropLast {
  static const char* repr() { return "drop_last"; };
};

template <typename... Args>
struct Concat {
  static const char* repr() { return "concat"; };
};

/**
 * +
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Add {
  static const char* repr() { return "+"; };
};

/**
 * -
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Sub {
  static const char* repr() { return "-"; };
};

/**
 * *
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Mul {
  static const char* repr() { return "*"; };
};

/**
 * /
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Mod {
  static const char* repr() { return "/"; };
};

/**
 * &&
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct And {
  static const char* repr() { return "and"; };
};

/**
 * ||
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Or {
  static const char* repr() { return "or"; };
};

/**
 * ==
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsEqual {
  static const char* repr() { return "=="; };
};

/**
 * >
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsGreaterThan {
  static const char* repr() { return ">"; };
};

/**
 * <
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsLessThan {
  static const char* repr() { return "<"; };
};

/**
 * >=
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsGreaterEqual {
  static const char* repr() { return ">="; };
};

/**
 * <=
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsLessEqual {
  static const char* repr() { return "<="; };
};

/**
 * Placeholder used in match expression which will match any value.
 */
struct _ {
  static const char* repr() { return "-"; };
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
  static const char* repr() { return "___"; };
};

struct __1 {
  static const char* repr() { return "__1"; };
};

/**
 * Case branch used in `Match` expression.
 *
 * @tparam Condition
 * @tparam Result
 */
template <typename Condition, typename Result>
struct Case {
  static const char* repr() { return "case"; };
};

/**
 * Default branch used in `Match` and `Cond` expression.
 * @tparam Expr
 */
template <typename Expr>
struct Default {
  static const char* repr() { return "default"; };
};

/**
 * When branch used in `Match` expression.
 * @tparam Condition
 * @tparam Result
 */
template <typename Condition, typename Result>
struct When {
  static const char* repr() { return "when"; };
};

/**
 * Else branch used in `Match` and `Cond` expression.
 * @tparam Expr
 */
template <typename Expr>
struct Else {
  static const char* repr() { return "else"; };
};

/**
 * Pattern match expression.
 * @tparam AST
 * @tparam Branch1
 * @tparam Branch2
 * @tparam Branches
 */
template <typename AST, typename Branch1, typename Branch2,
          typename... Branches>
struct Match {
  static const char* repr() { return "match"; };
};

/**
 * Capture in match expression.
 * @tparam Target
 * @tparam VarName
 */
template <typename Target, typename VarName>
struct Capture {
  static const char* repr() { return "capture"; };
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
  static const char* repr() { return "cond"; };
};

template <typename E>
struct IsEmpty {
  static const char* repr() { return "empty?"; };
};

template <typename E>
struct IsNil {
  static const char* repr() { return "nil?"; };
};

template <typename S>
struct Str2List {
  static const char* repr() { return "str2list"; };
};

template <typename... Pairs>
struct Dict {
  static const char* repr() { return "dict"; };
};

}  // namespace crisp

#endif  // CRISP_COREAST_HPP
