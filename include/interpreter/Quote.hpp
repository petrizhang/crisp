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

#ifndef CRISP_QUOTE_HPP
#define CRISP_QUOTE_HPP
#include "Common.hpp"

namespace crisp {
using namespace ast;
using namespace util;

/// -------------------------------------------------------------------------------------------
/// Interpret quote expression.
template <typename Environ, typename AST>
struct Interp<Quote<AST>, Environ> {
using env = Environ;
using type = AST;

static decltype(type::c_value()) Run() { return type::c_value(); }
};
}
#endif  //CRISP_QUOTE_HPP
