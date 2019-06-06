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

#include "crisp_macros.h"

template <typename... Args>
struct M {};

template <typename T = int>
struct N {};

int main() {
  using namespace crisp;
  using type = InternalIf<true,
                          M<>,
                          Array<Int<1>, Int<2>>,
                          N<>,
                          Array<Int<3>, Int<4>>>::type;
  return 0;
}