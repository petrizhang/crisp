#ifndef CRISP_CORE_LAMBDA_HPP
#define CRISP_CORE_LAMBDA_HPP

#include "crisp/core/common.hpp"

namespace crisp {
namespace core {

//===========================================================================//
// Placeholders
//===========================================================================//
/// Forward declarations
template <unsigned N>
struct Arg {};
using _1  = Arg<1>;
using _2  = Arg<2>;
using _3  = Arg<3>;
using _4  = Arg<4>;
using _5  = Arg<5>;
using _6  = Arg<6>;
using _7  = Arg<7>;
using _8  = Arg<8>;
using _9  = Arg<9>;
using _10 = Arg<10>;

template <class A_, typename... Args_>
struct get_arg {
  using type = A_;
};

template <typename T_, typename... Args_>
struct get_arg<Arg<1>, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T_, typename... Args_>
struct get_arg<Arg<2>, T1_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T_, typename... Args_>
struct get_arg<Arg<3>, T1_, T2_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T3_, typename T_,
          typename... Args_>
struct get_arg<Arg<4>, T1_, T2_, T3_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T3_, typename T4_, typename T_,
          typename... Args_>
struct get_arg<Arg<5>, T1_, T2_, T3_, T4_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T3_, typename T4_, typename T5_,
          typename T_, typename... Args_>
struct get_arg<Arg<6>, T1_, T2_, T3_, T4_, T5_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T3_, typename T4_, typename T5_,
          typename T6_, typename T_, typename... Args_>
struct get_arg<Arg<7>, T1_, T2_, T3_, T4_, T5_, T6_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T3_, typename T4_, typename T5_,
          typename T6_, typename T7_, typename T_, typename... Args_>
struct get_arg<Arg<8>, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T3_, typename T4_, typename T5_,
          typename T6_, typename T7_, typename T8_, typename T_,
          typename... Args_>
struct get_arg<Arg<9>, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T_, Args_...> {
  using type = T_;
};

template <typename T1_, typename T2_, typename T3_, typename T4_, typename T5_,
          typename T6_, typename T7_, typename T8_, typename T9_, typename T_,
          typename... Args_>
struct get_arg<Arg<10>, T1_, T2_, T3_, T4_, T5_, T6_, T7_, T8_, T9_, T_,
               Args_...> {
  using type = T_;
};

//===========================================================================//
// Lambda
//===========================================================================//
/**
 * A convenient lambda implementation.
 * @tparam F
 */
template <typename>
struct lambda;

template <typename ParamList_, typename... Args_>
struct collect_arg_list;

template <typename P_, typename... Params_, typename... Args_>
struct collect_arg_list<List<P_, Params_...>, Args_...> {
  using _Head = typename get_arg<P_, Args_...>::type;
  using _Tail = typename collect_arg_list<List<Params_...>, Args_...>::type;
  using type  = typename list_prepended<_Tail, _Head>::type;
};

template <typename... Args_>
struct collect_arg_list<List<>, Args_...> {
  using type = List<>;
};

template <template <typename...> class F_, typename... Params_>
struct lambda<F_<Params_...>> {
  template <typename... Args_>
  struct apply {
    using _ArgList =
        typename collect_arg_list<List<Params_...>, Args_...>::type;
    using type = typename apply_with_arg_list<F_, _ArgList>::type;
  };
};

template <typename T_>
struct is_lambda : is_template_of<lambda, T_> {};

}  // namespace core
}  // namespace crisp

#endif  // CRISP_CORE_LAMBDA_HPP
