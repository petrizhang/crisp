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

#include <ast/CoreAST.hpp>
#include <iostream>
#include <type_traits>
#include "util/Dict.hpp"

using std::is_same;

using namespace crisp;
using namespace util;

int main() {
  /*
     * DictPut tests
     */
  {
    using d0 = Dict<Pair<int, char>, Pair<char, int>>;
    using d1 = DictPutImpl<d0, int, bool>::type;
    static_assert(is_same<d1, Dict<Pair<int, bool>, Pair<char, int>>>::value, "");

    using d2 = Dict<>;
    using d3 = DictPutImpl<d2, int, bool>::type;
    static_assert(is_same<d3, Dict<Pair<int, bool>>>::value, "");

    using d4 = DictPutImpl<d0, char, bool>::type;
    static_assert(is_same<d4, Dict<Pair<int, char>, Pair<char, bool>>>::value, "");

    using d5 = DictPutImpl<d0, bool, float>::type;
    static_assert(is_same<d5, Dict<Pair<bool, float>, Pair<int, char>, Pair<char, int>>>::value, "");
  }

  /*
     * DictGet tests
     */
  {
    using d0 = Dict<Pair<int, char>, Pair<char, int>>;
    using v0 = DictGetImpl<d0, int>::type;
    static_assert(is_same<char, v0>::value, "");

    using v1 = DictGetImpl<d0, float>::type;
    static_assert(is_same<Undefined, v1>::value, "");
  }
  return 0;
}