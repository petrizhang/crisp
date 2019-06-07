# Crisp
Crisp: ***C***ompile Time Interprete***r*** for L***isp*** Written in C++ Templates

## About Crisp
Crisp is a lisp-like and turing-complete DSL built upon C++ templates.
Users could consider Crisp as a lisp interpreter that interprets lisp programs at C++ compile time.

With Crisp, users could easily build compile-time programs by an extremely friendly syntax,
rather than manually writing tedious C++ templates. 

Just like lisp, Crisp is a simple but powerful functional programming language. The core features of Crisp
include **Immutable variable**, **Lambda**, **Lexical Scope**, **Closure**, **Patten Match**, and **Recursive Function**.

What's more, **all operations in Crisp are accomplished at compile time**. It is the most awesome feature of Crisp.

## How to Use
First let us see two simple but complete examples:

1. Crisp Macro Style
```cpp
#include "crisp_macros.h"

int main() {
  // The `println` part of the expression is evaluated at runtime rather than at compile time. 
  // To strictly use compile time evaluation:
  //   - use `using Result = eval(add(v(1),v(1)));` to get the evaluation result type
  //   - use `constexpr auto value = Result::c_value();` to get the constexpr result value
  run(println(add(v(1), v(1))));
  return 0;
}
```

2. Crisp Template Style
```cpp
#include "crisp_templates.h"

int main() {
  // The `Println` part of the expression is evaluated at runtime rather than at compile time.  
  // To strictly use compile time evaluation:
  //   - use `using Result = Eval<Add<Int<1>, Int<1>>>::type;` to get the result type
  //   - use `constexpr auto value = Result::c_value();` to get the constexpr result value
  Eval<Println<Add<Int<1>, Int<1>>>>::Run();
  return 0;
}
```

As demonstrated above, Crisp provides two kinds of syntax: macro style and template style. You could build Crisp program with your preferred one. But you should **never mix these two styles together**, as it will lead to unexpected behaviors.
For instance, in C++, `println(Var<'a', 'b'>)` will be recognized as `println(` `Var<'a'`, `'b'>` `)`, which will cause a compile error. 

Thus the best practice to use Crisp is **always using one style in a single C++ source file**. 

Generally, to express the same program, the macro style will be shorter, more readable 
and more friendly to IDE highlighting. Thus **we recommend the macro style**. 

### Use Crisp in Your Project
Crisp is a header-only library, users could use Crisp by including Crisp header files in their projects.

1. Copy header files located at `crisp/include` into your project.
2. Choose one style of API you prefer.
    - To use Crisp macro api, add `#include "crisp_macros.h"` to your c++ source code.
    - To use Crisp template api, add`#include "crisp_templates.h"` to c++ source code.
3. Enjoy it.

Notes:

1. Your compiler must support C++11.
2. Remember to set the `-ftemplate-depth-5000` flag to avoid insufficient template expansion depth.
3. You'd better use Crisp headers as the last ones of your includes. For that Crisp macros may potentially have the same names as symbols in other source files and cause compile errors.
4. As mentioned above: **never mix macros and templates style together**.  

## Examples
### Println
```cpp
    // print an empty line
    run(println());
    // 3
    run(println(v(3)));
    // 5 true
    run(println(v(5), v(true)));
```
 
### Basic value types
```cpp
    // int
    run(println(v(1)));
    // bool
    run(println(v(true)));
    // char
    run(println(v('c')));
    // symbol (equivalent string in other languages)
    run(println(str('s', 'y', 'm', 'b', 'o', 'l')));
```
### Block
A block contains a sequence of expressions, and the result is also an expression. 
The value of the block is the value of the last expression.
```cpp
    // 3
    run(println(block(v(1),
                      v(2),
                      v(3))));
```

### Basic operations
```cpp
    // 1+2 = 3
    run(println(add(v(1), v(2))));
    // 1-2 = -1
    run(println(sub(v(1), v(2))));
    // 1*2 = 2
    run(println(mul(v(1), v(2))));
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
```
### Variable definition
```cpp
    /*
     * int a = 100;
     * println(a);
     */
    run(block(define(var('a'), v(100)),
              var('a'),
              println(var('a'))));
```
### If-then-else
```cpp
    // true ? 1 : 2
    run(println(if_(v(true), v(1), v(2))));
```
### Function Call
```cpp
    run(block(define(var('+'), lambda(params(var('x'), var('y')),
                                      add(var('x'), var('y')))),
              define(var('s'), call(var('+'), v(10), v(20))),
              println(var('s'))));
```
### Closure
```cpp
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
```

In this example, the function `makeAddX` captures it's parameter `x` and returns another function.
The returned function takes a parameter `y` and returns the sum of `y` and the captured `x`.
We could find that calling `add1(10)` we will get `11` and calling `add2(10)` we will get `12`. 
 
### Recursive Function
```cpp
    using n = var('n');
    using factorial = var('f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l');

    // 3628800
    run(block(define(factorial, lambda(params(n),
                                       if_(eq_(n, v(1)),
                                           v(1),
                                           mul(n, call(factorial, sub(n, v(1))))))),
              println(call(factorial, v(10)))));
```

In this example, we define a recursive function `factorial` which calls itself to calculate the product from 1 to `n`.

## API

### Macro Style
Users could find a complete macro api list at "include/crisp_macros.h".
  
### Template Style
- `Nil`: the Nil value is similar to null in other languages.
- `Bool<B>`: a boolean value. e.g. `Bool<true>, Bool<false>`
- `Char<C>`: a char value. e.g. `Char<'a'>, Char<'b'>`
- `Int<N>` : an integer value. e.g. `Int<5>`
- `Var<'c','c',...>`: a symbol/variable reference. e.g. `Var<'a'>, Var<'t','e','s','t'>`
- `Pair<L,R>`: a pair value, it is very similar to `std::pair` in C++
- `Lambda< ParamList< Var<..>, Var<..>... >, Body >`, e.g. :
```cpp
Lambda< ParamList<Var<'x'>,Var<'y'>>,
    Add<Var<'x'>, Var<'y'>> >
```
- `Call`: calling user defined functions 
- ...

Users could find a complete template api list at "include/crisp_templates.h".

## Build and Dependency
If you want to explore Crisp source code and run Crisp examples and tests, you need to:

1. Prepare "cmake" and "make" and a C++ compiler supports C++11 in your system.
2. Clone this project.
2. In the project root directory, run commands:
```
cmake .
make
```

## Acknowledgements
This project is inspired by [TemplatedPL](https://github.com/Cheukyin/TemplatedPL). Thanks for the author's great idea.

The basic api of Crisp and TemplatedPL are quite similar. But there are still many differences between them:
1. Crisp supports recursive functions but TemplatedPL doesn't.
2. TemplatedPL supports call/cc but Crisp doesn't.
3. ...

In fact, the code structure of Crisp and TemplatedPL are **completely different**,
i.e., Crisp is a fully clean-room implementation.