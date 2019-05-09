#include <functional>
#include <iostream>
#include "tispp.hpp"

using namespace tispp;

int main() {
  std::cout << "Hello, World!" << std::endl;

  typedef list(CarNode, v(1), v(2), v(3), v(4)) L;

  typedef car(cdr(L)) F;

  return 0;
}