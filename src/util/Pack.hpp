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

#ifndef CRISP_PACK_HPP
#define CRISP_PACK_HPP

#include "ast/CoreAST.hpp"

namespace util {
using crisp::Bool;
using crisp::Char;
using crisp::Int;

/**
 * Pack a const value(int/char/bool) into Value type.
 * e.g. PackToValue<int, 1>::packed_type will be Value<Int<1>>.
 * @tparam T
 * @tparam V
 */
template <typename T, T V>
struct Pack;

template <bool V>
struct Pack<bool, V> {
  using type = Bool<V>;
};

template <char V>
struct Pack<char, V> {
  using type = Char<V>;
};

template <int V>
struct Pack<int, V> {
  using type = Int<V>;
};

}  // namespace util

#endif  //CRISP_PACK_HPP
