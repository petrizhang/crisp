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

#ifndef CRISP_INTERPRETLITERAL_HPP
#define CRISP_INTERPRETLITERAL_HPP
#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

template <typename Environ>
struct Interpret<Nil, Environ> {
  using env = Environ;
  using type = Nil;
};

template <typename Environ, bool V>
struct Interpret<Bool<V>, Environ> {
  using env = Environ;
  using type = Bool<V>;
};

template <typename Environ, char V>
struct Interpret<Char<V>, Environ> {
  using env = Environ;
  using type = Char<V>;
};

template <typename Environ, int V>
struct Interpret<Int<V>, Environ> {
  using env = Environ;
  using type = Int<V>;
};

template <typename Environ, char... chars>
struct Interpret<Str<chars...>, Environ> {
  using env = Environ;
  using type = Str<chars...>;
};

}  // namespace crisp

#endif  //CRISP_INTERPRETLITERAL_HPP
