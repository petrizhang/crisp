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
#include "tispp_macros.h"

int main() {
  using x = var('x');
  using y = var('y');
  using add1 = var('a', 'd', 'd', '1');
  using add2 = var('a', 'd', 'd', '2');
  using make_add_x = var('m', 'a', 'k', 'e', 'a', 'd', 'd', 'x');

  // 11
  auto v1 = eval(seq(define(make_add_x, lambda(params(y),
                                               lambda(params(x), add(x, y)))),
                     define(add1, call(make_add_x, v(1))),
                     call(add1, v(10))))::type::c_value();

  // 12
  auto v2 = eval(seq(define(make_add_x, lambda(params(y),
                                               lambda(params(x), add(x, y)))),
                     define(add2, call(make_add_x, v(2))),
                     call(add2, v(10))))::type::c_value();

  // 11
  std::cout << v1 << std::endl;
  // 12
  std::cout << v2 << std::endl;

  using n = var('n');
  using factorial = var('f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l');

  // 3628800
  auto v3 = eval(seq(define(factorial, lambda(params(n),
                                              if_(eq_(n, v(1)),
                                                  v(1),
                                                  mul(n, call(factorial, sub(n, v(1))))))),
                     call(factorial, v(10))))::type::c_value();
  // 3628800
  std::cout << v3 << std::endl;
}
