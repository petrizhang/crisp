#include <functional>
#include <iostream>
#include <ratio>
#include "tispp.hpp"

using namespace tispp;

template <typename... Args>
struct Seq {};

template <typename... Args>
struct ParamList {};

template <typename... Args>
struct If {};

template <typename... Args>
struct Define {};

template <typename... Args>
struct Call {};

template <typename Params, typename Body>
struct Lambda {};

using add = Lambda<
    ParamList<Var<'a'>, Var<'b'>>,
    Add<Var<'a'>, Var<'b'>>>;

using make_add_x_y = Lambda<
    ParamList<Var<'x'>>,
    Lambda<
        ParamList<Var<'y'>>,
        Add<Var<'x'>, Var<'y'>>>>;

using fac = Var<'f', 'a', 'c'>;
using x = Var<'x'>;
using y = Var<'y'>;

using program =
    seq(define(fac,
               lambda(params(var('n')),
                      _if(eq(var('x'), v(1)),
                          v(1),
                          call(fac, sub(var('n'), v(1)))))),
        v(1),
        v(2));

template <typename P, typename Key>
struct LookupEnv {
  using type = Int<1>;
};

template <typename Expr>
struct interp {
  using env = int;
};

template <typename Ident, typename Value>
struct interp<Define<Ident, Value>> {
  using env = int;
};

template <char... args>
struct interp<Var<args...>> {
  using type = int;
};

using program =
    seq(define(fac,
               lambda(params(var('n')),
                      _if(eq(var('x'), v(1)),
                          v(1),
                          call(fac, sub(var('n'), v(1)))))),
        v(1),
        v(2));

using program1 =
    seq(define(var('a'), v(5)),
        var('a'));

int main() {

}