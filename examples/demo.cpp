/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
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
  // Basic value types
  {
    run(println(v(1)));
    run(println(v(true)));
    run(println(v('c')));
    run(println(sym('s', 'y', 'm', 'b', 'o', 'l')));
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
           * return a;
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
    using z = var('z');
    using add1 = var('a', 'd', 'd', '1');
    using add2 = var('a', 'd', 'd', '2');
    using makeAddX = var('m', 'a', 'k', 'e', 'A', 'd', 'd', 'X');

    // 11
    run(block(define(makeAddX, lambda(params(y),
                                      lambda(params(x), add(x, y)))),
              define(add1, call(makeAddX, v(1))),
              define(z, call(add1, v(10))),
              println(z)));

    // 12
    run(block(define(makeAddX, lambda(params(y),
                                      lambda(params(x), add(x, y)))),
              define(add2, call(makeAddX, v(2))),
              define(z, call(add2, v(10))),
              println(z)));
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
