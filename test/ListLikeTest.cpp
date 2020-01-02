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
struct A {};

using namespace util;

int main() {
  /**
   *  TemplateHead tests
   */
  {
    // This line will trigger a compile error:
    // using t0 = typename VectorLikeHead<A<>>::type;

    using t1 = typename ListLikeHead<A<int>>::type;
    static_assert(std::is_same<t1, int>::value, "");

    using t2 = typename ListLikeHead<A<bool, int, char>>::type;
    static_assert(std::is_same<t2, bool>::value, "");
  }

  /**
 *  TemplateLast tests
 */
  {
    // This line will trigger a compile error:
    // using t0 = typename VectorLikeLast<A<>>::type;

    using t1 = typename ListLikeLast<A<int>>::type;
    static_assert(std::is_same<t1, int>::value, "");

    using t2 = typename ListLikeLast<A<bool, int, char>>::type;
    static_assert(std::is_same<t2, char>::value, "");
  }

  /**
   *  TemplateTail tests
   */
  {
    using t0 = typename ListLikeTail<A<int>>::type;
    static_assert(std::is_same<t0, A<>>::value, "");

    using t1 = typename ListLikeTail<A<bool, int>>::type;
    static_assert(std::is_same<t1, A<int>>::value, "");

    using t2 = typename ListLikeTail<A<bool, int, char>>::type;
    static_assert(std::is_same<t2, A<int, char>>::value, "");
  }

  /**
   * TemplatePushFront tests
   */
  {
    using t0 = typename ListLikePushFront<A<>, int>::type;
    static_assert(std::is_same<t0, A<int>>::value, "");

    using t1 = typename ListLikePushFront<A<bool>, int>::type;
    static_assert(std::is_same<t1, A<int, bool>>::value, "");
  }

  /**
    * TemplatePushBack tests
    */
  {
    using t0 = typename ListLikePushBack<A<>, int>::type;
    static_assert(std::is_same<t0, A<int>>::value, "");

    using t1 = typename ListLikePushBack<A<bool>, int>::type;
    static_assert(std::is_same<t1, A<bool, int>>::value, "");
  }

  /**
   * TemplateConcat tests
   */
  {
    using t0 = typename ListLikeConcat<A<>, A<>>::type;
    static_assert(std::is_same<t0, A<>>::value, "");

    using t1 = typename ListLikeConcat<A<int>, A<>>::type;
    static_assert(std::is_same<t1, A<int>>::value, "");

    using t2 = typename ListLikeConcat<A<>, A<int>>::type;
    static_assert(std::is_same<t2, A<int>>::value, "");

    using t3 = typename ListLikeConcat<A<int, bool>, A<int, char, bool>>::type;
    static_assert(std::is_same<t3, A<int, bool, int, char, bool>>::value, "");
  }
  return 0;
}