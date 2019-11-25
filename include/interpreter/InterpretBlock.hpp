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

#ifndef CRISP_INTERPRETBLOCK_HPP
#define CRISP_INTERPRETBLOCK_HPP
#include "Common.hpp"

namespace crisp {
using namespace ast;
using namespace util;

/**
 * Interpret a block expression which contains a sequence of expressions.
 * e.g Block< Define<Var<'n'>,1>,
 *            Var<'n'>>.
 * The result of a block is the result of the last expression in this block.
 *
 * @tparam Environ
 * @tparam Head
 * @tparam Tail
 */
template <typename Environ, typename Head, typename... Tail>
struct Interpret<Block<Head, Tail...>, Environ> {
  using env = Environ;

  using HeadInterp = Interpret<Head, Environ>;
  // Pass the resulted env from `Head` to the next expression's execution
  using TailInterp = Interpret<Block<Tail...>, typename HeadInterp::env>;
  using type = typename TailInterp::type;

  static decltype(type::c_value()) Run() {
    HeadInterp::Run();
    TailInterp::Run();
    return type::c_value();
  }
};

template <typename Environ, typename Head>
struct Interpret<Block<Head>, Environ> {
  using HeadInterp = Interpret<Head, Environ>;

  using env = typename HeadInterp::env;
  using type = typename HeadInterp::type;

  static decltype(type::c_value()) Run() {
    HeadInterp::Run();
    return type::c_value();
  }
};
}  // namespace crisp
#endif  //CRISP_INTERPRETBLOCK_HPP
