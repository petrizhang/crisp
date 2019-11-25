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
#include <iostream>
#include "Error.hpp"
#include "List.hpp"
#include "OperatorImpl.hpp"
#include "Pack.hpp"
#include "Size.hpp"
#include "TemplateUtil.hpp"
#include "Zip.hpp"
#include "interpreter/AST.hpp"

namespace util {
using namespace ast;

template <typename T>
inline void output(T v) {
  std::cout << v;
}

inline void output(bool v) {
  std::cout << (v ? "true" : "false");
}

template <typename T>
struct IsVar : IsValueTemplateOf<char, Var, T> {};

template <typename T>
struct IsNil : std::is_same<T, Nil> {};

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
/// A map-like collection type.
template <typename... Pairs>
using Dict = Vector<Pairs...>;

template <typename dict, typename pair>
using DictPut = VectorPushFront<dict, pair>;

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
using Env = Vector<Dicts...>;

template <typename Environ, typename Extra>
using EnvExtendBack = VectorExtendBack<Environ, Extra>;

template <typename env, typename dict>
using EnvPushFront = VectorPushFront<env, dict>;

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
/// Recursively replace types in `T` according to `ReplaceDict`.
/// e.g.: Replace< T<int,bool>, Dict<Pair<bool, char>>::type will be T<int,char>
// TODO: test Replace template carefully

namespace internal {
template <typename... Args>
struct ReplaceList {};

template <typename E, typename L>
struct PrependReplaceList;

template <typename E, typename... Args>
struct PrependReplaceList<E, ReplaceList<Args...>> {
  using type = ReplaceList<E, Args...>;
};
}  // namespace internal

template <typename T, typename ReplaceDict>
struct Replace {
  using target = typename DictGet<ReplaceDict, T>::type;
  using type = typename std::conditional<IsNil<target>::value,
                                         T,
                                         target>::type;
};

template <typename ReplaceDict>
struct Replace<internal::ReplaceList<>, ReplaceDict> {
  using type = internal::ReplaceList<>;
};

template <typename Head, typename... Tail, typename ReplaceDict>
struct Replace<internal::ReplaceList<Head, Tail...>, ReplaceDict> {
  using HeadReplace = typename Replace<Head, ReplaceDict>::type;
  using TailReplaceList = typename Replace<internal::ReplaceList<Tail...>, ReplaceDict>::type;
  using type = typename internal::PrependReplaceList<HeadReplace, TailReplaceList>::type;
};

template <template <typename...> class T, typename... Args, typename ReplaceDict>
struct Replace<T<Args...>, ReplaceDict> {
  using ReplaceListResult = typename Replace<internal::ReplaceList<Args...>, ReplaceDict>::type;
  using type = typename Convert<ReplaceListResult, T>::type;
};

/// -------------------------------------------------------------------------------------------
/// Get `Condition` in `Case<Condition,Result>`
template <typename>
struct GetCaseCondition;

template <typename Condition, typename Result>
struct GetCaseCondition<Case<Condition, Result>> {
  using type = Condition;
};

/// -------------------------------------------------------------------------------------------
/// Get `Result` in `Case<Condition,Result>`
template <typename>
struct GetCaseResult;

template <typename Condition, typename Result>
struct GetCaseResult<Case<Condition, Result>> {
  using type = Result;
};

/// -------------------------------------------------------------------------------------------
/// Get `Result` in `Default<Result>`
template <typename>
struct GetDefaultResult;

template <typename Result>
struct GetDefaultResult<Default<Result>> {
  using type = Result;
};

/// -------------------------------------------------------------------------------------------
/// Get `AST` from `Quote<AST>`
template <typename>
struct GetQuoteAST;

template <typename AST>
struct GetQuoteAST<Quote<AST>> {
  using type = AST;
};

/// This namespace is used for internal evaluation.
/// Never use it in user code!!!
namespace internal {
template <typename... Args>
struct MatchList {};
}  // namespace internal

/// -------------------------------------------------------------------------------------------
/// Get template arguments into internal::MatchList
/// e.g. C<Args...> => internal::MatchList<Args...>
template <typename T>
struct GetArgsToMatchList {
  using type = internal::MatchList<>;
};

template <template <typename...> class C, typename... Args>
struct GetArgsToMatchList<C<Args...>> {
  using type = internal::MatchList<Args...>;
};

/// -------------------------------------------------------------------------------------------
/// Get the `Target` type in `Capture<Target,VarName>`
template <typename T>
struct GetCaptureTarget {
  using type = Undefined;
};

template <typename Target, typename VarName>
struct GetCaptureTarget<Capture<Target, VarName>> {
  using type = Target;
};

/// -------------------------------------------------------------------------------------------
/// Get the `VarName` type in `Capture<Target,VarName>`
template <typename T>
struct GetCaptureVarName {
  using type = Undefined;
};

template <typename Target, typename VarName>
struct GetCaptureVarName<Capture<Target, VarName>> {
  using type = VarName;
};

/// -------------------------------------------------------------------------------------------
/// Test if the given type `T` is `Capture<...>`
template <typename T>
struct IsCapture : IsTemplateOf<Capture, T> {};

/// -------------------------------------------------------------------------------------------
/// Test if the given type `T` is Capture<_,...> or Capture<___,...>
template <typename T>
struct IsCaptureAny {
  static const bool value = false;
};

template <typename VarName>
struct IsCaptureAny<Capture<_, VarName>> {
  static const bool value = true;
};

template <typename VarName>
struct IsCaptureAny<Capture<___, VarName>> {
  static const bool value = true;
};

/// -------------------------------------------------------------------------------------------
/// Test if the given type `T` is Capture<_,...>
template <typename T>
struct IsCaptureAnySingle {
  static const bool value = false;
};

template <typename VarName>
struct IsCaptureAnySingle<Capture<_, VarName>> {
  static const bool value = true;
};

template <typename VarName>
struct IsCaptureAnySingle<Capture<___, VarName>> {
  static const bool value = false;
};

/// -------------------------------------------------------------------------------------------
/// Test if the given type `T` is Capture<___,...>
template <typename T>
struct IsCaptureAnyList {
  static const bool value = false;
};

template <typename VarName>
struct IsCaptureAnyList<Capture<_, VarName>> {
  static const bool value = false;
};

template <typename VarName>
struct IsCaptureAnyList<Capture<___, VarName>> {
  static const bool value = true;
};

/// -------------------------------------------------------------------------------------------
/// Test if the given type `T` is `Capture<A,...>` where `A` is not `_` or `___`
template <typename T>
struct IsCaptureSpecific {
  static const bool value = false;
};

template <typename Target, typename VarName>
struct IsCaptureSpecific<Capture<Target, VarName>> {
  static const bool value = true;
};

template <typename VarName>
struct IsCaptureSpecific<Capture<_, VarName>> {
  static const bool value = false;
};

template <typename VarName>
struct IsCaptureSpecific<Capture<___, VarName>> {
  static const bool value = false;
};

/// -------------------------------------------------------------------------------------------
/// Helper template used in match implementation.
/// It saves a `Environ` and provides `matched` as false
template <typename Environ>
struct MatchFailure {
  static const bool matched = false;
  using env = Environ;
};

/// -------------------------------------------------------------------------------------------
/// Helper template used in match implementation
/// It saves a `Environ` and provides `matched` as true
template <typename Environ>
struct MatchSuccess {
  static const bool matched = true;
  using env = Environ;
};

/// -------------------------------------------------------------------------------------------
/// QuoteMatchCase implementation
template <typename Environ,
          typename SourceMatchList,
          typename TargetMatchList>
struct QuoteMatchInternal {
  static const bool matched = false;
  using env = Environ;
};

template <typename Environ, typename Source, typename Target>
struct QuoteMatchCase {
  using _TargetIsCapture = Bool<IsCapture<Target>::value>;

