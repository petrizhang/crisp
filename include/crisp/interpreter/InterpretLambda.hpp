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

#ifndef CRISP_INTERPRETLAMBDA_HPP
#define CRISP_INTERPRETLAMBDA_HPP
#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;

/**
 * Interpret an lambda expression.
 * @tparam Environ
 * @tparam Body
 * @tparam ParamL
 */
template <typename Environ, typename Body, typename ParamL>
struct Interpret<Lambda<ParamL, Body>, Environ> {
  using env = Environ;
  using type = Closure<Environ, Lambda<ParamL, Body>>;
};
}  // namespace crisp
#endif  //CRISP_INTERPRETLAMBDA_HPP
