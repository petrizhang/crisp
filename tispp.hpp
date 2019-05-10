#ifndef TISPP_HPP
#define TISPP_HPP

namespace tispp {

#include <cassert>
#include <type_traits>

namespace ast {
/// ----------------------------------------------------------------------------
/// Boolean value type.
template <bool V>
struct Bool {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *type_name = "bool";
  typedef bool c_type;
  static constexpr c_type c_value() { return V; };
};

/// ----------------------------------------------------------------------------
/// Char value type.
template <char V>
struct Char {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *type_name = "char";
  typedef char c_type;
  static constexpr c_type c_value() { return V; };
};

/// ----------------------------------------------------------------------------
/// Int value type.
template <int V>
struct Int {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *type_name = "int";
  typedef int c_type;
  static constexpr c_type c_value() { return V; };
};

/// ----------------------------------------------------------------------------
/// Symbol value type.
template <char... args>
struct Symbol;

template <char c>
struct Symbol<c> {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *type_name = "symbol";
  typedef std::string c_type;
  static const c_type c_value() { return std::string(1, c); }
};

template <char c, char... args>
struct Symbol<c, args...> {
  /// These members and methods are used for interacting with c++ at runtime.
  static constexpr const char *type_name = "symbol";
  typedef std::string c_type;
  static const c_type c_value() {
    return std::string(1, c) + Symbol<args...>::c_value();
  }
};

/// ----------------------------------------------------------------------------
/// Function parameter placeholder.
template <char... args>
struct Id : Symbol<args...> {};

/// ----------------------------------------------------------------------------
/// Pair(tuple2) value type.
template <typename L, typename R>
struct Pair {};

/// ----------------------------------------------------------------------------
/// Nil type. It likes the `void` type in C++.
struct Nil {
  typedef void c_type;
  static constexpr const char *type_name = "Nil";
  static constexpr const char *value = "nil";
};

/// ----------------------------------------------------------------------------
/// List(x,x,x,...)
/// e.g. List<Int<1>, Int<2>> will be Pair<Int<1>,Pair<Int<2>, Nil>>.
template <typename T, typename... Args>
struct List {};

/// ----------------------------------------------------------------------------
/// cons: construct Pair<L,R> from L and R.
template <typename L, typename R>
struct Cons {};

/// ----------------------------------------------------------------------------
/// car: get the first element of a Pair, e.g. Car<Pair<L,R>> will be L.
template <typename T>
struct Car {};

/// ----------------------------------------------------------------------------
/// cdr: get the second element of a Pair, e.g. Cdr<Pair<L,R>> will be R.
template <typename T>
struct Cdr {};

/// ----------------------------------------------------------------------------
/// +
template <typename... Args>
struct Add {};

/// ----------------------------------------------------------------------------
/// -
template <typename... Args>
struct Sub {};

/// ----------------------------------------------------------------------------
/// *
template <typename... Args>
struct Mul {};

/// ----------------------------------------------------------------------------
/// /
template <typename... Args>
struct Mod {};

/// ----------------------------------------------------------------------------
/// &&
template <typename... Args>
struct And {};

/// ----------------------------------------------------------------------------
/// ||
template <typename... Args>
struct Or {};

/// ----------------------------------------------------------------------------
/// ==
template <typename L, typename R>
struct IsEqual {};

/// ----------------------------------------------------------------------------
/// >
template <typename L, typename R>
struct IsGreaterThan {};

/// ----------------------------------------------------------------------------
/// <
template <typename L, typename R>
struct IsLessThan {};

/// ----------------------------------------------------------------------------
/// >=
template <typename L, typename R>
struct IsGreaterEqual {};

/// ----------------------------------------------------------------------------
/// <=
template <typename L, typename R>
struct IsLessEqual {};

}  // namespace ast
using namespace ast;

namespace utils {

/// ----------------------------------------------------------------------------
/// Use this type in static_assert to trigger a compiling error.
template <typename...>
struct type_checker {
  static const bool value = false;
};

/// ----------------------------------------------------------------------------
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

/// ----------------------------------------------------------------------------
/// Implementation for `Car`
template <typename T>
struct CarImpl;

template <typename L, typename R>
struct CarImpl<Pair<L, R>> {
  typedef L type;
};

/// ----------------------------------------------------------------------------
/// Implementation for `Cdr`
template <typename T>
struct CdrImpl;

template <typename L, typename R>
struct CdrImpl<Pair<L, R>> {
  typedef R type;
};

/// ----------------------------------------------------------------------------
/// Implementation for `List`
template <typename T, typename... Args>
struct ListImpl {
  typedef Pair<T, typename List<Args...>::type> type;
};

template <typename T>
struct ListImpl<T> {
  typedef Pair<T, Nil> type;
};

/// ----------------------------------------------------------------------------
/// Implementation for `Add`
template <typename L, typename R>
struct AddImpl {
  static_assert(type_checker<L, R>::value,
                "Incompatible types for operation `Add`.");
};

template <int LV, int RV>
struct AddImpl<Int<LV>, Int<RV>> {
  typedef Int<LV + RV> type;
};

/// Most binary operators (-,*,%,...) follow the same pattern as
/// `AddImpl`, thus we could implement them with an unified macro.
#define BinaryOperator(OpName, Operator, LeftValueType, LeftType,     \
                       RightValueType, RightType, ResultType)         \
  template <typename L, typename R>                                   \
  struct OpName##Impl {                                               \
    static_assert(type_checker<L, R>::value,                          \
                  "Incompatible types for operation `" #OpName "`."); \
  };                                                                  \
                                                                      \
  template <LeftValueType LV, RightValueType RV>                      \
  struct OpName##Impl<LeftType<LV>, RightType<RV>> {                  \
    typedef ResultType<(LV Operator RV)> type;                        \
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
  typedef Bool<std::is_same<L, R>::value> type;
};

}  // namespace utils
using namespace utils;

