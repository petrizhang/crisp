/*
 * Copyright 2019 by Crisp Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License";
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

#include "crisp/MacroAPI.h"

int main() {
  //-### Basic value types
  {
    // int
    using result0 = interpret(v(1));
    // bool
    using result1 = interpret(v(true));
    // char
    using result2 = interpret(v('c'));
    // string
    using result3 = interpret(str("symbol"));
  }
  //-### Block
  {
    // 3
    using result = interpret(block(v(1), v(2), v(3)));
  }
  //-### Basic operations
  {
    // 1+2 -> 3
    using result0 = interpret(add(v(1), v(2)));
    // 1-2 -> -1
    using result1 = interpret(sub(v(1), v(2)));
    // 1*2 -> 2
    using result2 = interpret(mul(v(1), v(2)));
    // 10 % 3 -> 1
    using result3 = interpret(mod(v(1), v(2)));
    // 1 == 2 -> false
    using result4 = interpret(eq_(v(1), v(2)));
    // 1 > 2 -> false
    using result5 = interpret(gt(v(1), v(2)));
    // 1 >= 2 -> false
    using result6 = interpret(ge(v(1), v(2)));
    // 1 < 2 -> true
    using result7 = interpret(lt(v(1), v(2)));
    // 1 <= 2 -> true
    using result8 = interpret(le(v(1), v(2)));
    // true && false -> false
    using result9 = interpret(and_(v(true), v(false)));
    // true || false -> true
    using result10 = interpret(or_(v(true), v(false)));
  }
  //-### Variable definition
  {
    /*
     * int a = 100;
     * a;
     */
    using result = interpret(block(define(var("a"), v(100)), var("a")));
  }
  //-### If-then-else
  {
    // true ? 1 : 2
    using result = interpret(if_(v(true), v(1), v(2)));
  }
  //-### Function definition and call
  {
    using result = interpret(block(
        define(var("+"),
               lambda(params(var("x"), var("y")), add(var("x"), var("y")))),
        define(var("s"), call(var("+"), v(10), v(20))), var("s")));
  }
  //-### Closure
  {
    using x        = var("x");
    using y        = var("y");
    using add1     = var("add1");
    using add2     = var("add2");
    using makeAddX = var("makeAddX");

    // 11, 12
    using result = interpret(block(define(makeAddX,                 //
                                          lambda(params(y),         //
                                                 lambda(params(x),  //
                                                        add(x, y)))),
                                   define(add1, call(makeAddX, v(1))),
                                   define(add2, call(makeAddX, v(2))),
                                   call(add1, v(10)),    // 11
                                   call(add2, v(10))));  // 12
  }
  //-### Recursive Function
  {
    using n         = var("n");
    using factorial = var("factorial");

    // Int<3628800>
    using result = interpret(
        block(define(factorial,                                            //
                     lambda(params(n),                                     //
                            if_(eq_(n, v(1)),                              //
                                v(1),                                      //
                                mul(n, call(factorial, sub(n, v(1))))))),  //
              call(factorial, v(10))));

    static_assert(result::value == 3628800, "");
  }
  //-### Quote and Eval
  {
    // quote
    using quoted_add = quote(add(v(1), v(2)));
    // eval
    using result = interpret(eval(quoted_add));
  }
  //-### Pattern Match
  {
    using x = var("x");
    using y = var("y");

    /*
     * Add(1, 2) match {
     *   case Sub(_, _) => '-'
     *   case Add(_, _) => '0'
     *   case _ => '?'
     * }
     *
     * Note: In order to prevent Crisp using result  = interpreter's evaluation,
     *       you need to quote an expression before match it.
     * Here we quote the expression `add(v(1), v(2))` before match it.
     */
    // the result is '+'
    using result0 = interpret(match(quote(add(v(1), v(2))),    //
                                    case_(sub(_, _), v('-')),  //
                                    case_(add(_, _), v('+')),  //
                                    default_(v('?'))));

    // Capture the first parameter of an `add` expression with `x`.
    // Here the expression `v(1)` is captured as `v(1)`,
    using result1 = interpret(match(quote(add(v(1), v(2))),           //
                                    case_(add(capture(_, x), _), x),  //
                                    default_(v('?'))));
  }
}
