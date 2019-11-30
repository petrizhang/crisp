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

#ifndef CRISP_MATCH_UTIL_HPP
#define CRISP_MATCH_UTIL_HPP

#include "Conditional.hpp"
#include "Defer.hpp"
#include "InternalList.hpp"
#include "TemplateUtil.hpp"
#include "ast/AST.hpp"

namespace util {
using ast::_;
using ast::___;
using ast::Capture;
using ast::Case;
using ast::Default;
using ast::Quote;

/**
 * Get `Condition` in `Case<Condition,Result>`
 */
template <typename>
struct GetCaseCondition;

template <typename Condition, typename Result>
struct GetCaseCondition<Case<Condition, Result>> {
  using type = Condition;
};

/**
 * Get `Result` in `Case<Condition,Result>`
 */
template <typename>
struct GetCaseResult;

template <typename Condition, typename Result>
struct GetCaseResult<Case<Condition, Result>> {
  using type = Result;
};

/**
 * Get `Result` in `Default<Result>`
 */
template <typename>
struct GetDefaultResult;

template <typename Result>
struct GetDefaultResult<Default<Result>> {
  using type = Result;
};

/**
 * Get `AST` from `Quote<AST>`
 */
template <typename>
struct GetQuoteAST;

template <typename AST>
struct GetQuoteAST<Quote<AST>> {
  using type = AST;
};

/**
 * Get template arguments into internal::InternalList
 * e.g. C<Args...> => internal::InternalList<Args...>
 * @tparam T
 */
template <typename T>
struct GetArgsToInternalList {
  using type = internal::InternalList<>;
};

template <template <typename...> class C, typename... Args>
struct GetArgsToInternalList<C<Args...>> {
  using type = internal::InternalList<Args...>;
};

/**
 * Get the `Target` type in `Capture<Target,VarName>`
 * @tparam T
 */
template <typename T>
struct GetCaptureTarget {
  using type = Undefined;
};

template <typename Target, typename VarName>
struct GetCaptureTarget<Capture<Target, VarName>> {
  using type = Target;
};

/**
 * Get the `VarName` type in `Capture<Target,VarName>`
 * @tparam T
 */
template <typename T>
struct GetCaptureVarName {
  using type = Undefined;
};

template <typename Target, typename VarName>
struct GetCaptureVarName<Capture<Target, VarName>> {
  using type = VarName;
};

/**
 * Test if the given type `T` is `Capture<...>`
 * @tparam T
 */
template <typename T>
struct IsCapture : IsTemplateOf<Capture, T> {};

/**
 * Test if the given type `T` is Capture<_,...> or Capture<___,...>
 * @tparam T
 */
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

/**
 * Test if the given type `T` is Capture<_,...>
 * @tparam T
 */
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

/**
 * Test if the given type `T` is Capture<___,...>
 */
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

/**
 * Test if the given type `T` is `Capture<A,...>` where `A` is not `_` or `___`
 * @tparam T
 */
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

/**
 * Helper template used in match implementation.
 * It saves a `CaptureDict` and provides `matched` as false
 * @tparam CaptureDict
 */
template <typename CaptureDict>
struct MatchFailure {
  static const bool matched = false;
  using dict = CaptureDict;
};

/**
 * Helper template used in match implementation
 * It saves a `CaptureDict` and provides `matched` as true
 * @tparam CaptureDict
 */
template <typename CaptureDict>
struct MatchSuccess {
  static const bool matched = true;
  using dict = CaptureDict;
};

/**
 * QuoteMatchCase implementation
 * @tparam CaptureDict
 * @tparam SourceInternalList
 * @tparam TargetInternalList
 */
template <typename CaptureDict,
          typename SourceInternalList,
          typename TargetInternalList>
struct MatchInternal {
  static const bool matched = false;
  using dict = CaptureDict;
};

template <typename CaptureDict, typename Source, typename Target>
struct MatchCase {
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

  using SomeSoureInternalList = typename ConditionalApply<
      When<_SourceIsTemplate,
           DeferApply<GetArgsToInternalList, Source>>,
      Else<DeferApply<NilF>>>::type;

  using SomeTargetInternalList = typename ConditionalApply<
      When<_TargetIsTemplate,
           DeferApply<GetArgsToInternalList, Target>>,
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
           DeferConstruct<MatchCase, CaptureDict, Source, SomeCaptureTarget>>,
      Else<DeferConstruct<MatchFailure, CaptureDict>>>::type;

  // 3. A<Args...> =?= A<Args...>
  using TemplateMatch = typename ConditionalApply<
      When<_IsSameTemplateAndNotTargetIsCapture,
           DeferConstruct<MatchInternal, CaptureDict, SomeSoureInternalList, SomeTargetInternalList>>,
      Else<DeferConstruct<MatchFailure, CaptureDict>>>::type;

  using Matched = typename Conditional<
      When<_IsTargetMatchAny, Bool<true>>,
      When<_TargetIsCapture, Bool<CaptureMatch::matched>>,
      When<_IsSameTemplateAndNotTargetIsCapture, Bool<TemplateMatch::matched>>,
      Else<Bool<false>>>::type;

  static const bool matched = Matched::value;

