#include <cassert>
#include <iostream>
#include "tispp.hpp"

using namespace tispp;

void test_binary_op() {
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
}

int main() {
  test_binary_op();
  return 0;
}