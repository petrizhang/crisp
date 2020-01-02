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

#include <type_traits>

#include "crisp/ast/CoreAST.hpp"
#include "crisp/util/Util.hpp"

using namespace crisp;
using namespace util;

int main() {
  using x = Var<'x'>;
  using y = Var<'y'>;

  /**
   * Single-element templates tests
   */
  {
    static_assert((MatchCase<Dict<>, int, int>::matched), "");
    static_assert((MatchCase<Dict<>, List<>, List<>>::matched), "");
    static_assert((!MatchCase<Dict<>, List<>, List<Int<1>>>::matched), "");
    static_assert((!MatchCase<Dict<>, List<Int<2>>, List<Int<1>>>::matched), "");
    static_assert((MatchCase<Dict<>, List<Int<1>>, List<Int<1>>>::matched), "");
  }

  /**
   * Capture tests
   */
  {
    static_assert((MatchCase<Dict<>, List<Int<1>>, List<Capture<_, x>>>::matched), "");
    using t0 = MatchCase<Dict<>, List<Int<1>>, List<Capture<Int<1>, x>>>;
    static_assert((t0::matched), "");
    static_assert((std::is_same<typename DictGetImpl<t0::dict, x>::type, Int<1>>::value), "");

    static_assert((!MatchCase<Dict<>, List<Int<1>>, List<Capture<Int<2>, x>>>::matched), "");

    static_assert((MatchCase<Dict<>, Capture<_, x>, Capture<_, x>>::matched), "");
    static_assert((!MatchCase<Dict<>, Capture<_, x>, Capture<x, x>>::matched), "");

    static_assert((MatchCase<Dict<>, int, int>::matched), "");
  }

  /**
   * Complex match tests
   */
  {
    static_assert((!MatchCase<Dict<>, List<Int<1>, Int<2>>, List<Int<1>>>::matched), "");
    static_assert((MatchCase<Dict<>, List<Int<1>, Int<2>>, List<Int<1>, Int<2>>>::matched), "");

    using l = List<List<Int<1>, Int<2>>,
                   List<
                       List<Int<3>,
                            List<Int<4>, Char<'x'>>>>>;

    using target0 = List<_, _>;
    static_assert((MatchCase<Dict<>, l, target0>::matched), "");

    using target1 = List<List<_, _>, Capture<_, x>>;
    using match1 = MatchCase<Dict<>, l, target1>;
    static_assert(match1::matched, "");
    static_assert((std::is_same<typename DictGetImpl<match1::dict, x>::type,
                                List<List<Int<3>, List<Int<4>, Char<'x'>>>>>::value),
                  "");

    using target2 = List<List<_, _>, List<List<Capture<_, x>, _>>>;
    using match2 = MatchCase<Dict<>, l, target2>;
    static_assert(match2::matched, "");
    static_assert((std::is_same<typename DictGetImpl<match2::dict, x>::type,
                                Int<3>>::value),
                  "");

    static_assert((MatchCase<Dict<>,
                             List<List<Int<1>, Int<2>>>,
                             List<List<Int<1>, Int<2>>>>::matched),
                  "");

    static_assert((MatchCase<Dict<>,
                             List<List<Int<1>, Int<2>>>,
                             List<List<Int<1>, _>>>::matched),
                  "");

    static_assert((MatchCase<Dict<>,
                             List<List<Int<1>, Int<2>>>,
                             List<List<_, _>>>::matched),
                  "");

    static_assert((!MatchCase<Dict<>,
                              List<List<Int<2>, Int<2>>>,
                              List<List<Int<1>, Int<2>>>>::matched),
                  "");

    using t0 = MatchCase<Dict<>,
                         List<Int<1>, Int<2>>,
                         List<Capture<Int<1>, x>, Int<2>>>;
    static_assert((t0::matched), "");
    static_assert((std::is_same<typename DictGetImpl<t0::dict, x>::type, Int<1>>::value), "");
  }

  /**
   * Corner cases
   */

  return 0;
}