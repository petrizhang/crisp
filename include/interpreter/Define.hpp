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

#ifndef CRISP_DEFINE_HPP
#define CRISP_DEFINE_HPP
#include "Common.h"

namespace crisp {
using namespace ast;
using namespace util;

/// -------------------------------------------------------------------------------------------
/// Interpret variable definition. e.g. Define<Var<'a'>,Int<1>>
template <typename Environ, typename Ident, typename Value>
struct Interp<Define<Ident, Value>, Environ> {
  using ValueInterp = Interp<Value, Environ>;

  using env = typename EnvPut<Environ, Ident, typename ValueInterp::type>::type;
  using type = Undefined;

  static decltype(type::c_value()) Run() {
    ValueInterp::Run();
    return type::c_value();
  }
};
}  // namespace crisp
#endif  //CRISP_DEFINE_HPP
