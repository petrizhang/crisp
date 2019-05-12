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
#ifndef TISPP_MACROS_H
#define TISPP_MACROS_H

#include "tispp.hpp"

#define v(x) tispp::PackToType<decltype(x), x>::type
#define add(args...) tispp::Add<args>
#define sub(args...) tispp::Sub<args>
#define mul(args...) tispp::Mul<args>
#define mod(args...) tispp::Mod<args>
#define eq_(args...) tispp::IsEqual<args>
#define gt(args...) tispp::IsGreaterThan<args>
#define lt(args...) tispp::IsLessThan<args>
#define ge(args...) tispp::IsGreatEqual<args>
#define le(args...) tispp::IsLessEqual<args>
#define or_(args...) tispp::Or<args>
#define and_(args...) tispp::And<args>
#define if_(cond, body, elseBody) tispp::If<cond, body, elseBody>
#define params(args...) tispp::ParamList<args>
#define var(args...) tispp::Var<args>
#define define(args...) tispp::Define<args>
#define lambda(args...) tispp::Lambda<args>
#define call(f, args...) tispp::Call<f, args>
#define seq(args...) tispp::Seq<args>
#define eval(expr) tispp::Eval<expr, tispp::Env<>>

#endif  //TISPP_MACROS_H
