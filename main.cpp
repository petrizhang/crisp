#include <functional>
#include <iostream>
#include <ratio>
#include "tispp.hpp"

using namespace tispp;

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

// clang-format off
typedef Lambda<
    ParamList<Id<'a'>, Id<'b'>>,
    Add<Id<'a'>, Id<'b'>>> add;

typedef Lambda<
    ParamList<Id<'x'>>,
    Lambda<
        ParamList<Id<'y'>>,
        Add<Id<'x'>, Id<'y'>>>> make_add_x_y;

using fac = Id<'f','a','c'>;

using fac_def =
define(fac,
    lambda(params(id('n')),
        _if(eq(id('x'), v(1)),
            v(1),
            call(fac, sub(id('n'),v(1))))));

// clang-format on
template <typename... Args>
struct Extend {
  using type = int;
};

template <typename Env, typename Expr>
struct interp;

template <typename Env, typename Ident, typename Value>
struct interp<Env, Define<Ident, Value>> {
  using env = typename Extend<Env, Pair<Ident, Value>>::type;
  struct Eval {};
};

template <typename Env, typename... Args>
struct interp<Env, Add<Args...>> {
  using env = Env;
  using type = typename AddImpl<Args...>::type;
};

// clang-format on
int main() {
  std::cout << "Hello, World!" << std::endl;
  std::cout << eval(eq(v(1), v(1)))::type::c_value() << std::endl;
  std::cout << eval(add(v(1), v(1000)))::type::c_value() << std::endl;
  return 0;
}