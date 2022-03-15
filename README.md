# Crisp
Crisp: ***C***ompile-Time Interprete***r*** for L***isp***. A header-only library implemented with C++ templates.

See a demo [safe_printf](https://github.com/pzque/crisp/blob/master/demos/safe_printf.cpp) which can validate format string at compile time with Crisp.

![language](https://img.shields.io/badge/language-c++-orange.svg)
![license](https://img.shields.io/badge/license-Apache2-green.svg)
![platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)
![std](https://img.shields.io/badge/c++->=14-blue.svg)

## About Crisp
You may have been told that **C++ templates are Turing-complete**.
It means you could do anything that a general programming language can do 
with C++ templates at **compile time**.

Yes, C++ templates are powerful enough to build arbitrarily complex programs. 
But the tedious syntax and obscure semantic of C++ prevent people to do so.   

Now, Crisp brings a **convenient, flexible and elegant** way for users to play with C++ templates and build
compile-time programs.

In short, Crisp is a Lisp-like and Turing-complete DSL built upon C++ templates.
It could be considered as a interpreter that interprets Lisp-like programs at C++ compile time. 

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
[Lisp](https://en.wikipedia.org/wiki/Lisp_(programming_language)) is a family of computer programming languages with a long history.

All program code of Lisp is written as **s-expressions**, or parenthesized lists. 

A function call or syntactic form is written as a list with the function or operator's name first, 
and the arguments following; for instance, a function f that takes three arguments would be called as `(f arg1 arg2 arg3)`.

Lisp code is actually a list that forms a tree structure, i.e., the abstract syntax tree (AST) of a program.
For example: `((1+2)*(3+4))` in Lisp will be:
```lisp
(* (+ 1 2)
   (+ 3 4))
```

This gives users a chance to process code as data(lists), which makes meta-programming easier.

### Crisp
In conclusion, the two most important features make Lisp elegant are:
1. Lisp exposes the AST structure(lists) directly to users.
2. Lisp provides tools(macros and list processing functions) for users to easily access and process AST(lists).

#### Feature 1
We found that feature 1 is natural in C++ templates, because nested C++ templates are very similar to lists and could also be considered as tree structures.

Consider this example:

```lisp
(* (+ 1 2)
   (+ 3 4))
```

To express the same program, users could write C++ templates as:

```cpp
Mul< Add<1, 2>,
     Add<3, 4>>;
```

Similarly to **s-expressions**, this kind of expressions are called **m-expressions**, 
which could also be treated as a tree structure(AST) that encodes a program.

Then we could use C++ template meta-programming techniques to write a interpreter that
interprets this kind of AST at compile time,
as the [Crisp interpreter](https://github.com/pzque/crisp/tree/master/core) does.

#### Feature 2
We have **nested templates as AST** now, but C++ doesn't provide convenient tools for users to process nested templates.
It is very hard to modifies a deep level of a nested template and transform it to another one.

Crisp brings the novel **pattern match** feature for users to process such nested templates.
Due to that, processing program code in Crisp is dramatically easy. 

Here is a simple example demonstrates how to transform an addition expression to a multiplication expression through a function: 

```cpp
  block(define(var("add2mul"), lambda(params(var("x")),
                                      match(var("x"),
                                            case_(add(var("left"), var("right")),
                                                  mul(var("left"), var("right"))),
                                            default_(var("x"))))),
        call(var("add2mul"), quote(add(v(1), v(2)))));
``` 

Crisp supports Lisp-style macros as well. Further more, Crisp provides the **plugin** system which allows users to
freely modify the program AST before it being interpreted. These advance topics will be discussed later.  

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
### Basic value types
```cpp
    // int
    interpret(v(1));
    // bool
    interpret(v(true));
    // char
    interpret(v('c'));
    // string
    interpret(str("symbol"));
```
### Block
A block contains a sequence of expressions, and the result is also an expression. 
The value of the block is the value of the last expression.
```cpp
    // 3
    interpret(block(v(1),
                    v(2),
                    v(3)));
```

### Basic operations
```cpp
    // 1+2 -> 3
    interpret(add(v(1), v(2)));
    // 1-2 -> -1
    interpret(sub(v(1), v(2)));
    // 1*2 -> 2
    interpret(mul(v(1), v(2)));
    // 10 % 3 -> 1
    interpret(mod(v(1), v(2)));
    // 1 == 2 -> false
    interpret(eq_(v(1), v(2)));
    // 1 > 2 -> false
    interpret(gt(v(1), v(2)));
    // 1 >= 2 -> false
    interpret(ge(v(1), v(2)));
    // 1 < 2 -> true
    interpret(lt(v(1), v(2)));
    // 1 <= 2 -> true
    interpret(le(v(1), v(2)));
    // true && false -> false
    interpret(and_(v(true), v(false)));
    // true || false -> true
    interpret(or_(v(true), v(false)));
```
### Variable definition
```cpp
    /*
     * int a = 100;
     * a;
     */
    interpret(block(define(var("a"), v(100)),
                    var("a"));
```
### If-then-else
```cpp
    // true ? 1 : 2
    interpret(if_(v(true), v(1), v(2)));
```
### Function Call
```cpp
    interpret(block(define(var("+"), lambda(params(var("x"), var("y")),
                                            add(var("x"), var("y")))),
                    define(var("s"), call(var("+"), v(10), v(20))),
                    var("s")));
```
### Closure
```cpp
    using x = var("x");
    using y = var("y");
    using add1 = var("add1");
    using add2 = var("add2");
    using makeAddX = var("makeAddX");

    // 11, 12
    interpret(block(define(makeAddX, lambda(params(y),
                                            lambda(params(x), add(x, y)))),
                    define(add1, call(makeAddX, v(1))),
                    define(add2, call(makeAddX, v(2))),
                    call(add1, v(10)),    // 11
                    call(add2, v(10))));  //12
```

In this example, the function `makeAddX` captures it's parameter `x` and returns another function.
The returned function takes a parameter `y` and returns the sum of `y` and the captured `x`.
We could find that calling `add1(10)` we will get `11` and calling `add2(10)` we will get `12`. 
 
### Recursive Function
```cpp
    using n = var("n");
    using factorial = var("factorial");

    // 3628800
    interpret(block(define(factorial, lambda(params(n),
                                             if_(eq_(n, v(1)),
                                                 v(1),
                                                 mul(n, call(factorial, sub(n, v(1))))))),
                    call(factorial, v(10))));
```

In this example, we define a recursive function `factorial` which calls itself to calculate the product from 1 to `n`.

### Quote and Eval
```cpp
    // quote
    using quoted_add = quote(add(v(1), v(2)));
    // eval
    interpret(eval(quoted_add));
```

### Pattern Match
```cpp
    using x = var("x");
    using y = var("y");

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
    interpret(match(quote(add(v(1), v(2))),
                    case_(sub(_, _), v('-')),
                    case_(add(_, _), v('+')),
                    default_(v('?'))));

    // Capture the first parameter of an `add` expression with `x`.
    // Here the expression `v(1)` is captured as `v(1)`,
    interpret(match(quote(add(v(1), v(2))),
                    case_(add(capture(_, x), _), x),
                    default_(v('?'))));
```
## How to Use

### Use Crisp in Your Project
Crisp is a header-only library, you could use Crisp by simply including Crisp headers in your project.

1. **Copy the `include` folder into your project. Set it as one of your including directories**.
2. 
    - Use **Crisp macro api** by **`#include "crisp/MacroAPI.h"`**.
    - Use **Crisp template api** by **`#include "crisp/TemplateAPI.h"`**.
3. **Enjoy it**.

### Notes

1. Your compiler must support **C++14**.
2. Remember to set the **`-ftemplate-depth-5000`** flag to avoid insufficient template expansion depth.
3. Since Crisp macros may have the same names as symbols in other files, you'd better **include Crisp headers as the last ones of your includes**.
4. **Never mix macros and templates style together**. In C++, expressions like `println(Var<'a', 'b'>)` will be recognized as `println(` `Var<'a'`, `'b'>` `)`, which will cause a compile error. Thus the best practice to use Crisp is **always using one style in a single C++ source file**. 

## Build and Dependency
If you want to build Crisp from source and run Crisp examples, you need to:

1. Prepare "cmake", "make" and a C++ compiler supports C++14 in your system.
2. Clone this project.
2. In the root directory of this project, run commands:
```
cmake .
make
```

## Acknowledgements
This project is inspired by [TemplatedPL](https://github.com/Cheukyin/TemplatedPL). Thanks for the author's great idea.
