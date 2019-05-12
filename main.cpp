#include <functional>
#include <iostream>
#include <ratio>
#include "tispp.hpp"

using namespace tispp;

/// Evaluate variable definitions .
template <typename Environ, typename Ident, typename Value>
struct Eval<Define<Ident, Value>, Environ> {
  using env = typename EnvPut<Environ, Ident, typename Eval<Value, Environ>::type>::type;
  using type = Nil;
};

/// Evaluate variable references.
template <char... args, typename Environ>
struct Eval<Var<args...>, Environ> {
  using env = Environ;
  using type = typename EnvLookup<Environ, Var<args...>>::type;
};

/// Evaluate a sequence of expressions.
template <typename Environ, typename Head, typename... Tail>
struct Eval<Seq<Head, Tail...>, Environ> {
  using env = typename Eval<Head, Environ>::env;
  using type = typename Eval<Seq<Tail...>, env>::type;
};

template <typename Environ, typename Head>
struct Eval<Seq<Head>, Environ> {
  using env = typename Eval<Head, Environ>::env;
  using type = typename Eval<Head, Environ>::type;
};

/// Evaluate lambda instantiation.
template <typename Environ, typename Body, typename ParamL>
struct Eval<Lambda<ParamL, Body>, Environ> {
  using env = Environ;
  using type = Closure<Environ, Lambda<ParamL, Body>>;
};

/// Calling a closure.
template <typename CallSiteEnviron, typename ClosureV, typename... Args>
struct CallClosure;

template <typename CallSiteEnviron, typename ClosureEnviron, typename Body, typename... Params,
          typename... ArgValues>
struct CallClosure<CallSiteEnviron, Closure<ClosureEnviron, Lambda<ParamList<Params...>, Body>>,
                   Array<ArgValues...>> {
  // Check arguments number
  static_assert(Size<Params...>::value == Size<ArgValues...>::value,
                "Arguments number does't match.");

  // Get function inner scope
  using argDict = typename Zip<Array<Params...>, Array<ArgValues...>>::type;

  // Insert all arguments into the function's scope to create the execution scope
  using executionEnv0 = typename ArrayPushFront<ClosureEnviron, argDict>::type;
  // Add the call site's scope into the end of the environment list,
  // thus the recursive calls could be supported
  using executionEnv = typename ArrayExtendBack<executionEnv0, CallSiteEnviron>::type;

  using env = CallSiteEnviron;
  // Evaluate function body
  using type = typename Eval<Body, executionEnv>::type;
};

/// Evaluate function calls.
template <typename Environ, typename Func, typename... Args>
struct Eval<Call<Func, Args...>, Environ> {
  // Evaluate the expression to get a closure value.
  using closure = typename Eval<Func, Environ>::type;
  // Evaluate argument list.
  using argValues = typename Eval<Array<Args...>, Environ>::type;
  static_assert(IsCallable<closure>::value, "Expected a callable function/closure.");

  using env = Environ;
  // Call the closure.
  using type = typename CallClosure<Environ, closure, argValues>::type;
};

/// Evaluate argument list for function calls.
template <typename Environ, typename Head, typename... Tail>
struct Eval<Array<Head, Tail...>, Environ> {
  using headValue = typename Eval<Head, Environ>::type;
  using tailValues = typename Eval<Array<Tail...>, Environ>::type;

  using env = Environ;
  using type = typename ArrayPushFront<tailValues, headValue>::type;
};

template <typename Environ>
struct Eval<Array<>, Environ> {
  using env = Environ;
  using type = Array<>;
};

template <typename Environ, typename CondEvaluated, typename Body, typename ElseBody>
struct EvalIf {
  using type = typename Eval<Body, Environ>::type;
};

template <typename Environ, typename Body, typename ElseBody>
struct EvalIf<Environ, Bool<false>, Body, ElseBody> {
  using type = typename Eval<ElseBody, Environ>::type;
};

/// Evaluate if-then-else expression
// TODO: optimize If-expression evaluation to avoid unused branch computation
template <typename Environ, typename Cond, typename Body, typename ElseBody>
struct Eval<If<Cond, Body, ElseBody>, Environ> {
  using condEvaluated = typename Eval<Cond, Environ>::type;
  using env = Environ;
  using type = typename EvalIf<Environ, condEvaluated, Body, ElseBody>::type;
};

using program1 = seq(define(var('a'), v(5)), var('a'));

using f = var('f');
using n = var('n');
using fac = var('f', 'a', 'c');

using lam = lambda(params(n),        //
                   if_(eq(n, v(1)),  //
                       v(1),         //
                       mul(n, call(fac, sub(n, v(1))))));
using facDef = define(fac, lam);

using prog = seq(facDef, call(fac, v(10)));

int main() {
  using r = Eval<prog, Env<>>::type;
  std::cout << r::c_value();
  return 0;
}