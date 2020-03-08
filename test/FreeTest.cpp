#include <iostream>
#include <type_traits>

#include "crisp/base/lambda.hpp"

int main() {
  using namespace crisp::base;
  using add1  = lambda<add_<_1, Int<1>>>;
  const int a = add1::apply<Int<2>>::type::value;
  static_assert(a == 3, "");
  return 0;
}