  using dict = typename Conditional<
      When<_IsTargetMatchAny, CaptureDict>,
      When<_TargetIsCapture, typename DictPutImpl<typename CaptureMatch::dict, SomeVarName, Source>::type>,
      When<_IsSameTemplateAndNotTargetIsCapture, typename TemplateMatch::dict>,
      Else<CaptureDict>>::type;
};

template <typename CaptureDict, typename Source>
struct MatchCase<CaptureDict, Source, Source> {
  // If `Source` is Capture<_, Var<...> > or Capture<___, Var<...> >
  using _TargetIsCaptureAny = Bool<IsCaptureAny<Source>::value>;

  // When match Capture<_, Var<...> > with Capture<_, Var<...> >,
  // get `Var<...>` from given expression.
  using SomeVarName = typename ConditionalApply<
      When<_TargetIsCaptureAny,
           DeferApply<GetCaptureVarName, Source>>,
      Else<DeferApply<NilF>>>::type;

  // When match Capture<_, Var<...> > with Capture<_, Var<...> >,
  // put `Capture<_, Var<...> >` to current dictironment.
  // When match Capture<___, Var<...> > with Capture<___, Var<...> >,
  // put `List< Capture<_, Var<...>> >` to current dictironment.
  using MaybeEnv = typename ConditionalApply<
      When<Bool<IsCaptureAnySingle<Source>::value>,
           DeferApply<DictPutImpl, CaptureDict, SomeVarName, Source>>,
      When<Bool<IsCaptureAnyList<Source>::value>,
           DeferApply<DictPutImpl, CaptureDict, SomeVarName, List<Source>>>,
      Else<DeferApply<NilF>>>::type;

  // If `Source` is a template(C<Args...>) but not `Capture<_/___, Var<...> >`,
  using _IsTemplateButNotCaptureAny = Bool<IsTemplate<Source>::value && !_TargetIsCaptureAny::value>;

  // If `Source` a template(C<Args...>) but not `Capture<_/___, Var<...> >`,
  // pack it's arguments into `internal::InternalList`.
  using MaybeInternalList = typename ConditionalApply<
      When<_IsTemplateButNotCaptureAny,
           DeferApply<GetArgsToInternalList, Source>>,
      Else<DeferApply<NilF>>>::type;

  // If `Source` a template(C<Args...>) but not `Capture<_/___, Var<...> >`,
  // match it recursively.
  using MatchResult = typename ConditionalApply<
      When<_TargetIsCaptureAny,
           DeferConstruct<MatchSuccess, MaybeEnv>>,
      When<_IsTemplateButNotCaptureAny,
           DeferConstruct<MatchInternal, CaptureDict, MaybeInternalList, MaybeInternalList>>,
      Else<DeferConstruct<MatchSuccess, CaptureDict>>>::type;

  /*
   * 1. When match Capture<A, Var<...>> with Capture<A, Var<...>>,
   * we regard it as a match failure (because Capture<A, B> != A).
   * If you what to match Capture<A, Var<...>>, use Capture<_, Var<...> > instead.
   *
   * 2. When match C<Args...> with C<Args...>, we still need to match `Args` recursively.
   * Consider the follow example:
   *   MatchCase< T, List< Capture<A, Var<'a'>> >
   * Users use this line to match a single element array and capture it's first element `A` to `Var<'a'>`.
   * But when the given `T` is `List< Capture<A, Var<'a'> >`.
   * we got `MatchCase< List< Capture<A, Var<'a'> >, List< Capture<A, Var<'a'>> > >`
   * In this case, we regard it as a match failure, because the first element of the array is
   * `Capture<_, Var<'a'>`, and it doesn't match `A`.
   */
  static const bool matched = MatchResult::matched;
  using dict = typename MatchResult::dict;
};

template <typename CaptureDict>
struct MatchInternal<CaptureDict,
                     internal::InternalList<>,
                     internal::InternalList<>> {
  static const bool matched = true;
  using dict = CaptureDict;
};

template <typename CaptureDict, typename Source, typename Target>
struct MatchInternal<CaptureDict,
                     internal::InternalList<Source>,
                     internal::InternalList<Target>> {
  using SubMatch = MatchCase<CaptureDict, Source, Target>;

  static const bool matched = SubMatch::matched;
  using dict = typename SubMatch::dict;
};

template <typename CaptureDict,
          typename SourceHead, typename... SourceTail,
          typename TargetHead, typename... TargetTail>
struct MatchInternal<CaptureDict,
                     internal::InternalList<SourceHead, SourceTail...>,
                     internal::InternalList<TargetHead, TargetTail...>> {
  using HeadMatch = MatchCase<CaptureDict, SourceHead, TargetHead>;
  using HeadMatched = Bool<HeadMatch::matched>;

  using MatchResult = typename ConditionalApply<
      When<HeadMatched,
           DeferConstruct<MatchInternal,
                          typename HeadMatch::dict,
                          internal::InternalList<SourceTail...>,
                          internal::InternalList<TargetTail...>>>,
      Else<DeferConstruct<MatchFailure, CaptureDict>>>::type;

  static const bool matched = MatchResult::matched;
  using dict = typename MatchResult::dict;
};
}  // namespace util
#endif  //CRISP_MATCH_UTIL_HPP
