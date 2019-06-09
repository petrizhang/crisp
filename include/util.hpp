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

#ifndef CRISP_UTIL_HPP
#define CRISP_UTIL_HPP
#include "ast.hpp"

namespace util {
using namespace ast;

template <typename T>
inline void output(T v) {
  std::cout << v;
}

inline void output(bool v) {
  std::cout << (v ? "true" : "false");
}

/// -------------------------------------------------------------------------------------------
/// Use this type in static_assert to trigger a compile error.
template <typename...>
struct Error {
  static const bool always_false = false;
};

/// -------------------------------------------------------------------------------------------
/// Check if given type `T` is a instance of template `C`
/// e.g. IsTemplate<Array, Array<Int<1>,Int<2>>>::value will be true
template <template <typename...> class C, typename T>
struct IsTemplateOf {
  static const bool value = false;
};

template <template <typename...> class C, typename... Args>
struct IsTemplateOf<C, C<Args...>> {
  static const bool value = true;
};

/// -------------------------------------------------------------------------------------------
/// Check if given type `T` is a instance of template `C`
/// e.g. IsTemplate<Array, Array<Int<1>,Int<2>>>::value will be true
template <typename ValueType, template <ValueType...> class C, typename T>
struct IsValueTemplateOf {
  static const bool value = false;
};

template <typename ValueType, template <ValueType...> class C, ValueType... Args>
struct IsValueTemplateOf<ValueType, C, C<Args...>> {
  static const bool value = true;
};

template <typename T>
struct IsVar : IsValueTemplateOf<char, Var, T> {};

template <typename T>
struct IsNil : std::is_same<T, Nil> {};

/// -------------------------------------------------------------------------------------------
/// Merge two argument list into one.
/// e.g. MergeArgs< Array<Int<1>>, Array<Int<2>> >::type
/// will be Array<Int<1>,Int<2>>`
template <typename...>
struct MergeArgs;

template <template <typename...> class C, typename... LeftArgs, typename... RightArgs>
struct MergeArgs<C<LeftArgs...>, C<RightArgs...>> {
  using type = C<LeftArgs..., RightArgs...>;
};

/// -------------------------------------------------------------------------------------------
/// Merge the argument list of a template.
/// e.g. ReverseArgs< Array<Int<1>,Int<2>> >::type
/// will be Array<Int<2>,Int<1>>`
template <typename T>
struct ReverseArgs;

template <template <typename...> class C>
struct ReverseArgs<C<>> {
  using type = C<>;
};

template <template <typename...> class C, typename Head, typename... Tail>
struct ReverseArgs<C<Head, Tail...>> {
  using type = typename MergeArgs<typename ReverseArgs<C<Tail...>>::type,
                                  C<Head>>::type;
};

/// -------------------------------------------------------------------------------------------
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

/// -------------------------------------------------------------------------------------------
/// Check if an expression is callable or not.
template <typename... Args>
struct IsCallable {
  static const bool value = false;
};

template <typename... Args>
struct IsCallable<Closure<Args...>> {
  static const bool value = true;
};

/// -------------------------------------------------------------------------------------------
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

/// -------------------------------------------------------------------------------------------
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

/// -------------------------------------------------------------------------------------------
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

/// -------------------------------------------------------------------------------------------
/// A template receives a type T and return it unchanged.
template <typename T>
struct Id {
  using type = T;
};

/// -------------------------------------------------------------------------------------------
/// A template receives a type T and return it unchanged.
template <typename...>
struct NilF {
  using type = Nil;
};

/// -------------------------------------------------------------------------------------------
/// Save the context of a template `C` and it's arguments `Args`
/// and instantiate it later.
template <template <typename...> class C, typename... Args>
struct DeferApply {
  template <typename...>
  struct apply {
    using type = typename C<Args...>::type;
  };
};

/// -------------------------------------------------------------------------------------------
/// Save the context of a template `C` and it's arguments `Args`
/// and instantiate it later.
template <template <typename...> class C, typename... Args>
struct DeferConstruct {
  template <typename...>
  struct apply {
    using type = C<Args...>;
  };
};

/// -------------------------------------------------------------------------------------------
/// Check if given type `T` is an instantiation of `LazyApply`
template <typename T>
struct IsDeferApply {
  static const bool value = false;
};

template <template <typename...> class C, typename... Args>
struct IsDeferApply<DeferApply<C, Args...>> {
  static const bool value = true;
};

/// -------------------------------------------------------------------------------------------
/**
 * Get result from branches according to branches' condition.
 * It is very similar to switch statement in C++.
 * e.g.
 * @code
 * ConditionalImpl<When<Bool<true>, Int<1>>,
 *                 When<Bool<false>, Int<1>>,
 *                 Else<Int<2>> >;
 * @endcode
 *
 */
template <typename Branch1, typename Branch2, typename... Branches>
struct ConditionalImpl;

template <typename Body, typename ElseBody>
struct ConditionalImpl<When<Bool<true>, Body>, Else<ElseBody>> {
  using type = Body;
};

template <typename Body, typename ElseBody>
struct ConditionalImpl<When<Bool<false>, Body>, Else<ElseBody>> {
  using type = ElseBody;
};

template <typename Branch, typename ElseBranch>
struct ConditionalImpl<Branch, ElseBranch> {
  static_assert(Error<Branch>::always_false,
                "expected a valid `When` instantiation.");
  static_assert(Error<ElseBranch>::always_false,
                "expected a valid `Else` instantiation.");
};

template <typename Body1, typename Branch2, typename Branch3, typename... Tail>
struct ConditionalImpl<When<Bool<true>, Body1>, Branch2, Branch3, Tail...> {
  using type = Body1;
};

template <typename Body1, typename Branch2, typename Branch3, typename... Tail>
struct ConditionalImpl<When<Bool<false>, Body1>, Branch2, Branch3, Tail...> {
  using type = typename ConditionalImpl<Branch2, Branch3, Tail...>::type;
};

template <typename Branch1, typename Branch2, typename Branch3, typename... Tail>
struct ConditionalImpl<Branch1, Branch2, Branch3, Tail...> {
  static_assert(Error<Branch1>::always_false,
                "expected a valid `When` instantiation.");
};

/// -------------------------------------------------------------------------------------------
/**
 * As same as @see ConditionalImpl,
 * except that we will expand the template context saved in `LazyApply`.
 * e.g.
 * @code
 * ConditionalApply<When<Bool<true>, LazyApply<Array, Int<1>> >,
 *                  When<Bool<false>, LazyApply<Array, Int<2>> >,
 *                  Else<LazyApply<Array, Int<3>>> >;
 * @endcode
 *
 */
template <typename Branch1, typename Branch2, typename... Branches>
struct ConditionalApply {
  using Result = typename ConditionalImpl<Branch1, Branch2, Branches...>::type;
  using type = typename Result::template apply<>::type;
};

/// -------------------------------------------------------------------------------------------
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

/// -------------------------------------------------------------------------------------------
/// Bind a variable name `K` with a value `V` in current scope
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

/// -------------------------------------------------------------------------------------------
/// Lookup a variable by name `K` in current environment
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

/// -------------------------------------------------------------------------------------------
/// Implementation for `Car`
template <typename T>
struct CarImpl;

template <typename L, typename R>
struct CarImpl<Pair<L, R>> {
  using type = L;
};

/// -------------------------------------------------------------------------------------------
/// Implementation for `Cdr`
template <typename T>
struct CdrImpl;

template <typename L, typename R>
struct CdrImpl<Pair<L, R>> {
  using type = R;
};

/// -------------------------------------------------------------------------------------------
/// Implementation for `List`
template <typename T, typename... Args>
struct ListImpl {
  using type = Pair<T, typename List<Args...>::type>;
};

template <typename T>
struct ListImpl<T> {
  using type = Pair<T, Nil>;
};

/// -------------------------------------------------------------------------------------------
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

/// -------------------------------------------------------------------------------------------
/// Implementation for `IsEqual`
/// If two types are the same, then the values they represent are the same.
template <typename L, typename R>
struct IsEqualImpl {
  using type = Bool<std::is_same<L, R>::value>;
};
}  // namespace util

#endif  //CRISP_UTIL_HPP
