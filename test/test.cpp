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

#include "crisp/TemplateAPI.h"

using namespace crisp;

void TestEnv() {
  using x = Var<'x'>;
  using y = Var<'y'>;
  using d0 = Dict<Pair<x, Int<1>>>;
  using d1 = Dict<Pair<x, Int<2>>, Pair<y, Int<3>>>;

  using e0 = Env<d0>;
  using e1 = EnvPushHead<e0, d1>::type;

  static_assert(EnvLookup<e0, x>::type::value == 1, "");
  static_assert(EnvLookup<e1, x>::type::value == 2, "");
  static_assert(EnvLookup<e1, y>::type::value == 3, "");

  using e2 = Env<>;
  using e3 = EnvPut<e2, x, Int<4>>::type;
  static_assert(EnvLookup<e3, x>::type::value == 4, "");
}

void TestBinaryOpImpl() {
  static_assert((AddImpl<Int<3>, Int<4>>::type::value == 7), "");
  static_assert((SubImpl<Int<100>, Int<4>>::type::value == 96), "");
  static_assert((MulImpl<Int<100>, Int<4>>::type::value == 400), "");
  static_assert((ModImpl<Int<101>, Int<4>>::type::value == 1), "");

  static_assert((IsGreaterEqualImpl<Int<5>, Int<4>>::type::value), "");
  static_assert((IsGreaterEqualImpl<Int<4>, Int<4>>::type::value), "");
  static_assert(!(IsGreaterEqualImpl<Int<3>, Int<4>>::type::value), "");

  static_assert((IsGreaterThanImpl<Int<5>, Int<4>>::type::value), "");
  static_assert((!IsGreaterThanImpl<Int<4>, Int<4>>::type::value), "");
  static_assert(!(IsGreaterThanImpl<Int<3>, Int<4>>::type::value), "");

  typedef AddImpl<Int<3>, Int<2>>::type i5;
  static_assert((IsEqualImpl<i5, Int<5>>::type::value), "");
}

void TestInterpBinaryOp() {
  static_assert((Interpret<Add<Int<1>, Int<2>, Int<3>, Int<4>>>::type::value == 10), "");
}

void TestConditionalApply() {
  using t1 = ConditionalApply<When<Bool<false>, DeferApply<Id, Int<1>>>,
                              Else<DeferApply<Id, Int<2>>>>;
  static_assert((std::is_same<t1::type, Int<2>>::value), "");

  using t2 = ConditionalApply<When<Bool<true>, DeferApply<Id, Int<1>>>,
                              Else<DeferApply<Id, Int<2>>>>;
  static_assert((std::is_same<t2::type, Int<1>>::value), "");

  using t3 = ConditionalApply<When<Bool<true>, DeferApply<Id, Int<1>>>,
                              When<Bool<true>, DeferApply<Id, Int<2>>>,
                              Else<DeferApply<Id, Int<3>>>>;
  static_assert((std::is_same<t3::type, Int<1>>::value), "");

  using t4 = ConditionalApply<When<Bool<false>, DeferApply<Id, Int<1>>>,
                              When<Bool<true>, DeferApply<Id, Int<2>>>,
                              Else<DeferApply<Id, Int<3>>>>;
  static_assert((std::is_same<t4::type, Int<2>>::value), "");

  using t5 = ConditionalApply<When<Bool<false>, DeferApply<Id, Int<1>>>,
                              When<Bool<false>, DeferApply<Id, Int<2>>>,
                              Else<DeferApply<Id, Int<3>>>>;
  static_assert((std::is_same<t5::type, Int<3>>::value), "");
};

void TestIsTemplateOf() {
  static_assert((IsValueTemplateOf<bool, Bool, Bool<true>>::value), "");
  static_assert((!IsValueTemplateOf<bool, Bool, Int<1>>::value), "");
}

int main() {
  using x = Var<'x'>;
  using y = Var<'y'>;

  using t0 = typename Interpret<
      Match<Quote<Add<Int<1>, Int<2>>>,
            Case<Add<Capture<_, x>, Capture<_, y>>, Add<Mul<x, y>, Sub<x, y>>>,
            Default<Char<'1'>>>>::type;

  return 0;
}