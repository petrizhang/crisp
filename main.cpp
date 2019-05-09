#include <functional>
#include <iostream>
#include <ratio>
#include "tispp.hpp"

using namespace tispp;

int main() {
  std::cout << "Hello, World!" << std::endl;

  std::cout << AddImpl<v(1), v(2), v(3)>::value << std::endl;
  std::cout << AddImpl<v(1), v(2), v(3)>::value << std::endl;

  return 0;
}