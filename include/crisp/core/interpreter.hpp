#ifndef CRISP_INTERPRETER_HPP
#define CRISP_INTERPRETER_HPP

#include "crisp/core/common.hpp"

namespace crisp {
namespace core {

template <typename...>
struct InternalList {};

template <typename Expr_, typename Env_, typename Cont_>
struct Interpret;

template <typename Cont_, typename Val_>
struct ApplyCont;

template <template <typename...> class F_, typename... Args_>
struct NativeCall {};

//===========================================================================//
// Cont
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}

template <typename Cont_, typename RestList_>
struct InternalListCont;

template <typename Cont_, template <typename...> class F_>
struct NativeCallCont {};

//===========================================================================//
// ApplyCont
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}

template <typename Cont_, typename RestList_, typename... Evaluated>
struct ApplyCont<InternalListCont<Cont_, RestList_>, In> {
  using _EvalutedList = 1;
};

//===========================================================================//
// Interpret
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}

template <typename Env_, typename Cont_, template <typename...> class F_,
          typename... Args_>
struct Interpret<NativeCall<F_, Args_...>, Env_, Cont_> {
  using type = typename Interpret<InternalList<Args_...>, Env_,
                                  NativeCallCont<Cont_, F_>>::type;
};

}  // namespace core
}  // namespace crisp

#endif  // CRISP_INTERPRETER_HPP
