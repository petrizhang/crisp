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

#ifndef CRISP_INTERPRETINTERNALLIST_HPP
#define CRISP_INTERPRETINTERNALLIST_HPP

#include "Common.hpp"

namespace crisp {
using util::internal::InternalList;
using util::internal::InternalListPushFront;

/**
 * Interpret parameters packed in a InternalList<...>.
 * It is internally used in Interpreter for function parameters evaluation.
 * @tparam Environ
 */
template <typename Environ>
struct Interpret<InternalList<>, Environ> {
  using env = Environ;
  using type = InternalList<>;

  static constexpr const char *Run() { return "#undefined"; }
};

template <typename Environ, typename Head, typename... Tail>
struct Interpret<InternalList<Head, Tail...>, Environ> {
  using HeadInterp = Interpret<Head, Environ>;
  using HeadValue = typename HeadInterp::type;

  using TailInterp = Interpret<InternalList<Tail...>, Environ>;
  using TailValue = typename TailInterp::type;

  using env = Environ;
  using type = typename InternalListPushFront<TailValue, HeadValue>::type;

  static auto Run() {
    HeadInterp::Run();
    TailInterp::Run();
    return "#undefined";
  }
};

}  // namespace crisp

#endif  //CRISP_INTERPRETINTERNALLIST_HPP
