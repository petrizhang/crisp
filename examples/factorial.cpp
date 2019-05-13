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

using n = var('n');
using factorial = var('f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l');

using factorial_def = define(factorial, lambda(params(n),
                                               if_(eq_(n, v(1)),
                                                   v(1),
                                                   mul(n, call(factorial, sub(n, v(1)))))));

using program0 = seq(factorial_def,
                     call(factorial, v(3)));

using program1 = seq(factorial_def,
                     call(factorial, v(10)));
int main() {
  using r0 = eval(program0)::type;
  using r1 = eval(program1)::type;

  // factorial(3) = 6
  static_assert(r0::c_value() == 6, "");
  std::cout << r0::c_value() << std::endl;
  // factorial(10) = 3628800
  static_assert(r1::c_value() == 3628800, "");
  std::cout << r1::c_value() << std::endl;
  return 0;
}