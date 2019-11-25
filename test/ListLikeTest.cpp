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
#include "util/ListLike.hpp"

template <typename... Args>
struct A {};

using namespace util;

int main() {
  /**
   *  TemplateHead tests
   */

  // This line will trigger a compile error:
  // using t0 = typename VectorLikeHead<A<>>::type;

  using t1 = typename TemplateHead<A<int>>::type;
  static_assert(std::is_same<t1, int>::value, "");

  using t2 = typename TemplateHead<A<bool, int, char>>::type;
  static_assert(std::is_same<t2, bool>::value, "");

  /**
   *  TemplateTail tests
   */

  using t3 = typename TemplateTail<A<int>>::type;
  static_assert(std::is_same<t3, A<>>::value, "");

  using t4 = typename TemplateTail<A<bool, int>>::type;
  static_assert(std::is_same<t4, A<int>>::value, "");

  using t5 = typename TemplateTail<A<bool, int, char>>::type;
  static_assert(std::is_same<t5, A<int, char>>::value, "");

  /**
   * TemplatePushFront tests
   */

  using t6 = typename TemplatePushFront<A<>, int>::type;
  static_assert(std::is_same<t6, A<int>>::value, "");

  using t7 = typename TemplatePushFront<A<bool>, int>::type;
  static_assert(std::is_same<t7, A<int, bool>>::value, "");

  /**
    * TemplatePushBack tests
    */

  using t8 = typename TemplatePushBack<A<>, int>::type;
  static_assert(std::is_same<t8, A<int>>::value, "");

  using t9 = typename TemplatePushBack<A<bool>, int>::type;
  static_assert(std::is_same<t9, A<bool, int>>::value, "");

  /**
   * TemplateConcat tests
   */

  using t10 = typename TemplateConcat<A<>, A<>>::type;
  static_assert(std::is_same<t10, A<>>::value, "");

  using t11 = typename TemplateConcat<A<int>, A<>>::type;
  static_assert(std::is_same<t11, A<int>>::value, "");

  using t12 = typename TemplateConcat<A<>, A<int>>::type;
  static_assert(std::is_same<t12, A<int>>::value, "");

  using t13 = typename TemplateConcat<A<int, bool>, A<int, char, bool>>::type;
  static_assert(std::is_same<t13, A<int, bool, int, char, bool>>::value, "");
  return 0;
}