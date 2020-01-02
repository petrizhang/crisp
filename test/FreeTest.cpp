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

#include <iostream>
#include <type_traits>

#include "crisp/util/Conditional.hpp"
#include "crisp/util/Defer.hpp"
#include "crisp/util/Dict.hpp"

using namespace crisp;
using namespace util;
using std::is_same;
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

template <typename...>
struct MatchList {};

template <typename CaptureDict, typename Source, typename Target>
struct MatchCaseImpl {
  static const bool matched = is_same<Source, Target>::value;
  using dict = CaptureDict;
};

template <typename CaptureDict, typename Source, char... args>
struct MatchCaseImpl<CaptureDict, Source, Var<args...>> {
  static const bool matched = true;
  using dict = typename DictPutImpl<CaptureDict, Var<args...>, Source>::type;
};

template <typename CaptureDict, typename Source>
struct MatchCaseImpl<CaptureDict, Source, _> {
  static const bool matched = true;
  using dict = CaptureDict;
};

template <typename CaptureDict, typename Source, typename CaptureTarget, typename CaptureVarName>
struct MatchCaseImpl<CaptureDict, Source, Capture<CaptureTarget, CaptureVarName>> {
  static_assert(IsVar<CaptureVarName>::value, "expected a `Var<...>` instantiation in `Capture` expression");
  using TargetMatch = MatchCaseImpl<CaptureDict, Source, CaptureTarget>;
  static const bool matched = TargetMatch::matched;
  using dict = typename TargetMatch ::dict;
};

template <typename CaptureDict,
          template <typename...> class Source, typename... SourceArgs,
          template <typename...> class Target, typename... TargetArgs>
struct MatchCaseImpl<CaptureDict, Source<SourceArgs...>, Target<TargetArgs...>> {
  using NextMatch = MatchCaseImpl<CaptureDict, MatchList<SourceArgs...>, MatchList<TargetArgs...>>;
  static const bool matched = NextMatch::matched;
  using dict = typename NextMatch ::dict;
};

template <typename CaptureDict, typename... SourceArgs, typename... TargetArgs>
struct MatchCaseImpl<CaptureDict, MatchList<SourceArgs...>, MatchList<TargetArgs...>> {
  static const bool matched = false;
  using dict = CaptureDict;
};

int main() {
  auto a = MatchCaseImpl<Dict<>, MatchList<>, MatchList<>>::matched;
  std::cout << a << "\n";
  return 0;
}