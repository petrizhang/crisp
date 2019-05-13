# tispp
Tispp: Yet Another Lisp Interpreter Written in C++ Template

## About Tispp
Tispp is a DSL built upon C++ template.
Users could consider Tispp as a lisp interpreter which interpret lisp programs at C++ compile time.
With Tispp, users could easily build compile-time programs by an extremely friendly syntax,
rather than manually writing tedious C++ templates.

Just like lisp, Tispp is a simple but powerful functional programming language. The core features of Tispp
include immutable variables, lambdas, lexical scope and closures, and recursive functions.

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

In this example, we define a recursive function `factorial` which calculates the product from 0 to `n`.
It calls itself in it's function body thus it is a recursive function. 

## Expressions
Tispp provides two kinds of syntax: macro style and template style.
A basic principle to use Tispp is that **never mix these two styles together**.
Mix of macros and templates will lead to unexpected behaviors
as the different priority of `,` in macros and templates.
For example, `add(Var<'a',b'>)` will be recognized as `"add"("Var<'a'", "b'>")`.   
So the best practice is to always use one style in a C++ source file. 
Generally, to express the same program, the macro style will be shorter, more readable 
and more friendly to IDE highlighting. Thus we recommend the macro style. 

### Macro Style
Users could find a complete macro api list at "src/tispp_macros.h".
  
### Template Style
Users could find a complete template api list at "src/tispp_templates.h".

- `Nil`: the Nil value is similar to null in other languages.
- `Bool<B>`: a boolean value. e.g. `Bool<true>, Bool<false>`
- `Char<C>`: a char value. e.g. `Char<'a'>, Char<'b'>``
- `Int<N>` : an integer value. e.g. `Int<5>`
- `Var<'c','c',...>`: a symbol/variable reference. e.g. `Var<'a'>, Var<'t','e','s','t'>`
- `Pair<L,R>`: a pair value, it is very similar to `std::pair` in C++
- `List<e1,e2,...en>`: equivalent to  `Pair<e1, Pair<e2, ...Pair<en, Nil>>, e.g. `List<Int<1>,Int<2>>`
- `Lambda< ParamList< Var<..>, Var<..>... >, Body >`, e.g. :
```cpp
Lambda< ParamList<Var<'x'>,Var<'y'>>,
    Add<Var<'x'>, Var<'y'>> >
```
- `Call`: calling user defined functions 

## Build and Dependency

### Use Tispp in Your Project
Tispp is a header-only library, you could easily 
copy the head files at the `src` folder to your project and choose a style:
- Add a line `#include "tispp_macros.h"` in your source to use Tispp macro api.
- Add a line `#include "tispp_templates.h"` in your source to use Tispp template api.  
Then enjoy it.

Notes:
0. Your compiler must support C++11.
1. Remember to set the `-ftemplate-depth-5000` flag to avoid insufficient template expansion depth.
2. You'd better add the line `#include "tispp_macros.h"` to the last after all your includes
because that our macros may potentially have the same names as symbols in other source files.
Those same names will cause compile errors for other source files.
3. As mentioned above: **never mix macros and templates style together**.   

### Build Examples and Tests
1. Prepare `cmake` and `make` and a compiler supports C++11.
2. In project directory, run commands:
```
cmake .
make
```