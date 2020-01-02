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

#include "crisp/CrispMacroAPI.h"

using std::is_same;

int main() {
  /**
   * A test group
   */
  {
    using result = var("result");
    using add2mul = var("add2mul");
    using x = var("x");
    using y = var("y");
    using left = var("left");
    using right = var("right");

    using t0 = interpret(block(define(var("add2mul"), lambda(params(x),
                                                             match(x,
                                                                   case_(add(capture(_, left), capture(_, right)),
                                                                         mul(left, right)),
                                                                   default_(x)))),
                               define(result, call(var("add2mul"), quote(add(v(3), v(4))))),
                               eval(result)));
    static_assert(is_same<t0, v(12)>::value, "");

    using t1 = interpret(block(eval(quote(define(x, v(1)))),
                               x));

    static_assert(is_same<t1, v(1)>::value, "");
  }
  return 0;
}