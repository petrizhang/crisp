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

using x = var('x');
using y = var('y');

using prgram_make_addx = lambda(params(y),
                                lambda(params(x), add(x, y)));

int main() {
  using make_addx = eval(prgram_make_addx)::type;
  using add1 = eval(call(make_addx, v(1)))::type;
  using add2 = eval(call(make_addx, v(2)))::type;

  using r1 = eval(call(add1, v(1)))::type;
  using r2 = eval(call(add2, v(1)))::type;

  static_assert(r1::c_value() == 2, "");
  std::cout << r1::c_value() << std::endl;
  static_assert(r2::c_value() == 3, "");
  std::cout << r2::c_value() << std::endl;

  return 0;
}