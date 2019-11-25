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

#ifndef CRISP_LITERAL_HPP
#define CRISP_LITERAL_HPP
#include "Common.h"

namespace crisp {
using namespace ast;
using namespace util;

/// -------------------------------------------------------------------------------------------
/// Interpret value types.
template <typename Environ, bool V>
struct Interp<Bool<V>, Environ> {
  using env = Environ;
  using type = Bool<V>;
  static constexpr bool Run() { return V; }
};

template <typename Environ, char V>
struct Interp<Char<V>, Environ> {
  using env = Environ;
  using type = Char<V>;
  static constexpr char Run() { return V; }
};

template <typename Environ, int V>
struct Interp<Int<V>, Environ> {
  using env = Environ;
  using type = Int<V>;
  static constexpr int Run() { return V; }
};

template <typename Environ, char... chars>
struct Interp<String<chars...>, Environ> {
  using env = Environ;
  using type = String<chars...>;
  static std::string Run() { return type::c_value(); }
};
}  // namespace crisp

#endif  //CRISP_LITERAL_HPP
