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

#ifndef CRISP_CONDITIONAL_HPP
#define CRISP_CONDITIONAL_HPP

#include "Error.hpp"
#include "ast/CoreAST.hpp"

namespace util {
using crisp::Bool;
using crisp::Else;
using crisp::When;

/**
 * Get result from branches according to branches' condition.
 * It is very similar to switch statement in C++.
 * e.g.
 * @code
 * ConditionalImpl<When<Bool<true>, Int<1>>,
 *                 When<Bool<false>, Int<1>>,
 *                 Else<Int<2>> >;
 * @endcode
 *
 */
template <typename Branch1, typename Branch2, typename... Branches>
struct Conditional;

template <typename Body, typename ElseBody>
struct Conditional<When<Bool<true>, Body>, Else<ElseBody>> {
  using type = Body;
};

template <typename Body, typename ElseBody>
struct Conditional<When<Bool<false>, Body>, Else<ElseBody>> {
  using type = ElseBody;
};

template <typename Branch, typename ElseBranch>
struct Conditional<Branch, ElseBranch> {
  static_assert(Error<Branch>::always_false,
                "expected a valid `When` instantiation.");
  static_assert(Error<ElseBranch>::always_false,
                "expected a valid `Else` instantiation.");
};

template <typename Body1, typename Branch2, typename Branch3, typename... Tail>
struct Conditional<When<Bool<true>, Body1>, Branch2, Branch3, Tail...> {
  using type = Body1;
};

template <typename Body1, typename Branch2, typename Branch3, typename... Tail>
struct Conditional<When<Bool<false>, Body1>, Branch2, Branch3, Tail...> {
  using type = typename Conditional<Branch2, Branch3, Tail...>::type;
};

template <typename Branch1, typename Branch2, typename Branch3, typename... Tail>
struct Conditional<Branch1, Branch2, Branch3, Tail...> {
  static_assert(Error<Branch1>::always_false,
                "expected a valid `When` instantiation.");
};

/**
 * As same as @see ConditionalImpl,
 * except that we will expand the template context saved in `DeferApply`.
 * e.g.
 * @code
 * ConditionalApply<When<Bool<true>, LazyApply<Array, Int<1>> >,
 *                  When<Bool<false>, LazyApply<Array, Int<2>> >,
 *                  Else<LazyApply<Array, Int<3>>> >;
 * @endcode
 *
 */
template <typename Branch1, typename Branch2, typename... Branches>
struct ConditionalApply {
  using Result = typename Conditional<Branch1, Branch2, Branches...>::type;
  using type = typename Result::template apply<>::type;
};

}  // namespace util

#endif  //CRISP_CONDITIONAL_HPP
