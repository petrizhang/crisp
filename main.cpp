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

template <typename Expr, typename ParentEnv = Env<>>
struct interp;

template <typename ParentEnv, int v>
struct interp<Int<v>, ParentEnv> {
  using env = ParentEnv;
  using type = Int<v>;
};

template <typename ParentEnv, typename Ident, typename Value>
struct interp<Define<Ident, Value>, ParentEnv> {
  using env = typename EnvPut<ParentEnv,
                              Ident,
                              typename interp<Value, ParentEnv>::type>::type;
  using type = Nil;
};

template <char... args, typename ParentEnv>
struct interp<Var<args...>, ParentEnv> {
  using env = ParentEnv;
  using type = typename EnvLookup<ParentEnv, Var<args...>>::type;
};

template <typename ParentEnv, typename Head, typename... Tail>
struct interp<Seq<Head, Tail...>, ParentEnv> {
  using env = typename interp<Head>::env;
  using type = typename interp<Seq<Tail...>, env>::type;
};
template <typename ParentEnv, typename Head>
struct interp<Seq<Head>, ParentEnv> {
  using env = typename interp<Head, ParentEnv>::env;
  using type = typename interp<Head, ParentEnv>::type;
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
  using x = var('x');
  using y = var('y');
  using def = define(x, v(1));
  using ref = x;
  using r0 = interp<seq(def, ref)>::type;
  std::cout << r0::c_value();
}