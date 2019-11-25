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
#include "CrispMacroAPI.h"

int main() {
  //-### Println
  {
    run(println());
    run(println(v(3)));
    run(println(v(5), v(true)));
  }
  //-### Basic value types
  {
    // int
    run(println(v(1)));
    // bool
    run(println(v(true)));
    // char
    run(println(v('c')));
    // string
    run(println(str('s', 'y', 'm', 'b', 'o', 'l')));
  }
  //-### Block
  {
    // 3
    run(println(block(v(1),
                      v(2),
                      v(3))));
  }
  //-### Basic operations
  {
    // 1+2 -> 3
    run(println(add(v(1), v(2))));
    // 1-2 -> -1
    run(println(sub(v(1), v(2))));
    // 1*2 -> 2
    run(println(mul(v(1), v(2))));
    // 10 % 3 -> 1
    run(println(mod(v(1), v(2))));
    // 1 == 2 -> false
    run(println(eq_(v(1), v(2))));
    // 1 > 2 -> false
    run(println(gt(v(1), v(2))));
    // 1 >= 2 -> false
    run(println(ge(v(1), v(2))));
    // 1 < 2 -> true
    run(println(lt(v(1), v(2))));
    // 1 <= 2 -> true
    run(println(le(v(1), v(2))));
    // true && false -> false
    run(println(and_(v(true), v(false))));
    // true || false -> true
    run(println(or_(v(true), v(false))));
  }
  //-### Variable definition
  {
    /*
       * int a = 100;
       * println(a);
       */
    run(block(define(var('a'), v(100)),
              var('a'),
              println(var('a'))));
  }
  //-### If-then-else
  {
    // true ? 1 : 2
    run(println(if_(v(true), v(1), v(2))));
  }
  //-### Function definition
  {
    run(block(define(var('+'), lambda(params(var('x'), var('y')),
                                      add(var('x'), var('y')))),
              define(var('s'), call(var('+'), v(10), v(20))),
              println(var('s'))));
  }
  //-### Closure
  {
    using x = var('x');
    using y = var('y');
    using add1 = var('a', 'd', 'd', '1');
    using add2 = var('a', 'd', 'd', '2');
    using makeAddX = var('m', 'a', 'k', 'e', 'A', 'd', 'd', 'X');

    // 11, 12
    run(block(define(makeAddX, lambda(params(y),
                                      lambda(params(x), add(x, y)))),
              define(add1, call(makeAddX, v(1))),
              define(add2, call(makeAddX, v(2))),
              println(call(add1, v(10))),    // 11
              println(call(add2, v(10)))));  //12
  }
  //-### Recursive Function
  {
    using n = var('n');
    using factorial = var('f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l');

    // 3628800
    run(block(define(factorial, lambda(params(n),
                                       if_(eq_(n, v(1)),
                                           v(1),
                                           mul(n, call(factorial, sub(n, v(1))))))),
              println(call(factorial, v(10)))));
  }
  //-### Quote, QuoteF, Unquote and Eval
  {
//    // quote
//    using quoted_add = quote(add(v(1), v(2)));
//    // eval
//    run(println(eval(quoted_add)));
//
//    // quotef
//    // e.g. quotef(f( args...))
//    // here `args` will be evaluated to `evaluatedArgs`
//    // and the result will be `quote(f(evaluatedArgs...))`
//    run(println(eval(quotef(mul(unquote(quoted_add), unquote(quoted_add))))));
  }
  //-### Pattern Match
  {
//    using x = var('x');
//    using y = var('y');
//
//    /*
//     * Add(1, 2) match {
//     *   case Sub(_, _) => '-'
//     *   case Add(_, _) => '0'
//     *   case _ => '?'
//     * }
//     *
//     * Note: In order to prevent Crisp interpreter's evaluation,
//     *       you need to quote an expression before match it.
//     * Here we quote the expression `add(v(1), v(2))` before match it.
//     */
//    // the result is '+'
//    run(println(match(quote(add(v(1), v(2))),
//                      case_(sub(_, _), v('-')),
//                      case_(add(_, _), v('+')),
//                      default_(v('?')))));
//
//    // Capture the first parameter of an `add` expression with `x`.
//    // Note: When match a quoted expression, the value of captured variables
//    //       are also quoted expressions.
//    // Here the expressioin `v(1)` is captured as `quote(v(1))`,
//    // thus we need use `unquote` to get the orginal AST `v(1)`.
//    run(println(match(quote(add(v(1), v(2))),
//                      case_(add(capture(_, x), _), unquote(x)),
//                      default_(v('?')))));
  }
}
