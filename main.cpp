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
    ParamList<Var<'a'>, Var<'b'>>,
    Add<Var<'a'>, Var<'b'>>> add;

typedef Lambda<
    ParamList<Var<'x'>>,
    Lambda<
        ParamList<Var<'y'>>,
        Add<Var<'x'>, Var<'y'>>>> make_add_x_y;

typedef
Define<Symbol<'x'>,
    Lambda<ParamList<Var<'n'>>,
        If<IsEqual<Var<'x'>, v(1)>,
            v(1),
            Call<Symbol<'x'>,Sub<Var<'n'>,v(1)>>>>> factorial;

int main() {
std::cout << "Hello, World!" << std::endl;
  Eval<factorial>::type a;
  Eval<Call<Symbol<'x'>,v(10)>>::type b;
  return 0;
}