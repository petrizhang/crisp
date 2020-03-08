#ifndef CRISP_BASE_COMMON_HPP
#define CRISP_BASE_COMMON_HPP

#include <type_traits>

namespace crisp {
namespace base {
using std::is_same;
using std::size_t;

//===========================================================================//
// Basic types
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}
/**
 * Boolean literal.
 * @tparam V
 */
template <bool V_>
struct Bool {
  static const bool value = V_;
};

/**
 * Char literal.
 * @tparam V
 */
template <char V_>
struct Char {
  static const char value = V_;
};

/**
 * Int literal.
 * @tparam V
 */
template <int V_>
struct Int {
  static const int value = V_;
};

/**
 * Nil literal, which equals to `null` in many programming languages.
 */
struct Nil {};

/**
 * Undefined.
 * We use this value when we cannot resolve a variable reference.
 */
struct Undefined {};

/**
 * String literal.
 * @tparam args
 */
template <char... args_>
struct Str {};

/**
 * Convert a string literal to a String<...> instance.
 * e.g.
 * @code
 *   using t = decltype("hello"_s);
 *   std::cout << t::value; // hello
 * @endcode
 * @tparam CharT
 * @tparam Chars
 * @return
 */
template <typename CharT_, CharT_... Chars_>
constexpr auto operator""_s() {
  return Str<static_cast<char>(Chars_)...>{};
}

template <typename...>
struct List {};

template <typename L_, typename R_>
struct Pair {};

template <typename...>
struct Dict {};

/**
 * When branch used in `Cond` expression.
 * @tparam Condition
 * @tparam Result
 */
template <typename Condition_, typename Result_>
struct When {};

/**
 * Default branch used in `Match` and `Cond` expression.
 * @tparam Expr
 */
template <typename Expr_>
struct Default {};

//===========================================================================//
// Error reporting
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}
/**
 * Use this type in static_assert to trigger a compile error.
 * @tparam ... type parameters will be displayed in compiler error message.
 */
template <typename...>
struct Error {
  static const bool value = false;
};

namespace ErrorMsg {

template <char...>
struct ErrorMsg {
  static const bool This = false;
};

template <typename>
struct from;

template <char... values_>
struct from<Str<values_...>> {
  using type = ErrorMsg<values_...>;
};

}  // namespace ErrorMsg

/**
 * Trigger a compile error and show message S_.
 * @tparam S_ must be a Str<...> instantiation.
 */
template <typename S_>
struct report_error {
  using _Msg = typename ErrorMsg::from<S_>::type;
  static_assert(_Msg::This, "is an error message.");
  using type = void;
};

//===========================================================================//
// Operators
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}
/**
 * Implementation for `Add`.
 * @tparam L
 * @tparam R
 */
template <typename L_, typename R_>
struct add_ {
  static_assert(Error<L_, R_>::value,
                "Incompatible types for operation `Add`.");
};

template <int LV_, int RV_>
struct add_<Int<LV_>, Int<RV_>> {
  using type = Int<LV_ + RV_>;
};

/**
 * Most binary operators (-,*,%,...) follow the same pattern as * `AddImpl`,
 * thus we implement them with an unified macro.
 */
#define BinaryOperator(OpName, Operator, LeftValueType, LeftType,     \
                       RightValueType, RightType, ResultType)         \
  template <typename L_, typename R_>                                 \
  struct OpName {                                                     \
    static_assert(Error<L_, R_>::value,                               \
                  "Incompatible types for operation `" #OpName "`."); \
  };                                                                  \
                                                                      \
  template <LeftValueType LV_, RightValueType RV_>                    \
  struct OpName<LeftType<LV_>, RightType<RV_>> {                      \
    using type = ResultType<(LV_ Operator RV_)>;                      \
  };

BinaryOperator(sub_, -, int, Int, int, Int, Int);
BinaryOperator(mul_, *, int, Int, int, Int, Int);
BinaryOperator(mod_, %, int, Int, int, Int, Int);
BinaryOperator(and_, &&, bool, Bool, bool, Bool, Bool);
BinaryOperator(pr_, ||, bool, Bool, bool, Bool, Bool);
BinaryOperator(is_greater_than_, >, int, Int, int, Int, Bool);
BinaryOperator(is_less_than_, <, int, Int, int, Int, Bool);
BinaryOperator(is_greater_equal_, >=, int, Int, int, Int, Bool);
BinaryOperator(is_less_equal_, <=, int, Int, int, Int, Bool);

/**
 * Check if two types are the same.
 * @tparam L
 * @tparam R
 */
template <typename L_, typename R_>
struct is_equal {
  using type = Bool<std::is_same<L_, R_>::value>;
};

//===========================================================================//
// Template Utilities
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}
/**
 * Convert A<Args...> to B<Args...>
 * @tparam A_
 * @tparam B_
 */
