/*
 * Copyright 2019 by Crisp Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CRISP_INTERPRETLIST_HPP
#define CRISP_INTERPRETLIST_HPP
#include "Common.hpp"

namespace crisp {
using namespace crisp;
using namespace util;
using util::internal::InternalList;

template <typename Environ, typename... Elements>
struct Interpret<List<Elements...>, Environ> {
  using ElementsInterp = Interpret<InternalList<Elements...>, Environ>;
  using ElementsValue = typename ElementsInterp::type;

  using env = Environ;
  using type = typename Convert<ElementsValue, List>::type;
};

template <typename Environ, typename LeftList, typename RightList>
struct Interpret<Concat<LeftList, RightList>, Environ> {
  using LeftInterp = Interpret<LeftList, Environ>;
  using RightInterp = Interpret<RightList, Environ>;

  using LeftListValue = typename LeftInterp::type;
  using RightListValue = typename RightInterp::type;

  static_assert(IsTemplateOf<List, LeftListValue>::value,
                "the input type is not a List<...> instaniation");
  static_assert(IsTemplateOf<List, RightListValue>::value,
                "the input type is not a List<...> instaniation");

  using env = Environ;
  using type = typename ListConcat<LeftListValue, RightListValue>::type;
};

#define InterpretNonEmptyListUnaryOperation(OpName)                  \
  template <typename Environ, typename L>                            \
  struct Interpret<OpName<L>, Environ> {                             \
    using LInterp = Interpret<L, Environ>;                           \
    using ListValue = typename LInterp::type;                        \
                                                                     \
    static_assert(IsTemplateOf<List, ListValue>::value,              \
                  "the input type is not a List<...> instaniation"); \
    static_assert(!IsEmptyImpl<ListValue>::value,                    \
                  "cannot apply `" #OpName "` on an empty list");    \
                                                                     \
    using env = Environ;                                             \
    using type = typename List##OpName<ListValue>::type;             \
  };

InterpretNonEmptyListUnaryOperation(Head);
InterpretNonEmptyListUnaryOperation(PopHead);
InterpretNonEmptyListUnaryOperation(PopLast);
InterpretNonEmptyListUnaryOperation(DropHead);
InterpretNonEmptyListUnaryOperation(DropLast);

#define InterpretListUnaryOperation(OpName)                          \
  template <typename Environ, typename L>                            \
  struct Interpret<OpName<L>, Environ> {                             \
    using LInterp = Interpret<L, Environ>;                           \
    using ListValue = typename LInterp::type;                        \
                                                                     \
    static_assert(IsTemplateOf<List, ListValue>::value,              \
                  "the input type is not a List<...> instaniation"); \
                                                                     \
    using env = Environ;                                             \
    using type = typename List##OpName<ListValue>::type;             \
  };

InterpretListUnaryOperation(Tail);

#define InterpretListBinaryOperation(OpName)                         \
  template <typename Environ, typename L, typename E>                \
  struct Interpret<OpName<L, E>, Environ> {                          \
    using LInterp = Interpret<L, Environ>;                           \
    using EInterp = Interpret<E, Environ>;                           \
    using ListValue = typename LInterp::type;                        \
    using ExprValue = typename EInterp::type;                        \
                                                                     \
    static_assert(IsTemplateOf<List, ListValue>::value,              \
                  "the input type is not a List<...> instaniation"); \
                                                                     \
    using env = Environ;                                             \
    using type = typename List##OpName<ListValue, ExprValue>::type;  \
  };

InterpretListBinaryOperation(PushHead);
InterpretListBinaryOperation(PushLast);
}  // namespace crisp

#endif  // CRISP_INTERPRETLIST_HPP
