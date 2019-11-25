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

#include <cassert>
#include <iostream>
#include "CrispTemplateAPI.h"

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

void TestInterpBinaryOp() {
  static_assert((Interp<Add<Int<1>, Int<2>, Int<3>, Int<4>>>::type::c_value() == 10), "");
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

void TestQuoteMatchCase0() {
  using x = Var<'x'>;
  // Single-element templates test
  static_assert((QuoteMatchCase<Env<>, int, int>::matched), "");
  static_assert((QuoteMatchCase<Env<>, Array<>, Array<>>::matched), "");
  static_assert((!QuoteMatchCase<Env<>, Array<>, Array<Int<1>>>::matched), "");
  static_assert((!QuoteMatchCase<Env<>, Array<Int<2>>, Array<Int<1>>>::matched), "");
  static_assert((QuoteMatchCase<Env<>, Array<Int<1>>, Array<Int<1>>>::matched), "");

  // Capture test
  static_assert((QuoteMatchCase<Env<>, Array<Int<1>>, Array<Capture<_, x>>>::matched), "");
  using t0 = QuoteMatchCase<Env<>, Array<Int<1>>, Array<Capture<Int<1>, x>>>;
  static_assert((t0::matched), "");
  //  static_assert((std::is_same<typename EnvLookup<t0::env, x>::type, Quote<Int<1>>>::value), "");

  static_assert((!QuoteMatchCase<Env<>, Array<Int<1>>, Array<Capture<Int<2>, x>>>::matched), "");

  static_assert((QuoteMatchCase<Env<>, Capture<_, x>, Capture<_, x>>::matched), "");
  // static_assert((!QuoteMatchCase<Env<>, Capture<_, x>, Capture<x, x>>::matched), "");

  static_assert((QuoteMatchCase<Env<>, int, int>::matched), "");
}

void TestQuoteMatchCase1() {
  using x = Var<'x'>;
  static_assert((!QuoteMatchCase<Env<>, Array<Int<1>, Int<2>>, Array<Int<1>>>::matched), "");
  static_assert((QuoteMatchCase<Env<>, Array<Int<1>, Int<2>>, Array<Int<1>, Int<2>>>::matched), "");

  static_assert((QuoteMatchCase<Env<>,
                                Array<Array<Int<1>, Int<2>>>,
                                Array<Array<Int<1>, Int<2>>>>::matched),
                "");

  static_assert((QuoteMatchCase<Env<>,
                                Array<Array<Int<1>, Int<2>>>,
                                Array<Array<Int<1>, _>>>::matched),
                "");

  static_assert((QuoteMatchCase<Env<>,
                                Array<Array<Int<1>, Int<2>>>,
                                Array<Array<_, _>>>::matched),
                "");

  static_assert((!QuoteMatchCase<Env<>,
                                 Array<Array<Int<2>, Int<2>>>,
                                 Array<Array<Int<1>, Int<2>>>>::matched),
                "");

  // Capture test
  using t0 = QuoteMatchCase<Env<>, Array<Int<1>, Int<2>>, Array<Capture<Int<1>, x>, Int<2>>>;
  static_assert((t0::matched), "");
  //  static_assert((std::is_same<typename EnvLookup<t0::env, x>::type,
  //                              Quote<Int<1>>>::value),
  //                "");
}

void TestPatternMatch1() {
  using x = Var<'x'>;
  using t0 = typename Interp<Match<Quote<Add<Int<1>, Int<2>>>,
                                   Case<Add<Capture<_, x>, _>, x>,
                                   Default<Char<'1'>>>>::type;
  //  static_assert((std::is_same<t0, Quote<Int<1>>>::value), "");

  using t1 = typename Interp<
      Match<Quote<Add<Int<1>, Int<2>>>,
            Case<Sub<_, _>, Char<'1'>>,
            Case<Add<_, _>, Char<'2'>>,
            Default<Char<'3'>>>>::type;
  static_assert((std::is_same<t1, Char<'2'>>::value), "");
}

int main() {
  using x = Var<'x'>;
  using y = Var<'y'>;

  using t0 = typename Interp<
      Match<Quote<Add<Int<1>, Int<2>>>,
            Case<Add<Capture<_, x>, Capture<_, y>>, Add<Mul<x, y>, Sub<x, y>>>,
            Default<Char<'1'>>>>::type;

  return 0;
}