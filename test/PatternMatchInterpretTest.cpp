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
#include "ast/CoreAST.hpp"
#include "interpreter/Interpreter.hpp"
#include "util/Util.hpp"

using namespace crisp;
using namespace util;
using namespace crisp;

int main() {
  using x = Var<'x'>;

  /**
   * A test group.
   */
  {
    using x = Var<'x'>;
    using t0 = typename Interpret<Match<Quote<Add<Int<1>, Int<2>>>,
                                        Case<Add<Capture<_, x>, _>, x>,
                                        Default<Char<'1'>>>>::type;
    static_assert((std::is_same<t0, Int<1>>::value), "");

    using t1 = typename Interpret<
        Match<Quote<Add<Int<1>, Int<2>>>,
              Case<Sub<_, _>, Char<'1'>>,
              Case<Add<_, _>, Char<'2'>>,
              Default<Char<'3'>>>>::type;
    static_assert((std::is_same<t1, Char<'2'>>::value), "");
  }
  return 0;
}