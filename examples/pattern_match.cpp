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

#include "crisp/MacroAPI.h"
using namespace crisp;

//
//template <typename Environ, typename Source, char... chars>
//struct MatchInternal<Environ,
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
//struct MatchCase<Environ, T<SourceHead, SourceTail...>, T<TargetHead, TargetTail...>>
//    : MatchInternal<Environ,
//                         SourceHead, internal::MatchList<SourceTail...>,
//                         TargetHead, internal::MatchList<TargetTail...>> {
//};
//
//template <typename Environ, typename SourceHead, typename TargetHead>
//struct MatchInternal<Environ,
//                          SourceHead, internal::MatchList<>,
//                          TargetHead, internal::MatchList<TargetHead>>
//    : MatchCase<Environ, SourceHead, TargetHead> {};
//
//template <typename Environ,
//          typename SourceHead, typename... SourceTail,
//          typename TargetHead, typename... TargetTail>
//struct MatchInternal<Environ,
//                          internal::MatchList<SourceHead, SourceTail...>,
//                          internal::MatchList<TargetHead, TargetTail...>> {
//  using HeadMatch = MatchCase<Environ, SourceHead, TargetHead>;
//  using HeadMatchEnv = typename HeadMatch::env;
//
//  using MatchResult = typename Conditional<HeadMatch::matched,
//                                                MatchInternal,
//                                                Vector<HeadMatchEnv,
//                                                      internal::MatchList<SourceTail...>,
//                                                      internal::MatchList<TargetTail...>>,
//                                                MatchCase,
//                                                Vector<Env<>, int, bool>>::type;
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
//    : MatchCase<Environ, SourceTail, TargetTail> {
//};
//
//template <typename Environ,
//          typename... SourceRHead, typename SourceRTail,
//          typename... TargetRHead, typename TargetRTail>
//struct QuoteMatchReversedTail<Environ,
//                              internal::MatchList<SourceRTail, SourceRHead...>,
//                              internal::MatchList<TargetRTail, TargetRHead...>> {
//  using RTailMatch = MatchCase<Environ, SourceRTail, TargetRTail>;
//  using RTailMatchEnv = typename RTailMatch::env;
//
//  using MatchResult = typename Conditional<RTailMatch::matched,
//                                                QuoteMatchReversedTail,
//                                                Vector<RTailMatchEnv,
//                                                      internal::MatchList<SourceRHead...>,
//                                                      internal::MatchList<TargetRHead...>>,
//                                                QuoteMatchReversedTail,
//                                                Vector<Env<>, int, bool>>::type;
//
//  static const bool matched = MatchResult::matched;
//  using env = typename MatchResult::env;
//};
//
//template <typename Environ,
//          template <typename...> class T,
//          typename... SourceArgs,
//          typename... TargetArgs>
//struct MatchInternal<Environ,
//                          internal::MatchList<SourceArgs...>,
//                          internal::MatchList<___, TargetArgs...>>
//    : QuoteMatchReversedTail<Environ,
//                             typename ReverseArgs<internal::MatchList<SourceArgs...>>::type,
//                             typename ReverseArgs<internal::MatchList<TargetArgs...>>::type> {
//};

int main() {
  using t = MatchInternal<Env<>,
                          internal::InternalList<_>,
                          internal::InternalList<_>>;
  std::cout << t::matched;

  return 0;
}