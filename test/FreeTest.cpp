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
#include <type_traits>

#include "crisp/util/Conditional.hpp"
#include "crisp/util/Defer.hpp"
#include "crisp/util/Dict.hpp"

using namespace crisp;
using namespace util;
using std::is_same;

int main() {
  std::cout << "a" << "\n";
  return 0;
}