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
  static const c_type c_value() {
    return std::string(1, c) + Symbol<args...>::c_value();
  }
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
  static constexpr const char *c_value() {
    return "nil";
  };
};

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
template <typename... Args>
struct Add {
  static constexpr const char *repr = "+";
};

/// ----------------------------------------------------------------------------
/// -
template <typename... Args>
struct Sub {
  static constexpr const char *repr = "-";
};

/// ----------------------------------------------------------------------------
/// *
template <typename... Args>
struct Mul {
  static constexpr const char *repr = "*";
};

/// ----------------------------------------------------------------------------
/// /
template <typename... Args>
struct Mod {
  static constexpr const char *repr = "/";
};

/// ----------------------------------------------------------------------------
/// &&
template <typename... Args>
struct And {
  static constexpr const char *repr = "and";
};

/// ----------------------------------------------------------------------------
/// ||
template <typename... Args>
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
  static_assert(type_checker<L, R>::value,
                "Incompatible types for operation `Add`.");
};

template <int LV, int RV>
struct AddImpl<Int<LV>, Int<RV>> {
  using type = Int<LV + RV>;
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
    using type = ResultType<(LV Operator RV)>;                        \
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
/// A Dictionary (map-like) type used as a symbol table.
template <typename L = Nil, typename R = Nil>
using DictImpl = Pair<L, R>;

template <typename dict, typename K, typename V>
struct DictPutImpl {
  using type = DictImpl<Pair<K, V>, dict>;
};

template <typename K, typename V>
struct DictPutImpl<DictImpl<Nil, Nil>, K, V> {
  using type = DictImpl<Pair<K, V>, Nil>;
};

template <typename P, typename K, typename V>
struct DictPutImpl<DictImpl<P, Nil>, K, V> {
  using type = DictImpl<Pair<K, V>, DictImpl<P, Nil>>;
};

template <typename dict, typename K>
struct DictGetImpl;

template <typename K>
struct DictGetImpl<DictImpl<Nil, Nil>, K> {
  static_assert(type_checker<K>::value, "Cannot find variable.");
};

template <typename K, typename V>
struct DictGetImpl<DictImpl<Pair<K, V>, Nil>, K> {
  using type = V;
};

template <typename K, typename V, typename Tail>
struct DictGetImpl<DictImpl<Pair<K, V>, Tail>, K> {
  using type = V;
};

template <typename T, typename K, typename V, typename Tail>
struct DictGetImpl<DictImpl<Pair<T, V>, Tail>, K> {
  using type = typename DictGetImpl<Tail, K>::type;
};
}  // namespace utils
using namespace utils;

namespace interpreter {
/// ----------------------------------------------------------------------------
/// Interpreter implementation
template <typename T>
struct Eval : T {
  using type = T;
};

/// ----------------------------------------------------------------------------
/// Eval Add<n1,n2,n3,...>
template <typename T>
struct Eval<Add<T>> {
  using type = T;
};

template <typename L, typename R>
struct Eval<Add<L, R>> {
  typedef typename AddImpl<typename Eval<L>::type, typename Eval<R>::type>::type
      type;
};

template <typename L, typename R, typename... Args>
struct Eval<Add<L, R, Args...>> {
  using LT =
      typename AddImpl<typename Eval<L>::type, typename Eval<R>::type>::type;
  using RT = typename Eval<Add<Args...>>::type;
  using type = typename AddImpl<LT, RT>::type;
};

/// ----------------------------------------------------------------------------
/// Eval chain operator like Add<n1,n2,n3,...>, Sub<n1,n2,n3,...>, ...
#define EvalForChainOperator(OpName)                                  \
  template <typename T>                                               \
  struct Eval<OpName<T>> {                                            \
    using type = T;                                                   \
  };                                                                  \
                                                                      \
  template <typename L, typename R>                                   \
  struct Eval<OpName<L, R>> {                                         \
    using type = typename OpName##Impl<typename Eval<L>::type,        \
                                       typename Eval<R>::type>::type; \
  };                                                                  \
                                                                      \
  template <typename L, typename R, typename... Args>                 \
  struct Eval<OpName<L, R, Args...>> {                                \
    using LT = typename OpName##Impl<typename Eval<L>::type,          \
                                     typename Eval<R>::type>::type;   \
    using RT = typename Eval<OpName<Args...>>::type;                  \
    using type = typename OpName##Impl<LT, RT>::type;                 \
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
  using type = typename IsEqualImpl<typename Eval<L>::type,
                                    typename Eval<R>::type>::type;
};

#define EvalForBinaryOperator(OpName)                                 \
  template <typename L, typename R>                                   \
  struct Eval<OpName<L, R>> {                                         \
    using type = typename OpName##Impl<typename Eval<L>::type,        \
                                       typename Eval<R>::type>::type; \
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
#define var(args...) Var<args>
#define _or(args...) Or<args>
#define _and(args...) And<args>
#define define(args...) Define<args>
#define _if(args...) If<args>
#define params(args...) ParamList<args>
#define lambda(args...) Lambda<args>
#define call(f, args...) Call<f, args>
#define seq(args...) Seq<args>
#define eval(s) Eval<s>
}  // namespace api
using namespace api;

}  // namespace tispp
#endif  // TISPP_HPP
