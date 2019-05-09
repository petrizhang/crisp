#include <functional>
#include <iostream>
#include <ratio>
#include "tispp.hpp"

using namespace tispp;
template <typename L, typename R>
struct AddIm {
  static_assert(type_checker<L, R>::value,
                "Incompatible types for operation `Add`.");
};

template <int LV, int RV>
struct AddIm<Int<LV>, Int<RV>> {
  typedef Bool<(LV > RV)> type;
};
int main() {
  std::cout << "Hello, World!" << std::endl;

  std::cout << AndImpl<v(false), v(true)>::type::c_value();
  return 0;
}