#include <functional>
#include <iostream>
#include <ratio>
#include "tispp.hpp"

using namespace tispp;

template <typename... Args>
struct Seq {};

template <typename... Args>
struct ParamList {};

template <typename Cond, typename Body, typename ElseBody>
struct If {};

template <typename... Args>
struct Define {};

template <typename... Args>
struct Call {};

template <typename Params, typename Body>
struct Lambda {};

template <typename Environ, typename Func>
struct Closure {};

template <typename... Args>
struct IsCallable {
  static const bool value = false;
};

template <typename... Args>
struct IsCallable<Closure<Args...>> {
  static const bool value = true;
};

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
                      if_(eq(var('x'), v(1)),
                          v(1),
                          call(fac, sub(var('n'), v(1)))))),
        v(1),
        v(2));

template <typename Expr, typename Environ = Env<>>
struct interp;

template <typename Environ, int v>
struct interp<Int<v>, Environ> {
  using env = Environ;
  using type = Int<v>;
};

/// Evaluate variable definition.
template <typename Environ, typename Ident, typename Value>
struct interp<Define<Ident, Value>, Environ> {
  using env = typename EnvPut<Environ,
                              Ident,
                              typename interp<Value, Environ>::type>::type;
  using type = Nil;
};

/// Evaluate variable references.
template <char... args, typename Environ>
struct interp<Var<args...>, Environ> {
  using env = Environ;
  using type = typename EnvLookup<Environ, Var<args...>>::type;
};

/// Evaluate a sequence of expressions.
template <typename Environ, typename Head, typename... Tail>
struct interp<Seq<Head, Tail...>, Environ> {
  using env = typename interp<Head>::env;
  using type = typename interp<Seq<Tail...>, env>::type;
};

template <typename Environ, typename Head>
struct interp<Seq<Head>, Environ> {
  using env = typename interp<Head, Environ>::env;
  using type = typename interp<Head, Environ>::type;
};

/// Evaluate lambda instantiation.
template <typename Environ, typename Body, typename ParamL>
struct interp<Lambda<ParamL, Body>, Environ> {
  using env = Environ;
  using type = Closure<Environ, Lambda<ParamL, Body>>;
};

/// Calling a closure.
template <typename CallSiteEnviron, typename ClosureV, typename... Args>
struct CallClosure;

template <typename CallSiteEnviron, typename ClosureEnviron,
          typename Body, typename... Params, typename... ArgValues>
struct CallClosure<CallSiteEnviron,
                   Closure<ClosureEnviron, Lambda<ParamList<Params...>, Body>>,
                   Array<ArgValues...>> {
  // Check arguments number
  static_assert(Size<Params...>::value == Size<ArgValues...>::value, "Arguments number does't match.");

  // Get function inner scope
  using argDict = typename Zip<Array<Params...>, Array<ArgValues...>>::type;
  using functionInnerScope = typename ArrayHead<ClosureEnviron>::type;
  static_assert(!std::is_same<functionInnerScope, Nil>::value, "Find a closure without an environment.");

  // Get parent scopes
  using parentScopes = typename ArrayTail<ClosureEnviron>::type;
  // Insert all arguments into the function's scope to create the execution scope
  using executionScope = typename ArrayExtendBack<functionInnerScope, argDict>::type;
  // Replace the closure's scope with the new one that contains all arguments
  using executionEnv0 = typename ArrayPushFront<parentScopes, executionScope>::type;
  // Add the call site's scope into the end of the environment list,
  // thus the recursive calls could be supported
  using executionEnv = typename ArrayPushBack<executionEnv0, CallSiteEnviron>::type;

  using env = CallSiteEnviron;
  // Evaluate function body
  using type = typename interp<Body, executionEnv>::type;
};

/// Evaluate function calls.
template <typename Environ, typename Func, typename... Args>
struct interp<Call<Func, Args...>, Environ> {
  // Evaluate the expression to get a closure value.
  using closure = typename interp<Func, Environ>::type;
  // Evaluate argument list.
  using argValues = typename interp<Array<Args...>, Environ>::type;
  static_assert(IsCallable<closure>::value, "Expected a callable function/closure.");

  using env = Environ;
  // Call the closure.
  using type = typename CallClosure<Environ, closure, argValues>::type;
};

/// Evaluate argument list for function calls.
template <typename Environ, typename Head, typename... Tail>
struct interp<Array<Head, Tail...>, Environ> {
  using headValue = typename interp<Head, Environ>::type;
  using tailValues = typename interp<Array<Tail...>, Environ>::type;

  using env = Environ;
  using type = typename ArrayPushFront<tailValues, headValue>::type;
};

template <typename Environ>
struct interp<Array<>, Environ> {
  using env = Environ;
  using type = Array<>;
};

/// Evaluate if-then-else expression
// TODO: optimize If-expression evaluation to avoid unused branch computation
template <typename Environ, typename Cond, typename Body, typename ElseBody>
struct interp<If<Cond, Body, ElseBody>, Environ> {
  using env = Environ;
  using type = typename std::conditional<interp<Cond>::value,
                                         typename interp<Body>::type,
                                         typename interp<ElseBody>::type>::type;
};

/// Evaluate if-then-else expression
template <typename Environ, typename L, typename R>
struct interp<IsEqual<L, R>, Environ> {
  using env = Environ;
  using type = Bool<std::is_same<typename interp<L, Environ>::type,
                                 typename interp<R, Environ>::type>::value>;
};

//template <typename Environ, typename Head>
//struct interp<Seq<Head>, Environ> {
//  using env = typename interp<Head, Environ>::env;
//  using type = typename interp<Head, Environ>::type;
//};

using program1 =
    seq(define(var('a'), v(5)),
        var('a'));

#define if__(args...) If<args>

using f = var('f');
using n = var('n');

using func = lambda(params(n),
                    if__(eq(n, v(1)),
                         v(1),
                         call(f, sub(n, v(1)))));

using prog = seq(define(f, func),
                 call(f, v(10)));
int main() {
  using r = interp<prog>::type;
  return 0;
}