#ifndef TISPP_HPP
#define TISPP_HPP

namespace tispp {

#include "type_traits"

namespace ast {
/// -----------------------------------------------------
/// Boolean value type.
template <bool V>
struct Bool {
  typedef bool value_type;
  static const bool value = V;
  static constexpr const char *type_name = "boll";
};

/// -----------------------------------------------------
/// Char value type.
template <char V>
struct Char {
  typedef char value_type;
  static const char value = V;
  static constexpr const char *type_name = "char";
};

/// -----------------------------------------------------
/// Int value type.
template <int V>
struct Int {
  typedef int value_type;
  static const int value = V;
  static constexpr const char *type_name = "int";
};

/// -----------------------------------------------------
/// Pair(tuple2) value type.
template <typename L, typename R>
struct Pair {
  typedef L first;
  typedef R second;
};

/// -----------------------------------------------------
/// Nil type. It likes the `void` type in C++.
struct Nil {
  typedef void value_type;
  static constexpr const char *type_name = "Nil";
  static constexpr const char *value = "nil";
};

/// -----------------------------------------------------
/// List(x,x,x,...)
/// e.g. List<Int<1>, Int<2>> will be Pair<Int<1>,Pair<Int<2>, Nil>>.
template <typename T, typename... Args>
struct List {};

/// -----------------------------------------------------
/// cons: construct Pair<L,R> from L and R.
template <typename L, typename R>
struct Cons {};

/// -----------------------------------------------------
/// car: get the first element of a Pair, e.g. Car<Pair<L,R>> will be L.
template <typename T>
struct Car {};

/// -----------------------------------------------------
/// cdr: get the second element of a Pair, e.g. Cdr<Pair<L,R>> will be R.
template <typename T>
struct Cdr {};

/// -----------------------------------------------------
/// +
template <typename... Args>
struct Add {};

/// -----------------------------------------------------
/// -
template <typename... Args>
struct Sub {};

/// -----------------------------------------------------
/// *
template <typename... Args>
struct Mul {};

/// -----------------------------------------------------
/// /
template <typename... Args>
struct Mod {};

/// -----------------------------------------------------
/// &&
template <typename... Args>
struct And {};

/// -----------------------------------------------------
/// ||
template <typename... Args>
struct Or {};

/// -----------------------------------------------------
/// ==
template <typename... Args>
struct IsEqual {};

/// -----------------------------------------------------
/// >
template <typename L, typename R>
struct IsGreaterThan {};

/// -----------------------------------------------------
/// <
template <typename L, typename R>
struct IsLessThan {};

/// -----------------------------------------------------
/// >=
template <typename L, typename R>
struct IsGreaterEqual {};

/// -----------------------------------------------------
/// <=
template <typename L, typename R>
struct IsLessEqual {};

}  // namespace ast
using namespace ast;

namespace utils {

/// -----------------------------------------------------
/// Pack a const value(int/char/bool) into Value type.
/// e.g. PackToValue<int, 1>::packed_type will be Value<Int<1>>.
template <typename T, T V>
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

/// -----------------------------------------------------
/// Implementation for `Car`
template <typename T>
struct CarImpl;

template <typename L, typename R>
struct CarImpl<Pair<L, R>> {
  typedef L type;
};

/// -----------------------------------------------------
/// Implementation for `Cdr`
template <typename T>
struct CdrImpl;

template <typename L, typename R>
struct CdrImpl<Pair<L, R>> {
  typedef R type;
};

/// -----------------------------------------------------
/// Implementation for `List`
template <typename T, typename... Args>
struct ListImpl {
  typedef Pair<T, typename List<Args...>::type> type;
};

template <typename T>
struct ListImpl<T> {
  typedef Pair<T, Nil> type;
};

/// -----------------------------------------------------
/// Implementation for `Add`
template <typename L, typename R>
struct AddImpl {
  typedef decltype(L::value + R::value) value_type;
  static constexpr const value_type value = L::value + R::value;
  typedef typename PackToType<value_type, value>::type type;
};

/// -----------------------------------------------------
/// Most binary operators (-,*,%,...) are of the same form as `AddImpl`,
/// thus we could implement them with an unified macro.

#define ImplementOperator(OpName, Operator)                               \
  template <typename L, typename R>                                       \
  struct OpName##Impl {                                                   \
    typedef decltype(L::value Operator R::value) value_type;              \
    static constexpr const value_type value = L::value Operator R::value; \
    typedef typename PackToType<value_type, value>::type type;            \
  };

ImplementOperator(Sub, -);
ImplementOperator(Mul, *);
ImplementOperator(Mod, %);
ImplementOperator(And, &&);
ImplementOperator(Or, ||);
ImplementOperator(IsEqual, ==);
ImplementOperator(IsGreaterThan, >);
ImplementOperator(IsLessThan, <);
ImplementOperator(IsGreaterEqual, >=);
ImplementOperator(IsLessEqual, <=);

}  // namespace utils
using namespace utils;

namespace builtin {}  // namespace builtin
using namespace builtin;

namespace interpreter {

template <typename T>
struct Eval {
  typedef T type;
};

}  // namespace interpreter
using namespace interpreter;

namespace api {

#define v(x) PackToType<decltype(x), x>::type

}  // namespace api
using namespace api;

}  // namespace tispp
#endif  // TISPP_HPP
