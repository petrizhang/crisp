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

#include "CrispMacroAPI.h"
using namespace crisp;



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


int main() {
  using t = QuoteMatchInternal<Env<>,
                               internal::MatchList<_>,
                               internal::MatchList<_>>;
  std::cout << t::matched;

  return 0;
}