  using _SourceIsTemplate = Bool<IsTemplate<Source>::value>;
  using _TargetIsTemplate = Bool<IsTemplate<Target>::value>;
  using _IsSameTemplateAndNotTargetIsCapture = Bool<!IsCapture<Target>::value &&
                                                    IsSameTemplate<Source, Target>::value>;

  using _IsTargetMatchAny = Bool<std::is_same<Target, _>::value ||
                                 std::is_same<Target, ___>::value>;

  using SomeVarName = typename ConditionalApply<
      When<_TargetIsCapture,
           DeferApply<GetCaptureVarName, Target>>,
      Else<DeferApply<NilF>>>::type;

  using SomeCaptureTarget = typename ConditionalApply<
      When<_TargetIsCapture,
           DeferApply<GetCaptureTarget, Target>>,
      Else<DeferApply<NilF>>>::type;

  using SomeSoureMatchList = typename ConditionalApply<
      When<_SourceIsTemplate,
           DeferApply<GetArgsToMatchList, Source>>,
      Else<DeferApply<NilF>>>::type;

  using SomeTargetMatchList = typename ConditionalApply<
      When<_TargetIsTemplate,
           DeferApply<GetArgsToMatchList, Target>>,
      Else<DeferApply<NilF>>>::type;

  /*
   * 1. A =?= _/___
   * 2. ? =?= Capture<T,V>
   * 2. A<Args...> =?= A<Args...>

   * ---------------------------
   * 5. A<Args...> =?= B<Args...>
   * 6. A =?= B
   */

