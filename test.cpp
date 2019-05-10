#include <cassert>
#include <iostream>
#include "tispp.hpp"

using namespace tispp;

void TestDict() {
  using x = var('x');
  using y = var('y');
  using d = DictImpl<>;
  using d1 = DictPutImpl<d, x, Int<1>>::type;
  using d2 = DictPutImpl<d1, y, Int<2>>::type;
  using d3 = DictPutImpl<d2, x, Int<3>>::type;
  using d4 = DictPutImpl<d3, y, Int<4>>::type;

  static_assert(DictGetImpl<d1, x>::type::c_value() == 1, "");
  static_assert(DictGetImpl<d2, x>::type::c_value() == 1, "");
  static_assert(DictGetImpl<d2, y>::type::c_value() == 2, "");
  static_assert(DictGetImpl<d3, x>::type::c_value() == 3, "");
  static_assert(DictGetImpl<d4, y>::type::c_value() == 4, "");
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
  TestBinaryOpImpl();
  TestEvalBinaryOp();
  return 0;
}