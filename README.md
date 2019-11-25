# Crisp
Crisp: ***C***ompile Time Interprete***r*** for L***isp*** Written in C++ Templates

![language](https://img.shields.io/badge/language-c++-orange.svg)
![license](https://img.shields.io/badge/license-Apache2-green.svg)
![platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)
![std](https://img.shields.io/badge/c++->=11-blue.svg)

## About Crisp
You may have been told that **C++ templates are Turing-complete**.
It means you could do anything that a general programming language can do 
with C++ templates at **compile time**.

Yes, C++ templates are powerful enough to build arbitrarily complex programs. 
But the tedious syntax and obscure semantic of C++ prevent people to do such thing.   

Now, Crisp brings a **convenient, flexible and elegant** way for people to play with C++ templates and build
compile-time programs.

In short, Crisp is a lisp-like and Turing-complete DSL built upon C++ templates.
It could be considered as a interpreter that interprets lisp programs at C++ compile time. 

Crisp is a simple but powerful functional programming language. The core features of Crisp include:

- [x] **Variable**
- [x] **Lambda**
- [x] **Lexical Closure**
- [ ] **Fractional Number**
- [x] **Recursive Function**
- [ ] **Rich Collection Operations**
- [x] **Quote**
- [x] **Pattern Match**
- [x] **Code as Data**

## Core Idea

### Lisp
[Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language) is a family of computer programming languages with a long history.
All program code of Lisp is written as **s-expressions**, or parenthesized lists. 
A function call or syntactic form is written as a list with the function or operator's name first, 
and the arguments following; for instance, a function f that takes three arguments would be called as `(f arg1 arg2 arg3)`.

Lisp code is actually a list that forms a tree structure, i.e., the abstract syntax tree (AST) of a program.
For example: `((1+2)*(3+4))` in Lisp will be:
```lisp
(* (+ 1 2)
   (+ 3 4))
```

This gives users a chance to process code as data(a list), which makes meta-programming easier.

### Crisp
The two core features makes Lisp elegant are:
1. Lisp exposes the AST structure(lists) directly to users.
2. Lisp provides tools(list process functions) for users to easily process AST(lists).

#### Feature 1
We found that feature 1 is natural in C++ templates, 
because C++ templates are very similar to lists and 
could also be considered as tree structures.

Consider this example:

```lisp
(* (+ 1 2)
   (+ 3 4))
```

To express this program, users could write C++ templates as:

```cpp
Mul< Add<1, 2>,
     Add<3, 4>>;
```

Similarly to **s-expressions**, this kind of expressions are called **m-expressions**, 
which could form the program AST.

Then we could use C++ template meta-programming techniques to write a interpreter that
interprets these kinds of AST at compile time.

#### Feature 2
We have **nested templates as AST** now, but C++ doesn't provide convenient tools for users to process nested templates.
It is very hard to modifies a deep level of a nested template and transform it to another one.

Crisp brings the novel **pattern match** feature for users to process nested templates,
which makes it dramatically easy to process Crisp program code just like Lisp.   

## Getting Started
Let's start with two simple but complete examples.

1. Crisp Macro Style
```cpp
#include "CrispMacroAPI.h"

int main() {
  using result = interpret(add(v(1), v(1)));
  constexpr int value = result::value;
  // `value` is a compile-time constant!
  int a[value] = {0, 1};
  return 0;
}
```

2. Crisp Template Style
```cpp
#include "CrispTemplateAPI.h"

int main() {
  using result = Interpret<Add<Int<1>, Int<1>>>::type;
  constexpr int value = result::value;
  // `value` is a compile-time constant!
  int a[value] = {0, 1};
  return 0;
}
```

As demonstrated above, Crisp provides two kinds of syntax: macro style and template style. You could build Crisp programs with your preferred one. 

Generally, to express the same program, the macro style will be shorter, more readable and more friendly to IDE highlighting. Thus **we recommend the macro style**.  

## More Examples
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
    // string
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

### Quote, QuoteF, Unquote and Eval
```cpp
    // quote
    using quoted_add = quote(add(v(1), v(2)));
    // eval
    run(println(eval(quoted_add)));

    // quotef
    // e.g. quotef(f( args...))
    // here `args` will be evaluated to `evaluatedArgs`
    // and the result will be `quote(f(evaluatedArgs...))`
    run(println(eval(quotef(mul(unquote(quoted_add), unquote(quoted_add))))));
```

### Pattern Match
```cpp
    using x = var('x');
    using y = var('y');

    /*
     * Add(1, 2) match {
     *   case Sub(_, _) => '-'
     *   case Add(_, _) => '0'
     *   case _ => '?'
     * }
     *
     * Note: In order to prevent Crisp interpreter's evaluation,
     *       you need to quote an expression before match it.
     * Here we quote the expression `add(v(1), v(2))` before match it.
     */
    // the result is '+'
    run(println(match(quote(add(v(1), v(2))),
                      case_(sub(_, _), v('-')),
                      case_(add(_, _), v('+')),
                      default_(v('?')))));

    // Capture the first parameter of an `add` expression with `x`.
    // Note: When match a quoted expression, the value of captured variables
    //       are also quoted expressions.
    // Here the expressioin `v(1)` is captured as `quote(v(1))`,
    // thus we need use `unquote` to get the orginal AST `v(1)`.
    run(println(match(quote(add(v(1), v(2))),
                      case_(add(capture(_, x), _), unquote(x)),
                      default_(v('?')))));
```
## How to Use

### Use Crisp in Your Project
Crisp is a header-only library, users could use Crisp by including Crisp header files in their projects.

1. **Copy `crisp/include` into your project**.
2. 
    - To use **Crisp macro api**, **`#include "crisp_macros.h"`**.
    - To use **Crisp template api**, **`#include "crisp_templates.h"`**.
3. **Enjoy it**.

### Notes

1. Your compiler must support **C++11**.
2. Remember to set the **`-ftemplate-depth-5000`** flag to avoid insufficient template expansion depth.
3. You'd better **use Crisp headers as the last ones of your includes**. For that Crisp macros may potentially have the same names as symbols in other source files and cause compile errors.
4. **Never mix macros and templates style together**. In C++, expressions like `println(Var<'a', 'b'>)` will be recognized as `println(` `Var<'a'`, `'b'>` `)`, which will cause a compile error. Thus the best practice to use Crisp is **always using one style in a single C++ source file**. 


## API

### Macro Style
Users could find a complete macro api list at [include/crisp_macros.h](https://github.com/pzque/crisp/blob/master/include/crisp_macros.h).
  
### Template Style
Users could find a complete template api list at [include/crisp_templates.h](https://github.com/pzque/crisp/blob/master/include/crisp_templates.h)..

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
This project is inspired by [TemplatedPL](https://github.com/Cheukyin/TemplatedPL). Thanks to the author's great idea.

Some basic API of Crisp and TemplatedPL look similar, but these 2 projects are implemented with **completly different techniques**.
