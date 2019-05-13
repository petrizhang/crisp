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

#include <cassert>
#include <iostream>
#include "crisp_templates.h"

using namespace crisp;

void TestDict() {
  using x = Var<'x'>;
  using y = Var<'y'>;
  using d = Dict<>;
  using d1 = DictPut<d, Pair<x, Int<1>>>::type;
  using d2 = DictPut<d1, Pair<y, Int<2>>>::type;
  using d3 = DictPut<d2, Pair<x, Int<3>>>::type;
  using d4 = DictPut<d3, Pair<y, Int<4>>>::type;

  static_assert(DictGet<d1, x>::type::c_value() == 1, "");
  static_assert(DictGet<d2, x>::type::c_value() == 1, "");
  static_assert(DictGet<d2, y>::type::c_value() == 2, "");
  static_assert(DictGet<d3, x>::type::c_value() == 3, "");
  static_assert(DictGet<d4, y>::type::c_value() == 4, "");
}

void TestEnv() {
  using x = Var<'x'>;
  using y = Var<'y'>;
  using d0 = Dict<Pair<x, Int<1>>>;
  using d1 = Dict<Pair<x, Int<2>>, Pair<y, Int<3>>>;

  using e0 = Env<d0>;
  using e1 = EnvPushFront<e0, d1>::type;

  static_assert(EnvLookup<e0, x>::type::c_value() == 1, "");
  static_assert(EnvLookup<e1, x>::type::c_value() == 2, "");
  static_assert(EnvLookup<e1, y>::type::c_value() == 3, "");

  using e2 = Env<>;
  using e3 = EnvPut<e2, x, Int<4>>::type;
  static_assert(EnvLookup<e3, x>::type::c_value() == 4, "");
}

void TestBinaryOpImpl() {
  static_assert((AddImpl<Int<3>, Int<4>>::type::c_value() == 7), "");
  static_assert((SubImpl<Int<100>, Int<4>>::type::c_value() == 96), "");
  static_assert((MulImpl<Int<100>, Int<4>>::type::c_value() == 400), "");
  static_assert((ModImpl<Int<101>, Int<4>>::type::c_value() == 1), "");

  static_assert((IsGreaterEqualImpl<Int<5>, Int<4>>::type::c_value()), "");
  static_assert((IsGreaterEqualImpl<Int<4>, Int<4>>::type::c_value()), "");
  static_assert(!(IsGreaterEqualImpl<Int<3>, Int<4>>::type::c_value()), "");

  static_assert((IsGreaterThanImpl<Int<5>, Int<4>>::type::c_value()), "");
  static_assert((!IsGreaterThanImpl<Int<4>, Int<4>>::type::c_value()), "");
  static_assert(!(IsGreaterThanImpl<Int<3>, Int<4>>::type::c_value()), "");

  typedef AddImpl<Int<3>, Int<2>>::type i5;
  static_assert((IsEqualImpl<i5, Int<5>>::type::c_value()), "");
}

void TestEvalBinaryOp() {
  static_assert((Eval<Add<Int<1>, Int<2>, Int<3>, Int<4>>>::type::c_value() == 10), "");
}

int main() {
  TestDict();
  TestEnv();
  TestBinaryOpImpl();
  TestEvalBinaryOp();
  return 0;
}