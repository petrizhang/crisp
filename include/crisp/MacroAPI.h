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

#ifndef CRISP_MACROAPI_H
#define CRISP_MACROAPI_H

#include "crisp/ast/CoreAST.hpp"
#include "crisp/ast/LibAST.hpp"
#include "crisp/interpreter/Interpreter.hpp"

using crisp::_;
using crisp::operator""_s;
using crisp::operator""_v;
using crisp::___;

#define nil crisp::Nil
#define v(x) crisp::Pack<decltype(x), x>::type
#define str(x) decltype(x##_s)

#define is_empty(l) crisp::IsEmpty<l>
#define is_nil(x) crisp::IsNil<x>

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

#define quote(e) crisp::Quote<e>
#define eval(expr) crisp::Eval<expr>

#define var(x) decltype(x##_v)
#define define(args...) crisp::Define<args>

#define lambda(args...) crisp::Lambda<args>
#define call(f, args...) crisp::Call<f, args>
#define block(args...) crisp::Block<args>

#define cond(args...) crisp::Cond<args>
#define when(args...) crisp::When<args>
#define else_(args...) crisp::Else<args>

#define match(args...) crisp::Match<args>
#define case_(C, E) crisp::Case<C, E>
#define default_(E) crisp::Default<E>
#define capture(T, V) crisp::Capture<T, V>

#define str2list(s) crisp::Str2List<s>
#define list(args...) crisp::List<args>
#define head(l) crisp::Head<l>
#define tail(l) crisp::Tail<l>
#define concat(l1, l2) crsip::Concat<l1, l2>
#define push_head(l, e) crisp::PushFront<l, e>
#define push_back(l, e) crisp::PushBack<l, e>
#define pop_front(l) crisp::PopHead<l>
#define pop_back(l) crisp::PopBack<l>
#define drop_head(l) crisp::DropHead<l>
#define drop_last(l) crisp::DropLast<l>

#define map(l, f) crisp::Map<l, f>

namespace crisp {
using PreDefinedEnv = Env<Dict<Pair<Map<_, _>::__name__, Closure<Env<>, Map<_, _>::__body__>>>>;
}

#ifdef CRISP_USER_DEFINED_ENV
#define interpret(expr) crisp::Interpret<expr, CRISP_USER_DEFINED_ENV>::type
#else
#define interpret(expr) crisp::Interpret<expr, crisp::PreDefinedEnv>::type
#endif

#endif  //CRISP_MACROAPI_H
