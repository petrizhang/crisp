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
#include "core/Interpreter.hpp"

using namespace crisp;
using namespace crisp;
using namespace util;
using std::is_same;

int main() {
  /**
   * A test group
   */
  {
    using t0 = typename Interpret<Cond<When<Bool<true>, Int<1>>,
                                       Else<Int<2>>>>::type;
    static_assert(is_same<t0, Int<1>>::value, "");

    using t1 = typename Interpret<Cond<When<Bool<false>, Int<1>>,
                                       When<And<Bool<false>, Bool<true>>, Int<2>>,
                                       When<And<Bool<true>, Bool<true>>, Int<3>>,
                                       Else<Int<4>>>>::type;
    static_assert(is_same<t1, Int<3>>::value, "");

    using t2 = typename Interpret<Cond<When<Bool<false>, Int<1>>,
                                       When<And<Bool<false>, Bool<true>>, Int<2>>,
                                       When<And<Bool<true>, Bool<false>>, Int<3>>,
                                       Else<Int<4>>>>::type;
    static_assert(is_same<t2, Int<4>>::value, "");
  }
  return 0;
}