  // 2. ? =?= Capture<T,V>
  using CaptureMatch = typename ConditionalApply<
      When<_TargetIsCapture,
           DeferConstruct<QuoteMatchCase, Environ, Source, SomeCaptureTarget>>,
      Else<DeferConstruct<MatchFailure, Environ>>>::type;

  // 3. A<Args...> =?= A<Args...>
  using TemplateMatch = typename ConditionalApply<
      When<_IsSameTemplateAndNotTargetIsCapture,
           DeferConstruct<QuoteMatchInternal, Environ, SomeSoureMatchList, SomeTargetMatchList>>,
      Else<DeferConstruct<MatchFailure, Environ>>>::type;

  using Matched = typename ConditionalImpl<
      When<_IsTargetMatchAny, Bool<true>>,
      When<_TargetIsCapture, Bool<CaptureMatch::matched>>,
      When<_IsSameTemplateAndNotTargetIsCapture, Bool<TemplateMatch::matched>>,
      Else<Bool<false>>>::type;

  static const bool matched = Matched::value;

  using env = typename ConditionalImpl<
      When<_IsTargetMatchAny, Environ>,
      When<_TargetIsCapture, typename DictPut<typename CaptureMatch::env, Pair<SomeVarName, Source>>::type>,
      When<_IsSameTemplateAndNotTargetIsCapture, typename TemplateMatch::env>,
      Else<Environ>>::type;
};

template <typename Environ, typename Source>
struct QuoteMatchCase<Environ, Source, Source> {
  // If `Source` is Capture<_, Var<...> > or Capture<___, Var<...> >
  using _TargetIsCaptureAny = Bool<IsCaptureAny<Source>::value>;

  // When match Capture<_, Var<...> > with Capture<_, Var<...> >,
  // get `Var<...>` from given expression.
  using SomeVarName = typename ConditionalApply<
      When<_TargetIsCaptureAny,
           DeferApply<GetCaptureVarName, Source>>,
      Else<DeferApply<NilF>>>::type;

  // When match Capture<_, Var<...> > with Capture<_, Var<...> >,
  // put `Capture<_, Var<...> >` to current environment.
  // When match Capture<___, Var<...> > with Capture<___, Var<...> >,
  // put `Vector< Capture<_, Var<...>> >` to current environment.
  using MaybeEnv = typename ConditionalApply<
      When<Bool<IsCaptureAnySingle<Source>::value>,
           DeferApply<DictPut, Environ, Pair<SomeVarName, Source>>>,
      When<Bool<IsCaptureAnyList<Source>::value>,
           DeferApply<DictPut, Environ, Pair<SomeVarName, Vector<Source>>>>,
      Else<DeferApply<NilF>>>::type;