template <typename A_, template <typename...> class B_>
struct convert;

template <template <typename...> class A_, template <typename...> class B_,
          typename... Args_>
struct convert<A_<Args_...>, B_> {
  using type = B_<Args_...>;
};

/** A copy of the list with an element prepended. */
template <typename T_, typename>
struct list_prepended;

template <typename... Args_, typename Element_>
struct list_prepended<List<Args_...>, Element_> {
  using type = List<Element_, Args_...>;
};

/**
 * Test if given type is a template.
 * e.g. is_template<int>::value will be false.
 *      is_template<T<int>>::value will be true.
 * @tparam T
 */
template <typename T>
struct is_template {
  static const bool value = false;
};

template <template <typename...> class C_, typename... Args_>
struct is_template<C_<Args_...>> {
  static const bool value = true;
};

/**
 * Test if the 2 given types are instances of a same template.
 * e.g. is_same_template<A<...>,A<...>>::value will be true.
 *      is_same_template<A<...>,B<...>>::value will be false.
 * @tparam A_
 * @tparam B_
 */
template <typename A_, typename B_>
struct is_same_template {
  static const bool value = false;
};

template <template <typename...> class A_, typename... Args1_,
          typename... Args2_>
struct is_same_template<A_<Args1_...>, A_<Args2_...>> {
  static const bool value = true;
};

/**
 * Check if a template is empty.
 * @tparam T_
 */
template <typename T_>
struct is_empty_template;

template <template <typename...> class C_, typename... Args_>
struct is_empty_template<C_<Args_...>> {
  static const bool value = (sizeof...(Args_) == 0);
};

/**
 * Check if given type `T` is a instance of template `C`.
 * e.g. is_template_of<Array, Array<Int<1>,Int<2>>>::value will be true.
 * @tparam C_
 * @tparam T_
 */
template <template <typename...> class C_, typename T_>
struct is_template_of {
  static const bool value = false;
};

template <template <typename...> class C_, typename... Args_>
struct is_template_of<C_, C_<Args_...>> {
  static const bool value = true;
};

/**
 * Check if T is an instance of C<ValueType...>.
 * e.g. is_value_template_of<int, A, A<1>> will be true.
 *      is_value_template_of<bool, B, b<true>> will be true.
 * @tparam ValueType_
 * @tparam C_
 * @tparam T_
 */
template <typename ValueType_, template <ValueType_...> class C_, typename T_>
struct is_value_template_of {
  static const bool value = false;
};

template <typename ValueType_, template <ValueType_...> class C_,
          ValueType_... Args_>
struct is_value_template_of<ValueType_, C_, C_<Args_...>> {
  static const bool value = true;
};

/**
 * Trigger a compile error when Expected != Actual
 * @tparam Expected_
 * @tparam Actual_
 */
template <typename Expected_, typename Actual_>
struct expect {
  static_assert(is_same<Expected_, Actual_>::value, "Type mismatch.");
  using type = void;
};

/**
 * Trigger a compile error when
 * @tparam ExpectedTemplate_
 * @tparam ActualType_
 */
template <template <typename...> class ExpectedTemplate_, typename ActualType_>
struct expect_template {
  static_assert(is_template_of<ExpectedTemplate_, ActualType_>::value,
                "Template mismatch");
  using type = void;
};

/**
 * Apply a template with arguments in ArgList_
 * @tparam F_
 * @tparam ArgList_
 */
template <template <typename...> class F_, typename ArgList_>
struct apply_with_arg_list;

template <template <typename...> class F_, typename... Args_>
struct apply_with_arg_list<F_, List<Args_...>> {
  using type = typename F_<Args_...>::type;
};