namespace builtin {}  // namespace builtin
using namespace builtin;

namespace interpreter {

/// ----------------------------------------------------------------------------
/// Interpreter implementation
template <typename T>
struct Eval : T {
  typedef T type;
};

/// ----------------------------------------------------------------------------
/// Eval Add<n1,n2,n3,...>
template <typename T>
struct Eval<Add<T>> {
  typedef T type;
};

template <typename L, typename R>
struct Eval<Add<L, R>> {
  typedef typename AddImpl<typename Eval<L>::type, typename Eval<R>::type>::type
      type;
};

template <typename L, typename R, typename... Args>
struct Eval<Add<L, R, Args...>> {
  typedef
      typename AddImpl<typename Eval<L>::type, typename Eval<R>::type>::type LT;
  typedef typename Eval<Add<Args...>>::type RT;
  typedef typename AddImpl<LT, RT>::type type;
};

/// ----------------------------------------------------------------------------
/// Eval chain operator like Add<n1,n2,n3,...>, Sub<n1,n2,n3,...>, ...
#define EvalForChainOperator(OpName)                                  \
  template <typename T>                                               \
  struct Eval<OpName<T>> {                                            \
    typedef T type;                                                   \
  };                                                                  \
                                                                      \
  template <typename L, typename R>                                   \
  struct Eval<OpName<L, R>> {                                         \
    typedef typename OpName##Impl<typename Eval<L>::type,             \
                                  typename Eval<R>::type>::type type; \
  };                                                                  \
                                                                      \
  template <typename L, typename R, typename... Args>                 \
  struct Eval<OpName<L, R, Args...>> {                                \
    typedef typename OpName##Impl<typename Eval<L>::type,             \
                                  typename Eval<R>::type>::type LT;   \
    typedef typename Eval<OpName<Args...>>::type RT;                  \
    typedef typename OpName##Impl<LT, RT>::type type;                 \
  };

EvalForChainOperator(Sub);
EvalForChainOperator(Mul);
EvalForChainOperator(Mod);
EvalForChainOperator(And);
EvalForChainOperator(Or);

/// ----------------------------------------------------------------------------
/// Eval IsEqual<L,R>
template <typename L, typename R>
struct Eval<IsEqual<L, R>> {
  typedef
      typename IsEqualImpl<typename Eval<L>::type, typename Eval<R>::type>::type
          type;
};

#define EvalForBinaryOperator(OpName)                                 \
  template <typename L, typename R>                                   \
  struct Eval<OpName<L, R>> {                                         \
    typedef typename OpName##Impl<typename Eval<L>::type,             \
                                  typename Eval<R>::type>::type type; \
  };

EvalForBinaryOperator(IsGreaterThan);
EvalForBinaryOperator(IsLessThan);
EvalForBinaryOperator(IsGreaterEqual);
EvalForBinaryOperator(IsLessEqual);

}  // namespace interpreter
using namespace interpreter;

namespace api {

#define v(x) PackToType<decltype(x), x>::type
#define add(args...) Add<args>
#define sub(args...) Sub<args>
#define mul(args...) Mul<args>
#define mod(args...) Mod<args>
#define eq(args...) IsEqual<args>
#define gt(args...) IsGreaterThan<args>
#define lt(args...) IsLessThan<args>
#define ge(args...) IsGreatEqual<args>
#define le(args...) IsLessEqual<args>
#define id(args...) Id<args>
#define _or(args...) Or<args>
#define _and(args...) And<args>
#define define(args...) Define<args>
#define _if(args...) If<args>
#define params(args...) ParamList<args>
#define lambda(args...) Lambda<args>
#define call(f, args...) Call<f, args>
#define eval(s) Eval<s>

}  // namespace api
using namespace api;

}  // namespace tispp
#endif  // TISPP_HPP
