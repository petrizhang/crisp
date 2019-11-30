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
#include "CrispMacroAPI.h"

using std::is_same;

int main() {
  using x = var("x");
  using add1 = lambda(params(x),
                      add(x, v(1)));
  /**
   * A test group
   */
  {
    using t0 = interpret(map(list(v(0), v(1), v(2), v(3), v(4)),
                             add1));
    static_assert(is_same<t0, list(v(1), v(2), v(3), v(4), v(5))>::value, "");

    using t1 = interpret(map(list(), add1));
    static_assert(is_same<t1, list()>::value, "");
  }
  return 0;
}