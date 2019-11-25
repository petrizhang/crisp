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

#ifndef CRISP_PRINTLN_HPP
#define CRISP_PRINTLN_HPP
#include "Common.h"

namespace crisp {
using namespace ast;
using namespace util;
/// -------------------------------------------------------------------------------------------
/// Interpret println
template <typename Environ, typename Head, typename... Args>
struct Interp<Println<Head, Args...>, Environ> {
  static const char *Run() {
    auto value = Interp<Head, Environ>::Run();
    output(value);
    std::cout << " ";
    Interp<Println<Args...>, Environ>::Run();
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ, typename Head>
struct Interp<Println<Head>, Environ> {
  static const char *Run() {
    auto value = Interp<Head, Environ>::Run();
    output(value);
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

template <typename Environ>
struct Interp<Println<>, Environ> {
  static const char *Run() {
    std::cout << std::endl;
    return "#undefined";
  };

  using env = Environ;
  using type = Undefined;
};

}  // namespace crisp
#endif  //CRISP_PRINTLN_HPP
