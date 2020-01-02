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

#ifndef CRISP_TEMPLATEAPI_H
#define CRISP_TEMPLATEAPI_H

#include "crisp/ast/CoreAST.hpp"
#include "crisp/ast/LibAST.hpp"
#include "crisp/core/Interpreter.hpp"

// clang-format off
using crisp::_;
using crisp::___;
using crisp::operator""_s;
using crisp::operator""_v;

using crisp::Nil;
using crisp::Bool;
using crisp::Char;
using crisp::Int;
using crisp::Str;

using crisp::IsEmpty;
using crisp::IsNil;

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

using crisp::Cond;
using crisp::When;
using crisp::Else;

using crisp::Match;
using crisp::Capture;
using crisp::Case;
using crisp::Default;

using crisp::Str2List;
using crisp::List;
using crisp::Head;
using crisp::Tail;
using crisp::Concat;
using crisp::PushFront;
using crisp::PushBack;
using crisp::PopFront;
using crisp::PopBack;
using crisp::DropHead;
using crisp::DropLast;

using crisp::Quote;
using crisp::Eval;
using crisp::Interpret;

using crisp::Map;

#endif  //CRISP_TEMPLATEAPI_H
