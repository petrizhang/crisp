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

#ifndef CRISP_OPERATOR_IMPL_HPP
#define CRISP_OPERATOR_IMPL_HPP

#include "interpreter/AST.hpp"

namespace util {
using namespace ast;

/**
 * Implementation for `Add`.
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct AddImpl {
  static_assert(Error<L, R>::always_false, "Incompatible types for operation `Add`.");
};

template <int LV, int RV>
struct AddImpl<Int<LV>, Int<RV>> {
  using type = Int<LV + RV>;
};

/**
 * Most binary operators (-,*,%,...) follow the same pattern as * `AddImpl`,
 * thus we implement them with an unified macro.
 */
#define BinaryOperator(OpName, Operator, LeftValueType, LeftType, RightValueType, RightType, \
                       ResultType)                                                           \
  template <typename L, typename R>                                                          \
  struct OpName##Impl {                                                                      \
    static_assert(Error<L, R>::always_false,                                                 \
                  "Incompatible types for operation `" #OpName "`.");                        \
  };                                                                                         \
                                                                                             \
  template <LeftValueType LV, RightValueType RV>                                             \
  struct OpName##Impl<LeftType<LV>, RightType<RV>> {                                         \
    using type = ResultType<(LV Operator RV)>;                                               \
  };

BinaryOperator(Sub, -, int, Int, int, Int, Int);
BinaryOperator(Mul, *, int, Int, int, Int, Int);
BinaryOperator(Mod, %, int, Int, int, Int, Int);
BinaryOperator(And, &&, bool, Bool, bool, Bool, Bool);
BinaryOperator(Or, ||, bool, Bool, bool, Bool, Bool);
BinaryOperator(IsGreaterThan, >, int, Int, int, Int, Bool);
BinaryOperator(IsLessThan, <, int, Int, int, Int, Bool);
BinaryOperator(IsGreaterEqual, >=, int, Int, int, Int, Bool);
BinaryOperator(IsLessEqual, <=, int, Int, int, Int, Bool);

/**
 * Implementation for `IsEqual`.
 * Check if two types are the same.
 * @tparam L
 * @tparam R
 */
template <typename L, typename R>
struct IsEqualImpl {
  using type = Bool<std::is_same<L, R>::value>;
};

}  // namespace util

#endif  //CRISP_OPERATOR_IMPL_HPP
