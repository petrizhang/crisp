/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CRISP_TEMPLATES_H
#define CRISP_TEMPLATES_H

#include "crisp.hpp"

// clang-format off
using crisp::Bool;
using crisp::Char;
using crisp::Int;
using crisp::Symbol;

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

using crisp::Eval;

#endif  //CRISP_TEMPLATES_H
