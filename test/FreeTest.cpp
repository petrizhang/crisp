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

#include <iostream>
#include <type_traits>

#include "crisp/dump/DumpToLisp.hpp"

// clang-format off
#include "crisp/MacroAPI.h"
// clang-format on
using check = var("check");
using fmt = var("fmt");
using flag = var("flag");
using fmt_head = var("fmt_head");
using fmt_tail = var("fmt_tail");
using result = var("result");
using fmt_type = var("fmt_type");

using check_def =
    define(check,
           lambda(params(fmt, result, flag),
                  if_(is_empty(fmt),
                      result,
                      block(define(fmt_head, head(fmt)),
                            define(fmt_tail, tail(fmt)),
                            if_(flag,
                                block(define(fmt_type, match(fmt_head,
                                                             case_(v('d'), v(1)),
                                                             case_(v('u'), v(1)),
                                                             case_(v('f'), v(1)),
                                                             case_(v('c'), v(1)),
                                                             case_(v('s'), v(1)),
                                                             default_(nil))),
                                      if_(is_nil(fmt_type),
                                          call(check, fmt_tail, result, v(false)),
                                          call(check, fmt_tail, push_back(result, fmt_type), v(false)))),
                                call(check, fmt_tail, result, eq_(fmt_head, v('%'))))))));
using namespace crisp;
int main() {
  DumpToLisp<check_def>::dump(std::cout, 4);
  return 0;
}