# tispp
Tispp: Yet Another Lisp Interpreter Written in C++ Template

## About Tispp
Tispp is a lisp-like DSL built upon C++ templates.
Users could consider Tispp as a lisp interpreter which interprets lisp programs at C++ compile time.

With Tispp, users could easily build compile-time programs by an extremely friendly syntax,
rather than manually writing tedious C++ templates code. 

Just like lisp, Tispp is a simple but powerful functional programming language. The core features of Tispp
include immutable variables, lambdas, lexical scope and closures, and recursive functions.

Further more, **all operations in Tispp are accomplished at compile time**. It is the most attractive and awesome feature of Tispp.
## Examples
### Closure
```cpp
  using x = var('x');
  using y = var('y');
  using add1 = var('a', 'd', 'd', '1');
  using add2 = var('a', 'd', 'd', '2');
  using make_add_x = var('m', 'a', 'k', 'e', 'a', 'd', 'd', 'x');

  // 11
  auto v1 = eval(seq(define(make_add_x, lambda(params(y),
                                               lambda(params(x), add(x, y)))),
                     define(add1, call(make_add_x, v(1))),
                     call(add1, v(10))))::type::c_value();

  // 12
  auto v2 = eval(seq(define(make_add_x, lambda(params(y),
                                               lambda(params(x), add(x, y)))),
                     define(add2, call(make_add_x, v(2))),
                     call(add2, v(10))))::type::c_value();

  // 11
  std::cout << v1 << std::endl;
  // 12
  std::cout << v2 << std::endl;
```

In this example, the function `make_add_x` captures it's parameter `x` and returns another function.
The returned function takes a parameter `y` and returns the sum of `y` and the captured `x`.
We could find that calling `add1(10)` we will get `11` and calling `add2(10)` we will get `12`. 
 
### Recursive Function
```cpp
  using n = var('n');
  using factorial = var('f', 'a', 'c', 't', 'o', 'r', 'i', 'a', 'l');

  // 3628800
  auto v3 = eval(seq(define(factorial, lambda(params(n),
                                              if_(eq_(n, v(1)),
                                                  v(1),
                                                  mul(n, call(factorial, sub(n, v(1))))))),
                     call(factorial, v(10))))::type::c_value();
  // 3628800
  std::cout << v3 << std::endl;
```

In this example, we define a recursive function `factorial` which calls itself to calculate the product from 1 to `n`.

## Expressions
Tispp provides two kinds of syntax: macro style and template style. Users could choose their preferred one.

A basic principle to use Tispp is **never mix these two styles together**, as it will lead to unexpected behaviors.

For example, `add(Var<'a',b'>)` will be recognized as `"add"("Var<'a'", "b'>")`.   

Thus the best practice is always use one style in a single C++ source file. 

Generally, to express the same program, the macro style will be shorter, more readable 
and more friendly to IDE highlighting. Thus **we recommend the macro style**. 

### Macro Style
Users could find a complete macro api list at "src/tispp_macros.h".
  
### Template Style
Users could find a complete template api list at "src/tispp_templates.h".

- `Nil`: the Nil value is similar to null in other languages.
- `Bool<B>`: a boolean value. e.g. `Bool<true>, Bool<false>`
- `Char<C>`: a char value. e.g. `Char<'a'>, Char<'b'>`
- `Int<N>` : an integer value. e.g. `Int<5>`
- `Var<'c','c',...>`: a symbol/variable reference. e.g. `Var<'a'>, Var<'t','e','s','t'>`
- `Pair<L,R>`: a pair value, it is very similar to `std::pair` in C++
- `List<e1,e2,...en>`: equivalent to  `Pair<e1, Pair<e2, ...Pair<en, Nil>>`, e.g. `List<Int<1>,Int<2>>`
- `Lambda< ParamList< Var<..>, Var<..>... >, Body >`, e.g. :
```cpp
Lambda< ParamList<Var<'x'>,Var<'y'>>,
    Add<Var<'x'>, Var<'y'>> >
```
- `Call`: calling user defined functions 

## Build and Dependency

### Use Tispp in Your Project
Tispp is a header-only library, users could use Tispp by copying the head files at the `src` folder.

1. To use Tispp macro api, add `#include "tispp_macros.h"` to your c++ source code.
2. To use Tispp template api, add`#include "tispp_templates.h"` in c++ source code.

Enjoy it.

Notes:

1. Your compiler must support C++11.
2. Remember to set the `-ftemplate-depth-5000` flag to avoid insufficient template expansion depth.
3. You'd better use Tispp headers as the last ones of your includes. For that Tispp macros may potentially have the same names as symbols in other source files and cause compile errors.
4. As mentioned above: **never mix macros and templates style together**.   

### Build Examples and Tests
1. Prepare "cmake" and "make" and a C++ compiler supports C++11 in your system.
2. Clone this project.
2. In the project root directory, run commands:
```
cmake .
make
```
