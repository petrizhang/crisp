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
    std::cout << run(v(1)) << std::endl;
    std::cout << run(v(true)) << std::endl;
    std::cout << run(v('c')) << std::endl;
    std::cout << run(sym('s', 'y', 'm', 'b', 'o', 'l')) << std::endl;
  }
  // Basic operations
  {
    // 1+2 = 3
    std::cout << run(add(v(1), v(2))) << std::endl;
    // 1-2 = -1
    std::cout << run(sub(v(1), v(2))) << std::endl;
    // 1*2 = 2
    std::cout << run(mul(v(1), v(2))) << std::endl;
    // 1 == 2 -> true
    std::cout << run(eq_(v(1), v(2))) << std::endl;
    // 1 > 2 -> false
    std::cout << run(gt(v(1), v(2))) << std::endl;
    // 1 >= 2 -> false
    std::cout << run(ge(v(1), v(2))) << std::endl;
    // 1 < 2 -> true
    std::cout << run(lt(v(1), v(2))) << std::endl;
    // 1 <= 2 -> true
    std::cout << run(le(v(1), v(2))) << std::endl;
  }
  // Variable definition
  {
    /*
     * int a = 100;
     * return a;
     */
    std::cout << run(block(define(var('a'), v(100)),
                           var('a')))
              << std::endl;
  }
  // If-then-else
  {
    // true ? 1 : 2
    std::cout << run(if_(v(true), v(1), v(2))) << std::endl;
  }
  // Function definition
  {
    std::cout << run(block(define(var('+'), lambda(params(var('x'), var('y')),
                                                   add(var('x'), var('y')))),
                           call(var('+'), v(10), v(20))))
              << std::endl;
  }
  // Closure
  {
    using x = var('x');
    using y = var('y');
    using add1 = var('a', 'd', 'd', '1');
    using add2 = var('a', 'd', 'd', '2');
    using makeAddX = var('m', 'a', 'k', 'e', 'A', 'd', 'd', 'X');

    // 11
    auto v1 = run(block(define(makeAddX, lambda(params(y),
                                                lambda(params(x), add(x, y)))),
                        define(add1, call(makeAddX, v(1))),
                        call(add1, v(10))));

    // 12
    auto v2 = run(block(define(makeAddX, lambda(params(y),
                                                lambda(params(x), add(x, y)))),
                        define(add2, call(makeAddX, v(2))),
                        call(add2, v(10))));

    // 11
    std::cout << v1 << std::endl;
    // 12
    std::cout << v2 << std::endl;
  }
  // Recursive Function
  {
    using n = var('n');
    using factorial = var('f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l');

    // 3628800
    auto v3 = run(block(define(factorial, lambda(params(n),
                                                 if_(eq_(n, v(1)),
                                                     v(1),
                                                     mul(n, call(factorial, sub(n, v(1))))))),
                        call(factorial, v(10))));
    // 3628800
    std::cout << v3 << std::endl;
  }
  {
    std::cout << run(lambda(params(var('a')),
                            var('a')));
  }
}
