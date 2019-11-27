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

#ifndef CRISP_INTERPRETLIST_HPP
#define CRISP_INTERPRETLIST_HPP
#include "Common.hpp"

namespace crisp {
using namespace ast;
using namespace util;
using util::internal::InternalList;

template <typename Environ, typename... Elements>
struct Interpret<List<Elements...>, Environ> {
  using ElementsInterp = Interpret<InternalList<Elements...>, Environ>;
  using ElementsValue = typename ElementsInterp::type;

  using env = Environ;
  using type = typename Convert<ElementsValue, List>::type;
  static auto Run() {
    return ElementsInterp::Run();
  }
};

}  // namespace crisp

#endif  //CRISP_INTERPRETLIST_HPP
