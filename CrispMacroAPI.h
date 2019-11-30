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

#ifndef CRISP_MACROS_H
#define CRISP_MACROS_H

#include "ast/CoreAST.hpp"
#include "ast/LibAST.hpp"
#include "core/Interpreter.hpp"

namespace crisp {
using PreDefinedEnv = Env<Dict<Pair<Map<_, _>::__name__, Closure<Env<>, Map<_, _>::__body__>>>>;
}

#ifdef CRISP_USER_DEFINED_ENV
#define interpret(expr) crisp::Interpret<expr, CRISP_USER_DEFINED_ENV>::type
#else
#define interpret(expr) crisp::Interpret<expr, crisp::PreDefinedEnv>::type
#endif

#define run(expr) crisp::Interpret<expr, crisp::Env<>>::Run()

#endif  //CRISP_MACROS_H
