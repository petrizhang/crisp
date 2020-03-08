#ifndef CRISP_BASE_LIST_HPP
#define CRISP_BASE_LIST_HPP

#include <type_traits>

#include "common.hpp"
#include "crisp/base/common.hpp"

namespace crisp {
namespace base {
namespace list {
using namespace crisp::base;

/** from */
template <typename T_>
struct from;

template <template <typename...> class T_, typename... Args_>
struct from<T_<Args_...>> {
  using type = List<Args_...>;
};

/** is_empty */
template <typename T_>
struct is_empty {
  static_assert(is_list<T_>::value, "Expected a list.");
};

template <typename... Args_>
struct is_empty<List<Args_...>> {
  static const bool value = sizeof...(Args_) == 0;
};

/** is_empty_list */
template <typename T_>
struct is_empty_list {
  static const bool value = false;
};

template <>
struct is_empty_list<List<>> {
  static const bool value = true;
};

/** non_empty */
template <typename T_>
struct non_empty {
  static_assert(is_list<T_>::value, "Expected a list.");
};

template <typename... Args_>
struct non_empty<List<Args_...>> {
  static const bool value = sizeof...(Args_) > 0;
};

/** non_empty_list */
template <typename T_>
struct non_empty_list {
  static const bool value = false;
};

template <typename T_, typename... Args_>
struct non_empty_list<List<T_, Args_...>> {
  static const bool value = true;
};

/**  The size of a list. */
template <typename T_>
struct size {
  static_assert(is_list<T_>::value, "Expected a list.");
};

template <typename... Args_>
struct size<List<Args_...>> {
  static const size_t value = sizeof...(Args_);
};

/** A copy of the list with an element prepended. */
template <typename T_, typename>
struct prepended {
  static_assert(is_list<T_>::value, "Expected a list.");
};

template <typename... Args_, typename Element_>
struct prepended<List<Args_...>, Element_> {
  using type = List<Element_, Args_...>;
};

/** A copy of this sequence with an element appended. */
template <typename T_, typename>
struct appended {
  static_assert(is_list<T_>::value, "Expected a list.");
};

template <typename... Args_, typename Element_>
struct appended<List<Args_...>, Element_> {
  using type = List<Args_..., Element_>;
};

/** Returns a new list containing the elements from the left hand operand
 * followed by the elements from the right hand operand.
 */
template <typename L_, typename R_>
struct appended_all {
  static_assert(is_list<L_>::value, "Expected a list.");
  static_assert(is_list<R_>::value, "Expected a list.");
};

template <typename... LArgs_, typename... RArgs_>
struct appended_all<List<LArgs_...>, List<RArgs_...>> {
  using type = List<LArgs_..., RArgs_...>;
};

/** Alias of appended_all */
template <typename L_, typename R_>
struct concat : appended_all<L_, R_> {};

/** Selects the first element of a list. */
template <typename T_>
struct head {
  static_assert(non_empty_list<T_>::value, "Expected a non-empty list.");
};

template <typename Head_>
struct head<List<Head_>> {
  using type = Head_;
};

template <typename Head_, typename... Args_>
struct head<List<Head_, Args_...>> {
  using type = Head_;
};

/** Selects the last element of a list. */
template <typename T_>
struct last {
  static_assert(non_empty_list<T_>::value, "Expected a non-empty list.");
};

template <typename Last_>
struct last<List<Last_>> {
  using type = Last_;
};

template <typename Head_, typename... Args_>
struct last<List<Head_, Args_...>> {
  using type = typename last<List<Args_...>>::type;
};

/** The initial part of the list without its last element. */
template <typename T_>
struct init {
  static_assert(non_empty_list<T_>::value, "Expected a non-empty list.");
};

template <typename Last_>
struct init<List<Last_>> {
  using type = List<>;
};

template <typename Head_, typename... Args_>
struct init<List<Head_, Args_...>> {
  using _Remain = typename init<List<Args_...>>::type;
  using type    = typename prepended<_Remain, Head_>::type;
};

/** The rest of the list without its first element. */
template <typename>
struct tail;

template <>
struct tail<List<>> {
  using type = List<>;
};

template <typename Head_, typename... Args_>
struct tail<List<Head_, Args_...>> {
  using type = List<Args_...>;
};

/** Selects the first n elements. */
template <typename L_, typename N_>
struct take {
  static_assert(non_empty_list<L_>::value, "Expected a non-empty list.");
  static_assert(is_int_<N_>::value, "Expected a Int<?> instantiation.");
};

/** Selects the first n elements. */
template <typename L_, int n>
struct take<L_, Int<n>> {
  static_assert(non_empty_list<L_>::value, "Expected a non-empty list.");
  static_assert(size<L_>::value >= n,
                "Expected a list has more than n elements.");

  using _Head   = typename head<L_>::type;
  using _Tail   = typename tail<L_>::type;
  using _Remain = typename take<_Tail, Int<n - 1>>::type;
  using type    = typename prepended<_Remain, _Head>::type;
};

template <typename L_>
struct take<L_, Int<0>> {
  static_assert(is_list<L_>::value, "Expected a list.");
  using type = List<>;
};

/** find
 * Find the position of `Element` in the given list `L_`
 */
template <typename L_, typename Element_>
struct find {};

/** map */
struct map {};

/** reduce */
struct reduce {};

/** filter */
struct filter {};

/** flat_map */
struct flat_map {};

}  // namespace list
}  // namespace base
}  // namespace crisp

#endif  // CRISP_BASE_LIST_HPP
