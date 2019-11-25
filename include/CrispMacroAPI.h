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

#include "interpreter/Interpreter.hpp"

using crisp::_;
// TODO using crisp::___;

#define v(x) crisp::Pack<decltype(x), x>::type
#define quote(e) crisp::Quote<e>
#define eval(expr) crisp::Eval<expr>
#define str(args...) crisp::String<args>
#define add(args...) crisp::Add<args>
#define sub(args...) crisp::Sub<args>
#define mul(args...) crisp::Mul<args>
#define mod(args...) crisp::Mod<args>
#define eq_(args...) crisp::IsEqual<args>
#define gt(args...) crisp::IsGreaterThan<args>
#define lt(args...) crisp::IsLessThan<args>
#define ge(args...) crisp::IsGreaterEqual<args>
#define le(args...) crisp::IsLessEqual<args>
#define or_(args...) crisp::Or<args>
#define and_(args...) crisp::And<args>
#define if_(cond, body, elseBody) crisp::If<cond, body, elseBody>
#define params(args...) crisp::ParamList<args>
#define var(args...) crisp::Var<args>
#define define(args...) crisp::Define<args>
#define lambda(args...) crisp::Lambda<args>
#define call(f, args...) crisp::Call<f, args>
#define block(args...) crisp::Block<args>
#define println(args...) crisp::Println<args>
#define match(args...) crisp::Match<args>
#define case_(C, E) crisp::Case<C, E>
#define default_(E) crisp::Default<E>
#define capture(T, V) crisp::Capture<T, V>
#define interp(expr) crisp::Interp<expr, crisp::Env<>>::type
#define run(expr) crisp::Interp<expr, crisp::Env<>>::Run()

#endif  //CRISP_MACROS_H
