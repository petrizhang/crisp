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

#ifndef CRISP_MAP_HPP
#define CRISP_MAP_HPP

#include "ast/CoreAST.hpp"
#include "core/CrispFunction.hpp"

#define map(l, f) crisp::Map<l, f>

namespace crisp {

template <typename, typename>
struct Map : CrispFunction {
  using __name__ = var("map");
  using __body__ = lambda(params(var("list"), var("func")),
                          if_(is_empty(var("list")),
                              list(),
                              block(define(var("func(head)"), call(var("func"), head(var("list")))),
                                    push_head(call(var("map"), tail(var("list")), var("func")),
                                              var("func(head)")))));
};

}  // namespace crisp

#endif  //CRISP_MAP_HPP
