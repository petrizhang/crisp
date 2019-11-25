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

#ifndef CRISP_SIZE_HPP
#define CRISP_SIZE_HPP

#include <cstdint>

namespace util {
using std::uint64_t;

/**
 * Get size of given `Args`.
 * @tparam Args
 */
template <typename... Args>
struct Size;

template <>
struct Size<> {
  static const uint64_t value = 0;
};

template <typename Head, typename... Tails>
struct Size<Head, Tails...> {
  static const uint64_t value = 1 + Size<Tails...>::value;
};

}  // namespace util

#endif  //CRISP_SIZE_HPP
