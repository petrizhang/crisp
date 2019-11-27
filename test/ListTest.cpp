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
#include "interpreter/Interpreter.hpp"

using namespace ast;
using namespace util;
using namespace crisp;
using std::is_same;

int main() {
  /**
   * A test group
   */
  {
    using list = List<Int<1>, Add<Int<1>, Int<2>>>;
    using t0 = typename Interpret<list>::type;
    static_assert(is_same<t0, List<Int<1>, Int<3>>>::value, "");
  }

  /**
   * Head tests
   */
  {
    using list = List<Int<1>, Add<Int<1>, Int<2>>>;
    using t0 = typename Interpret<Head<list>>::type;
    // This line will trigger a compile error:
    // using t1 = typename Interpret<Head<List<>>>::type;
    static_assert(is_same<t0, Int<1>>::value, "");
  }

  /**
    * Tail tests
    */
  {
    using list = List<Int<1>, Int<2>, Int<3>, Add<Int<2>, Int<2>>>;
    using t0 = typename Interpret<Tail<list>>::type;
    static_assert(is_same<t0, List<Int<2>, Int<3>, Int<4>>>::value, "");
  }
  return 0;
}