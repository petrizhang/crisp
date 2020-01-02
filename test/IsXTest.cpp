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

#include "crisp/core/Interpreter.hpp"
#include "crisp/dump/DumpToLisp.hpp"
using namespace crisp;

int main() {
  static_assert(Interpret<IsBool<Bool<true>>>::type::value, "");
  static_assert(Interpret<IsInt<Int<1>>>::type::value, "");
  static_assert(Interpret<IsChar<Char<'c'>>>::type::value, "");
  static_assert(Interpret<IsStr<Str<>>>::type::value, "");
  static_assert(Interpret<IsStr<Str<'a', 'b'>>>::type::value, "");
  static_assert(Interpret<IsVar<Quote<Var<>>>>::type::value, "");
  static_assert(Interpret<IsVar<Quote<Var<'a', 'b'>>>>::type::value, "");
  return 0;
}
