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
#include <type_traits>
#include "CrispMacroAPI.h"

class SafePrintf {
 private:
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
                                                               case_(v('d'), int),
                                                               case_(v('u'), unsigned),
                                                               case_(v('f'), double),
                                                               case_(v('c'), char),
                                                               case_(v('s'), const char *),
                                                               default_(v('-')))),
                                        if_(eq_(fmt_type, v('-')),
                                            call(check, fmt_tail, result, v(false)),
                                            call(check, fmt_tail, push_last(result, fmt_type), v(false)))),
                                  call(check, fmt_tail, result, eq_(fmt_head, v('%'))))))));

 public:
  template <typename Format, typename... Args>
  static void SafePrintfImpl(Args... args) {
    using GivenTypes = list(Args...);
    using ExpectedTypes =
        typename interpret(block(check_def,
                                 call(check, str2list(Format), list(), v(false))));

    static_assert(std::is_same<GivenTypes, ExpectedTypes>::value, "type mismatch");
    std::printf(Format::value, args...);
  }
};

#define safe_printf(fmt, args...) SafePrintf::SafePrintfImpl<str(fmt)>(args)

int main() {
  // These lines are OK
  safe_printf("Hello world\n");
  safe_printf("Hello world %% %d\n", 10);
  safe_printf("Hello world - %s %d - pi=%f\n", "Crisp", 2019, 3.141592654);

  // These lines will trigger compile errors
  // safe_printf("Hello world %d", "1");
  // safe_printf("Hello world %d");
  // safe_printf("Hello world %s", 1);

  return 0;
}