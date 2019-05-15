#include "crisp_templates.h"

int main() {
  Eval<Println<Add<Int<1>, Int<1>>>>::c_value();
  return 0;
}
