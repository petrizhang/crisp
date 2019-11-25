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

#ifndef CRISP_TEMPLATES_H
#define CRISP_TEMPLATES_H

#include "interpreter/Interpreter.hpp"

// clang-format off
using crisp::Bool;
using crisp::Char;
using crisp::Int;
using crisp::String;

using crisp::Println;
using crisp::Add;
using crisp::Mul;
using crisp::Sub;
using crisp::Mod;

using crisp::And;
using crisp::Or;

using crisp::IsEqual;
using crisp::IsGreaterThan;
using crisp::IsLessThan;
using crisp::IsGreaterEqual;
using crisp::IsLessEqual;

using crisp::Var;
using crisp::ParamList;
using crisp::Lambda;
using crisp::Define;
using crisp::Call;
using crisp::Block;

using crisp::Match;
using crisp::Capture;
using crisp::Case;
using crisp::Default;

using crisp::Quote;

using crisp::Eval;

using crisp::Interpret;

#endif  //CRISP_TEMPLATES_H
