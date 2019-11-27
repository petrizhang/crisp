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

#include <cstdio>
#include <iostream>
#include <type_traits>
#include "CrispMacroAPI.h"

using std::printf;

struct SafePrintf {
};

int main() {
  using check = var("check");
  using fmt = var("fmt");
  using flag = var("flag");
  using fmt_head = var("fmt_head");
  using fmt_tail = var("fmt_tail");
  using result = var("result");
  using fmt_type = var("fmt_type");

  using check_func =
      define(check,
             lambda(params(fmt, result, flag),
                    if_(is_empty(fmt),
                        result,
                        block(define(fmt_head, head(fmt)),
                              define(fmt_tail, tail(fmt)),
                              if_(flag,
                                  block(define(fmt_type, match(fmt_head,
                                                               case_(v('d'), int),
                                                               case_(v('u'), unsigned),
                                                               case_(v('f'), double),
                                                               case_(v('c'), char),
                                                               case_(v('s'), const char *),
                                                               default_(nil))),
                                        if_(is_nil(fmt_type),
                                            call(check, fmt_tail, result, v(false)),
                                            call(check, fmt_tail, push_last(result, fmt_type), v(false)))),
                                  call(check, fmt_tail, result, eq_(fmt_head, v('%'))))))));

  using l = list(v('a'), v('b'), v('%'), v('s'), v('a'));
  using types = interpret(block(check_func,
                                call(check, l, list(), v(false))));
  return 0;
}