//===========================================================================//
// is_xxx
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}
template <typename T_>
struct is_bool_ : is_value_template_of<bool, Bool, T_> {};
template <typename T_>
struct is_char_ : is_value_template_of<char, Char, T_> {};
template <typename T_>
struct is_int_ : is_value_template_of<int, Int, T_> {};
template <typename T_>
struct is_nil_ : is_same<T_, Nil> {};
template <typename T_>
struct is_undefined_ : is_same<T_, Undefined> {};
template <typename T_>
struct is_str_ : is_value_template_of<char, Str, T_> {};
template <typename T_>
struct is_list : is_template_of<List, T_> {};
template <typename T_>
struct is_pair_ : is_template_of<Pair, T_> {};
template <typename T_>
struct is_dict_ : is_template_of<Dict, T_> {};

//===========================================================================//
// Conditional
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}
/**
 * Get result from branches according to branches' condition.
 * It is very similar to switch statement in C++.
 * e.g.
 * @code
 * conditional<When<Bool<true>, Int<1>>,
 *             When<Bool<false>, Int<1>>,
 *             Else<Int<2>> >;
 * @endcode
 *
 */
template <typename Branch1_, typename Branch2_, typename... Branches_>
struct conditional;

template <typename Body_, typename ElseBody_>
struct conditional<When<Bool<true>, Body_>, Default<ElseBody_>> {
  using type = Body_;
};

template <typename Body_, typename ElseBody_>
struct conditional<When<Bool<false>, Body_>, Default<ElseBody_>> {
  using type = ElseBody_;
};

template <typename Branch_, typename ElseBranch_>
struct conditional<Branch_, ElseBranch_> {
  static_assert(Error<Branch_>::value,
                "expected a valid `When` instantiation.");
  static_assert(Error<ElseBranch_>::value,
                "expected a valid `Else` instantiation.");
};

template <typename Body1_, typename Branch2_, typename Branch3_,
          typename... Tail_>
struct conditional<When<Bool<true>, Body1_>, Branch2_, Branch3_, Tail_...> {
  using type = Body1_;
};

template <typename Body1_, typename Branch2_, typename Branch3_,
          typename... Tail_>
struct conditional<When<Bool<false>, Body1_>, Branch2_, Branch3_, Tail_...> {
  using type = typename conditional<Branch2_, Branch3_, Tail_...>::type;
};

template <typename Branch1_, typename Branch2_, typename Branch3_,
          typename... Tail_>
struct conditional<Branch1_, Branch2_, Branch3_, Tail_...> {
  static_assert(Error<Branch1_>::value,
                "expected a valid `When` instantiation.");
};

/**
 * As same as @see Conditional,
 * except that we will expand the template context saved in `defer_apply`.
 * e.g.
 * @code
 * conditional_apply<When<Bool<true>, defer_apply<Array, Int<1>> >,
 *                   When<Bool<false>, defer_apply<Array, Int<2>> >,
 *                   Else<defer_apply<Array, Int<3>>> >;
 * @endcode
 *
 */
template <typename Branch1_, typename Branch2_, typename... Branches_>
struct conditional_apply {
  using Result = typename conditional<Branch1_, Branch2_, Branches_...>::type;
  using type   = typename Result::template apply<>::type;
};

//===========================================================================//
// Defer
//===========================================================================//
namespace ___________________________________________________________________ {
// This namespace is used for navigation in IDE.
}
/**
 * Save the context of a template `C` and it's arguments `Args`,
 * and apply it later.
 *
 * @tparam C_
 * @tparam Args_
 */
template <template <typename...> class C_, typename... Args_>
struct defer_apply {
  template <typename...>
  struct apply {
    using type = typename C_<Args_...>::type;
  };
};

/**
 * Save the context of a template `C` and it's arguments `Args`,
 * and instantiate  it later.
 *
 * @tparam C_
 * @tparam Args_
 */
template <template <typename...> class C_, typename... Args_>
struct defer_construct {
  template <typename...>
  struct apply {
    using type = C_<Args_...>;
  };
};

/**
 * A meta function receives a type T and return it unchanged.
 * @tparam T_
 */
template <typename T_>
struct id {
  using type = T_;
};

/**
 *  A meta function receives type parameters and returns Nil
 * @tparam ...
 */
template <typename...>
struct nil_f {
  using type = Nil;
};

}  // namespace base
}  // namespace crisp

#endif  // CRISP_BASE_COMMON_HPP
