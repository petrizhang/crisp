#ifndef CRISP_CORE_LIST_HPP
#define CRISP_CORE_LIST_HPP

#include <type_traits>

#include "common.hpp"
#include "crisp/core/common.hpp"
#include "crisp/core/lambda.hpp"

namespace crisp {
namespace core {
namespace list {
using namespace crisp::core;

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

  using type = Int<sizeof...(Args_)>;
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
  using _Rest = typename init<List<Args_...>>::type;
  using type  = typename prepended<_Rest, Head_>::type;
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

  using _Head = typename head<L_>::type;
  using _Tail = typename tail<L_>::type;
  using _Rest = typename take<_Tail, Int<n - 1>>::type;
  using type  = typename prepended<_Rest, _Head>::type;
};

template <typename L_>
struct take<L_, Int<0>> {
  static_assert(is_list<L_>::value, "Expected a list.");
  using type = List<>;
};

/** Builds a new list by applying a function to all elements of this list. */
template <typename L_, typename Lambda_>
struct map {
  static_assert(is_list<L_>::value, "Expected a list.");
  static_assert(is_lambda<Lambda_>::value, "Expected a lambda instantiation.");
};

template <typename Lambda_>
struct map<List<>, Lambda_> {
  using type = List<>;
  static_assert(is_lambda<Lambda_>::value, "Expected a lambda instantiation.");
};

template <typename Head_, typename... Tail_, typename Lambda_>
struct map<List<Head_, Tail_...>, Lambda_> {
  static_assert(is_lambda<Lambda_>::value, "Expected a lambda instantiation.");
  using _HeadResult = typename Lambda_::template apply<Head_>::type;
  using _TailResult = typename map<List<Tail_...>, Lambda_>::type;
  using type        = typename prepended<_TailResult, _HeadResult>::type;
};

/** Reduces the elements of this collection using the specified associative
 * binary operator. */
template <typename L_, typename Lambda_>
struct reduce {
  static_assert(is_list<L_>::value, "Expected a list.");
  static_assert(size<L_>::value >= 2,
                "Expected a list has at least 2 elements.");
  static_assert(is_lambda<Lambda_>::value, "Expected a lambda instantiation.");
};

template <typename E0_, typename E1_, typename Lambda_>
struct reduce<List<E0_, E1_>, Lambda_> {
  static_assert(is_lambda<Lambda_>::value, "Expected a lambda instantiation.");
  using type = typename Lambda_::template apply<E0_, E1_>::type;
};

template <typename E0_, typename E1_, typename... Elements_, typename Lambda_>
struct reduce<List<E0_, E1_, Elements_...>, Lambda_> {
  static_assert(is_lambda<Lambda_>::value, "Expected a lambda instantiation.");
  using _FirstResult = typename Lambda_::template apply<E0_, E1_>::type;
  using type = typename reduce<List<_FirstResult, Elements_...>, Lambda_>::type;
};

/** filter */
struct filter {};

/** flat_map */
struct flat_map {};

/** find
 * Find the position of `Element` in the given list `L_`
 */
template <typename L_, typename Element_>
struct find {};

}  // namespace list
}  // namespace core
}  // namespace crisp

#endif  // CRISP_CORE_LIST_HPP
