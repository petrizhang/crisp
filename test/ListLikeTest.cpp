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

#include "crisp/util/ListLike.hpp"

template <typename... Args>
struct list {};

using namespace util;

int main() {
  /**
   *  TemplateHead tests
   */
  {
    // This line will trigger a compile error:
    // using t0 = typename VectorLikeHead<A<>>::type;

    using t1 = typename ListLikeHead<list<int>>::type;
    static_assert(std::is_same<t1, int>::value, "");

    using t2 = typename ListLikeHead<list<bool, int, char>>::type;
    static_assert(std::is_same<t2, bool>::value, "");
  }

  /**
 *  TemplateLast tests
 */
  {
    // This line will trigger a compile error:
    // using t0 = typename VectorLikeLast<A<>>::type;

    using t1 = typename ListLikeLast<list<int>>::type;
    static_assert(std::is_same<t1, int>::value, "");

    using t2 = typename ListLikeLast<list<bool, int, char>>::type;
    static_assert(std::is_same<t2, char>::value, "");
  }

  /**
   *  TemplateTail tests
   */
  {
    using t0 = typename ListLikeTail<list<int>>::type;
    static_assert(std::is_same<t0, list<>>::value, "");

    using t1 = typename ListLikeTail<list<bool, int>>::type;
    static_assert(std::is_same<t1, list<int>>::value, "");

    using t2 = typename ListLikeTail<list<bool, int, char>>::type;
    static_assert(std::is_same<t2, list<int, char>>::value, "");
  }

  /**
   * TemplatePushFront tests
   */
  {
    using t0 = typename ListLikePushFront<list<>, int>::type;
    static_assert(std::is_same<t0, list<int>>::value, "");

    using t1 = typename ListLikePushFront<list<bool>, int>::type;
    static_assert(std::is_same<t1, list<int, bool>>::value, "");
  }

  /**
    * TemplatePushBack tests
    */
  {
    using t0 = typename ListLikePushBack<list<>, int>::type;
    static_assert(std::is_same<t0, list<int>>::value, "");

    using t1 = typename ListLikePushBack<list<bool>, int>::type;
    static_assert(std::is_same<t1, list<bool, int>>::value, "");
  }

  /**
   * TemplateConcat tests
   */
  {
    using t0 = typename ListLikeConcat<list<>, list<>>::type;
    static_assert(std::is_same<t0, list<>>::value, "");

    using t1 = typename ListLikeConcat<list<int>, list<>>::type;
    static_assert(std::is_same<t1, list<int>>::value, "");

    using t2 = typename ListLikeConcat<list<>, list<int>>::type;
    static_assert(std::is_same<t2, list<int>>::value, "");

    using t3 = typename ListLikeConcat<list<int, bool>, list<int, char, bool>>::type;
    static_assert(std::is_same<t3, list<int, bool, int, char, bool>>::value, "");
  }
  return 0;
}