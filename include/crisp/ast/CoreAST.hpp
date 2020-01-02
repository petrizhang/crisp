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
struct Pair {};

// TODO print ast nodes prettily
/**
 * Quote, which will prevent the interpreter's evaluation for given `AST`.
 * @tparam AST
 */
template <typename AST>
struct Quote {};

/**
 * Evaluate a quoted expression in current environment.
 * @tparam QuotedExpr
 */
template <typename QuotedExpr>
struct Eval {};

/**
 * Closure. We use this type to represent a function value.
 * @tparam Environ
 * @tparam Func
 */
template <typename Environ, typename Func>
struct Closure {};

/**
 * Block.
 * A sequence of expressions.
 * It's result is the result of it's last expression.
 * @tparam Args
 */
template <typename... Args>
struct Block {};

/**
 * Parameter list used in function definition.
 * @tparam Params
 */
template <typename... Params>
struct ParamList {};

/**
 * If-then-else expression.
 * @tparam Cond
 * @tparam Body
 * @tparam ElseBody
 */
template <typename Cond, typename Body, typename ElseBody>
struct If {};

/**
 * Define statement.
 * @tparam Args
 */
template <typename... Args>
struct Define {};

/**
 * Function call.
 * @tparam Args
 */
template <typename... Args>
struct Call {};

/**
 * Lambda literal.
 * @tparam Params
 * @tparam Body
 */
template <typename Params, typename Body>
struct Lambda {};

/**
 * List
 * @tparam Args
 */
template <typename... Args>
struct List {};

template <typename... Args>
struct Head {};

template <typename... Args>
struct Tail {};

template <typename... Args>
struct PopHead {};

template <typename... Args>
struct PopLast {};

template <typename... Args>
struct PushHead {};

template <typename... Args>
struct PushLast {};

template <typename... Args>
struct DropHead {};

template <typename... Args>
struct DropLast {};

template <typename... Args>
struct Concat {};

/**
 * Cons: construct Pair<L,R> from L and R.
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct Cons {};

/**
 * Car: get the first element of a Pair, e.g. Car<Pair<L,R>> will be L.
 * @tparam T
 */
template <typename T>
struct Car {};

/**
 * cdr: get the second element of a Pair, e.g. Cdr<Pair<L,R>> will be R.
 */
template <typename T>
struct Cdr {};

/**
 * +
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Add {};

/**
 * -
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Sub {};

/**
 * *
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Mul {};

/**
 * /
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Mod {};

/**
 * &&
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct And {};

/**
 * ||
 * @tparam L
 * @tparam R
 * @tparam Args
 */
template <typename L, typename R, typename... Args>
struct Or {};

/**
 * ==
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsEqual {};

/**
 * >
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsGreaterThan {};

/**
 * <
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsLessThan {};

/**
 * >=
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsGreaterEqual {};

/**
 * <=
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsLessEqual {};

/**
 * Placeholder used in match expression which will match any value.
 */
struct _ {};

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
struct ___ {};

struct __1 {};

/**
 * Case branch used in `Match` expression.
 *
 * @tparam Condition
 * @tparam Result
 */
template <typename Condition, typename Result>
struct Case {};

/**
 * Default branch used in `Match` and `Cond` expression.
 * @tparam Expr
 */
template <typename Expr>
struct Default {};

/**
 * When branch used in `Match` expression.
 * @tparam Condition
 * @tparam Result
 */
template <typename Condition, typename Result>
struct When {};

/**
 * Else branch used in `Match` and `Cond` expression.
 * @tparam Expr
 */
template <typename Expr>
struct Else {};

/**
 * Pattern match expression.
 * @tparam AST
 * @tparam Branch1
 * @tparam Branch2
 * @tparam Branches
 */
template <typename AST, typename Branch1, typename Branch2,
          typename... Branches>
struct Match {};

/**
 * Capture in match expression.
 * @tparam Target
 * @tparam VarName
 */
template <typename Target, typename VarName>
struct Capture {};

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
struct Cond {};

template <typename E>
struct IsEmpty {};

template <typename E>
struct IsNil {};

template <typename S>
struct Str2List {};

template <typename... Pairs>
struct Dict {};

}  // namespace crisp

#endif  // CRISP_COREAST_HPP
