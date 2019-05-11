#include <cassert>
#include <iostream>
#include "tispp.hpp"

using namespace tispp;

void TestDict() {
  using x = var('x');
  using y = var('y');
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
  using x = var('x');
  using y = var('y');
  using d0 = Dict<Pair<x, v(1)>>;
  using d1 = Dict<Pair<x, v(2)>, Pair<y, v(3)>>;

  using e0 = Env<d0>;
  using e1 = EnvPushFront<e0, d1>::type;

  static_assert(EnvLookup<e0, x>::type::c_value() == 1, "");
  static_assert(EnvLookup<e1, x>::type::c_value() == 2, "");
  static_assert(EnvLookup<e1, y>::type::c_value() == 3, "");

  using e2 = Env<>;
  using e3 = EnvPut<e2, x, v(4)>::type;
  static_assert(EnvLookup<e3, x>::type::c_value() == 4, "");
}

void TestBinaryOpImpl() {
  static_assert((AddImpl<v(3), v(4)>::type::c_value() == 7), "");
  static_assert((SubImpl<v(100), v(4)>::type::c_value() == 96), "");
  static_assert((MulImpl<v(100), v(4)>::type::c_value() == 400), "");
  static_assert((ModImpl<v(101), v(4)>::type::c_value() == 1), "");

  static_assert((IsGreaterEqualImpl<v(5), v(4)>::type::c_value()), "");
  static_assert((IsGreaterEqualImpl<v(4), v(4)>::type::c_value()), "");
  static_assert(!(IsGreaterEqualImpl<v(3), v(4)>::type::c_value()), "");

  static_assert((IsGreaterThanImpl<v(5), v(4)>::type::c_value()), "");
  static_assert((!IsGreaterThanImpl<v(4), v(4)>::type::c_value()), "");
  static_assert(!(IsGreaterThanImpl<v(3), v(4)>::type::c_value()), "");

  typedef AddImpl<v(3), v(2)>::type i5;
  static_assert((IsEqualImpl<i5, Int<5>>::type::c_value()), "");
}

void TestEvalBinaryOp() {
  static_assert((Eval<Add<v(1), v(2), v(3), v(4)>>::type::c_value() == 10), "");
}

int main() {
  TestDict();
  TestEnv();
  TestBinaryOpImpl();
  TestEvalBinaryOp();
  return 0;
}