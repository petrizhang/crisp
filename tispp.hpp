//
// Created by pengcheng on 5/7/19.
//

#ifndef TISPP_HPP
#define TISPP_HPP

namespace tispp {

#include "type_traits"

namespace utils {
/**
 * Fetch return type of a function.
 * e.g. ReturnType<int(int)> = int;
 * @tparam F
 * @tparam Args
 */
template <class F, class... Args>
struct ReturnType;

template <class F, class... Args>
struct ReturnType<F(Args...)> {
  typedef F type;
};

/**
 * Fetch type of the first parameter of a function.
 * e.g. ReturnType<int(int)> = int;
 * @tparam F
 * @tparam Args
 */
template <class F>
struct ParamType;

template <class F, class Arg>
struct ParamType<F(Arg)> {
  typedef Arg type;
};
}  // namespace utils

using namespace utils;

namespace ast {

struct Value {};

struct Function {};

/**
 * Boolean type
 * @tparam V
 */
template <bool V>
struct Bool : Value {
  typedef bool value_type;
  static const bool value = V;
  static constexpr const char *type_name = "boll";
  typedef Bool<V> type;
};

/**
 * Char type
 * @tparam V
 */
template <char V>
struct Char : Value {
  typedef char value_type;
  static const char value = V;
  static constexpr const char *type_name = "char";
  typedef Char<V> type;
};

/**
 * Int type which wraps a integer value.
 * @tparam V
 */
template <int V>
struct Int : Value {
  typedef int value_type;
  static const int value = V;
  static constexpr const char *type_name = "int";
  typedef Int<V> type;
};

/**
 * Pair type (tuple).
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct Pair : Value {
  typedef L first;
  typedef R second;
  typedef Pair<L, R> type;
};

/**
 * Nil type. Cons<x, nil> = list<x>
 */
struct Nil : Value {
  typedef void value_type;
  static constexpr const char *type_name = "Nil";
  static constexpr const char *value = "nil";
};

static const Nil nil = Nil();

template <typename T, typename... Args>
struct List : Function {
  typedef Pair<T, typename List<Args...>::type> type;
};

template <typename T>
struct List<T> {
  typedef Pair<T, Nil> type;
};

struct CarNode {};

struct PlusNode {};

struct MinusNode {};

struct EqualNode {};

}  // namespace ast

using namespace ast;

namespace builtin {

/**
 *
 * @tparam T
 */
template <typename T>
struct CarImpl;

template <typename L, typename R>
struct CarImpl<Pair<L, R>> {
  typedef L type;
};

/**
 *
 * @tparam T
 */
template <typename T>
struct CdrImpl;

template <typename L, typename R>
struct CdrImpl<Pair<L, R>> {
  typedef R type;
};
}  // namespace builtin

using namespace builtin;

namespace api {

/**
 * Pack a const value(int/char/bool) into Value type.
 * e.g. PackToValue<int, 1>::packed_type will be Value<Int<1>>
 * @tparam T
 * @tparam V
 */
template <class T, T V>
struct PackToType;

template <bool V>
struct PackToType<bool, V> {
  typedef Bool<V> type;
};

template <char V>
struct PackToType<char, V> {
  typedef Char<V> type;
};

template <int V>
struct PackToType<int, V> {
  typedef Int<V> type;
};

/// Macro that wraps a literal value to it's represent type. e.g. v(1) => Int<1>
#define v(x) PackToType<decltype(x), x>::type

/// Macro that applies `Cdr` on type l
#define cdr(l) CdrImpl<l>::type

/// Macro that applies `Car` on type l
#define car(l) CarImpl<l>::type

///
#define list(args...) typename List<args>::type
/**
 *
 * @tparam T
 */
template <class T>
struct Eval {
  typedef class T::apply::type type;
};

}  // namespace api

using namespace api;
}  // namespace tispp

#endif  // TISPP_HPP