  // If `Source` is a template(C<Args...>) but not `Capture<_/___, Var<...> >`,
  using _IsTemplateButNotCaptureAny = Bool<IsTemplate<Source>::value && !_TargetIsCaptureAny::value>;

  // If `Source` a template(C<Args...>) but not `Capture<_/___, Var<...> >`,
  // pack it's arguments into `internal::MatchList`.
  using MaybeMatchList = typename ConditionalApply<
      When<_IsTemplateButNotCaptureAny,
           DeferApply<GetArgsToMatchList, Source>>,
      Else<DeferApply<NilF>>>::type;

  // If `Source` a template(C<Args...>) but not `Capture<_/___, Var<...> >`,
  // match it recursively.
  using MatchResult = typename ConditionalApply<
      When<_TargetIsCaptureAny,
           DeferConstruct<MatchSuccess, MaybeEnv>>,
      When<_IsTemplateButNotCaptureAny,
           DeferConstruct<QuoteMatchInternal, Environ, MaybeMatchList, MaybeMatchList>>,
      Else<DeferConstruct<MatchSuccess, Environ>>>::type;

  /*
   * 1. When match Capture<A, Var<...>> with Capture<A, Var<...>>,
   * we regard it as a match failure (because Capture<A, B> != A).
   * If you what to match Capture<A, Var<...>>, use Capture<_, Var<...> > instead.
   *
   * 2. When match C<Args...> with C<Args...>, we still need to match `Args` recursively.
   * Consider the follow example:
   *   QuoteMatchCase< T, Vector< Capture<A, Var<'a'>> >
   * Users use this line to match a single element array and capture it's first element `A` to `Var<'a'>`.
   * But when the given `T` is `Vector< Capture<A, Var<'a'> >`.
   * we got `QuoteMatchCase< Vector< Capture<A, Var<'a'> >, Vector< Capture<A, Var<'a'>> > >`
   * In this case, we regard it as a match failure, because the first element of the array is
   * `Capture<_, Var<'a'>`, and it doesn't match `A`.
   */
  static const bool matched = MatchResult::matched;
  using env = typename MatchResult::env;
};

template <typename Environ>
struct QuoteMatchInternal<Environ,
                          internal::MatchList<>,
                          internal::MatchList<>> {
  static const bool matched = true;
  using env = Environ;
};

template <typename Environ, typename Source, typename Target>
struct QuoteMatchInternal<Environ,
                          internal::MatchList<Source>,
                          internal::MatchList<Target>> {
  using SubMatch = QuoteMatchCase<Environ, Source, Target>;

  static const bool matched = SubMatch::matched;
  using env = typename SubMatch::env;
};

template <typename Environ,
          typename SourceHead, typename... SourceTail,
          typename TargetHead, typename... TargetTail>
struct QuoteMatchInternal<Environ,
                          internal::MatchList<SourceHead, SourceTail...>,
                          internal::MatchList<TargetHead, TargetTail...>> {
  using HeadMatch = QuoteMatchCase<Environ, SourceHead, TargetHead>;
  using HeadMatched = Bool<HeadMatch::matched>;

  using MatchResult = typename ConditionalApply<
      When<HeadMatched,
           DeferConstruct<QuoteMatchInternal,
                          typename HeadMatch::env,
                          internal::MatchList<SourceTail...>,
                          internal::MatchList<TargetTail...>>>,
      Else<DeferConstruct<MatchFailure, Environ>>>::type;

  static const bool matched = MatchResult::matched;
  using env = typename MatchResult::env;
};
}  // namespace util

#endif  //CRISP_UTIL_HPP
