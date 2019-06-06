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

#include <iostream>
#include "crisp_macros.h"

int main() {
  // Println
  {
    run(println());
    run(println(v(3)));
    run(println(v(5), v(true)));
  }
  // Basic value types
  {
    run(println(v(1)));
    run(println(v(true)));
    run(println(v('c')));
    run(println(sym('s', 'y', 'm', 'b', 'o', 'l')));
  }
  // Block
  {
    // 3
    run(println(block(v(1),
                      v(2),
                      v(3))));
  }

  // Basic operations
  {
    // 1+2 = 3
    run(println(add(v(1), v(2))));
    // 1-2 = -1
    run(println(sub(v(1), v(2))));
    // 1*2 = 2
    run(println(mul(v(1), v(2))));
    // 1 == 2 -> false
    run(println(eq_(v(1), v(2))));
    // 1 > 2 -> false
    run(println(gt(v(1), v(2))));
    // 1 >= 2 -> false
    run(println(ge(v(1), v(2))));
    // 1 < 2 -> true
    run(println(lt(v(1), v(2))));
    // 1 <= 2 -> true
    run(println(le(v(1), v(2))));
  }
  // Variable definition
  {
    /*
       * int a = 100;
       * println(a);
       */
    run(block(define(var('a'), v(100)),
              var('a'),
              println(var('a'))));
  }
  // If-then-else
  {
    // true ? 1 : 2
    run(println(if_(v(true), v(1), v(2))));
  }
  // Function definition
  {
    run(block(define(var('+'), lambda(params(var('x'), var('y')),
                                      add(var('x'), var('y')))),
              define(var('s'), call(var('+'), v(10), v(20))),
              println(var('s'))));
  }
  // Closure
  {
    using x = var('x');
    using y = var('y');
    using add1 = var('a', 'd', 'd', '1');
    using add2 = var('a', 'd', 'd', '2');
    using makeAddX = var('m', 'a', 'k', 'e', 'A', 'd', 'd', 'X');

    // 11, 12
    run(block(define(makeAddX, lambda(params(y),
                                      lambda(params(x), add(x, y)))),
              define(add1, call(makeAddX, v(1))),
              define(add2, call(makeAddX, v(2))),
              println(call(add1, v(10))),    // 11
              println(call(add2, v(10)))));  //12
  }
  // Recursive Function
  {
    using n = var('n');
    using factorial = var('f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l');

    // 3628800
    run(block(define(factorial, lambda(params(n),
                                       if_(eq_(n, v(1)),
                                           v(1),
                                           mul(n, call(factorial, sub(n, v(1))))))),
              println(call(factorial, v(10)))));
  }
}
