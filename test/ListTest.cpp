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

#include "crisp/interpreter/Interpreter.hpp"

using namespace crisp;
using namespace util;
using namespace crisp;
using std::is_same;

int main() {
  /**
   * List tests
   */
  {
    using l0 = List<Int<1>, Add<Int<1>, Int<2>>>;
    using t0 = typename Interpret<l0>::type;
    static_assert(is_same<t0, List<Int<1>, Int<3>>>::value, "");
  }

  /**
   * Head tests
   */
  {
    using l0 = List<Int<1>, Add<Int<1>, Int<2>>>;
    using t0 = typename Interpret<Head<l0>>::type;
    // This line will trigger a compile error:
    // using t1 = typename Interpret<Head<List<>>>::type;
    static_assert(is_same<t0, Int<1>>::value, "");
  }

  /**
    * Tail tests
    */
  {
    using l0 = List<Int<1>, Int<2>, Int<3>, Add<Int<2>, Int<2>>>;
    using t0 = typename Interpret<Tail<l0>>::type;
    static_assert(is_same<t0, List<Int<2>, Int<3>, Int<4>>>::value, "");
  }

  /**
    * Concat tests
    */
  {
    using l1 = List<Int<1>, Char<'2'>>;
    using l2 = List<Int<3>, Char<'4'>>;
    using t0 = typename Interpret<Concat<l1, l2>>::type;
    static_assert(is_same<t0, List<Int<1>, Char<'2'>, Int<3>, Char<'4'>>>::value, "");

    using l3 = List<Int<5>, Add<Int<3>, Int<3>>>;
    using t1 = typename Interpret<Concat<l1, l3>>::type;
    static_assert(is_same<t1, List<Int<1>, Char<'2'>, Int<5>, Int<6>>>::value, "");
  }

  /**
    * PushFront tests
    */
  {
    using l0 = List<Int<1>, Int<2>>;
    using t0 = typename Interpret<PushFront<l0, Int<3>>>::type;
    static_assert(is_same<t0, List<Int<3>, Int<1>, Int<2>>>::value, "");

    using l1 = List<>;
    using t1 = typename Interpret<PushFront<l1, Int<3>>>::type;
    static_assert(is_same<t1, List<Int<3>>>::value, "");
  }

  /**
    * PushBack tests
    */
  {
    using l0 = List<Int<1>, Int<2>>;
    using t0 = typename Interpret<PushBack<l0, Int<3>>>::type;
    static_assert(is_same<t0, List<Int<1>, Int<2>, Int<3>>>::value, "");

    using l1 = List<>;
    using t1 = typename Interpret<PushBack<l1, Int<3>>>::type;
    static_assert(is_same<t1, List<Int<3>>>::value, "");
  }

  /**
    * PopFront tests
    */
  {
    using l0 = List<Int<1>, Int<2>>;
    using t0 = typename Interpret<PopFront<l0>>::type;
    static_assert(is_same<t0, Int<1>>::value, "");
  }

  /**
    * PopBack tests
    */
  {
    using l0 = List<Int<1>, Int<2>>;
    using t0 = typename Interpret<PopBack<l0>>::type;
    static_assert(is_same<t0, Int<2>>::value, "");
  }

  /**
    * PopFront tests
    */
  {
    using l0 = List<Int<1>, Int<2>>;
    using t0 = typename Interpret<DropHead<l0>>::type;
    static_assert(is_same<t0, List<Int<2>>>::value, "");

    using l1 = List<Int<1>>;
    using t1 = typename Interpret<DropHead<l1>>::type;
    static_assert(is_same<t1, List<>>::value, "");
  }

  /**
    * PopBack tests
    */
  {
    using l0 = List<Int<1>, Int<2>>;
    using t0 = typename Interpret<DropLast<l0>>::type;
    static_assert(is_same<t0, List<Int<1>>>::value, "");

    using l1 = List<Int<1>>;
    using t1 = typename Interpret<DropLast<l1>>::type;
    static_assert(is_same<t1, List<>>::value, "");
  }

  return 0;
}