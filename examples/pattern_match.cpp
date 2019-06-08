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

#include "crisp_macros.h"
using namespace crisp;

template <typename Target, typename VarName>
struct Capture {
  static constexpr const char* repr = "capture";
};

template <typename T>
struct GetCaptureTarget {
  using type = Undefined;
};

template <typename Target, typename VarName>
struct GetCaptureTarget<Capture<Target, VarName>> {
  using type = Target;
};

template <typename T>
struct GetCaptureVarName {
  using type = Undefined;
};

template <typename Target, typename VarName>
struct GetCaptureVarName<Capture<Target, VarName>> {
  using type = VarName;
};

template <typename T>
struct IsCapture : IsTemplateOf<Capture, T> {};

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

template <typename Environ, typename Source, typename Target>
struct QuoteMatchCase {
  static const bool matched = false;
  using env = Environ;
};

template <typename Environ, typename Source>
struct QuoteMatchCase<Environ, Source, Source> {
  // when match Capture<A, Var<...>> with Capture<A, Var<...>>
  // we regard it as a match failure (because Capture<A, B> != A).
  // If you what to match Capture<A, Var<...>>, use Capture<_, Var<...> > instead.
  static constexpr bool matched = !IsCaptureSpecific<Source>::value;

  // when match Capture<_, Var<...> > with Capture<_, Var<...> >,
  // capture the expression to current environment
  using MayBeVarName = typename ConditionalApply<When<Bool<IsCaptureAny<Source>::value>,
                                                      DeferApply<GetCaptureVarName, Source>>,
                                                 Else<DeferApply<NilF>>>::type;

  using env = typename ConditionalApply<When<Bool<std::is_same<Nil, MayBeVarName>::value>,
                                             DeferApply<Id, Environ>>,
                                        Else<DeferApply<EnvPut, Environ, MayBeVarName, Quote<Source>>>>::type;
};

//template <typename Environ>
//struct QuoteMatchCase<Environ,
//                      internal::MatchList<>,
//                      internal::MatchList<>> {
//  static const bool matched = true;
//  using env = Environ;
//};
//
//template <typename Environ,
//          typename SourceMatchList,
//          typename TargetMatchList>
//struct QuoteMatchInternal;
//
//template <typename Environ, typename T>
//struct QuoteMatchInternal<Environ,
//                          internal::MatchList<T>,
//                          internal::MatchList<T>> {
//  static const bool matched = true;
//  using env = Environ;
//};
//
//template <typename Environ, typename Source>
//struct QuoteMatchInternal<Environ,
//                          internal::MatchList<Source>,
//                          internal::MatchList<_>> {
//  static const bool matched = true;
//  using env = Environ;
//};
//
//template <typename Environ, typename Source, char... chars>
//struct QuoteMatchInternal<Environ,
//                          Source, internal::MatchList<Source>,
//                          Var<chars...>, internal::MatchList<>> {
//  static const bool matched = true;
//  using env = typename EnvPut<Environ, Var<chars...>, Quote<Source>>::type;
//};
//
//template <typename Environ,
//          template <typename...> class T,
//          typename SourceHead, typename... SourceTail,
//          typename TargetHead, typename... TargetTail>
//struct QuoteMatchCase<Environ, T<SourceHead, SourceTail...>, T<TargetHead, TargetTail...>>
//    : QuoteMatchInternal<Environ,
//                         SourceHead, internal::MatchList<SourceTail...>,
//                         TargetHead, internal::MatchList<TargetTail...>> {
//};
//
//template <typename Environ, typename SourceHead, typename TargetHead>
//struct QuoteMatchInternal<Environ,
//                          SourceHead, internal::MatchList<>,
//                          TargetHead, internal::MatchList<TargetHead>>
//    : QuoteMatchCase<Environ, SourceHead, TargetHead> {};
//
//template <typename Environ,
//          typename SourceHead, typename... SourceTail,
//          typename TargetHead, typename... TargetTail>
//struct QuoteMatchInternal<Environ,
//                          internal::MatchList<SourceHead, SourceTail...>,
//                          internal::MatchList<TargetHead, TargetTail...>> {
//  using HeadMatch = QuoteMatchCase<Environ, SourceHead, TargetHead>;
//  using HeadMatchEnv = typename HeadMatch::env;
//
//  using MatchResult = typename ConditionalImpl<HeadMatch::matched,
//                                                QuoteMatchInternal,
//                                                Array<HeadMatchEnv,
//                                                      internal::MatchList<SourceTail...>,
//                                                      internal::MatchList<TargetTail...>>,
//                                                QuoteMatchCase,
//                                                Array<Env<>, int, bool>>::type;
//
//  static const bool matched = MatchResult::matched;
//  using env = typename MatchResult::env;
//};
//
//template <typename Environ, typename Source, typename Target>
//struct QuoteMatchReversedTail {
//  static const bool matched = false;
//  using env = Environ;
//};
//
//template <typename Environ, typename SourceTail, typename TargetTail>
//struct QuoteMatchReversedTail<Environ,
//                              internal::MatchList<SourceTail>,
//                              internal::MatchList<TargetTail>>
//    : QuoteMatchCase<Environ, SourceTail, TargetTail> {
//};
//
//template <typename Environ,
//          typename... SourceRHead, typename SourceRTail,
//          typename... TargetRHead, typename TargetRTail>
//struct QuoteMatchReversedTail<Environ,
//                              internal::MatchList<SourceRTail, SourceRHead...>,
//                              internal::MatchList<TargetRTail, TargetRHead...>> {
//  using RTailMatch = QuoteMatchCase<Environ, SourceRTail, TargetRTail>;
//  using RTailMatchEnv = typename RTailMatch::env;
//
//  using MatchResult = typename ConditionalImpl<RTailMatch::matched,
//                                                QuoteMatchReversedTail,
//                                                Array<RTailMatchEnv,
//                                                      internal::MatchList<SourceRHead...>,
//                                                      internal::MatchList<TargetRHead...>>,
//                                                QuoteMatchReversedTail,
//                                                Array<Env<>, int, bool>>::type;
//
//  static const bool matched = MatchResult::matched;
//  using env = typename MatchResult::env;
//};
//
//template <typename Environ,
//          template <typename...> class T,
//          typename... SourceArgs,
//          typename... TargetArgs>
//struct QuoteMatchInternal<Environ,
//                          internal::MatchList<SourceArgs...>,
//                          internal::MatchList<___, TargetArgs...>>
//    : QuoteMatchReversedTail<Environ,
//                             typename ReverseArgs<internal::MatchList<SourceArgs...>>::type,
//                             typename ReverseArgs<internal::MatchList<TargetArgs...>>::type> {
//};
void TestPatternMatch1() {
  using x = Var<'x'>;
  static_assert((QuoteMatchCase<Env<>, Capture<_, x>, Capture<_, x>>::matched), "");
  static_assert((!QuoteMatchCase<Env<>, Capture<_, x>, Capture<x, x>>::matched), "");

  static_assert((QuoteMatchCase<Env<>, int, int>::matched), "");
};

int main() {
  return 0;
}