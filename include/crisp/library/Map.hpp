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

#include "crisp/ast/CoreAST.hpp"
#include "crisp/interpreter/CrispFunction.hpp"

namespace crisp {

template <typename, typename>
struct Map : CrispFunction {
 private:
  using map    = decltype("map"_v);
  using h      = decltype("h"_v);
  using l      = decltype("l"_v);
  using func   = decltype("func"_v);
  using f_head = decltype("f_head"_v);

 public:
  using __name__ = map;
  using __body__ =
      Lambda<ParamList<l, func>,
             If<IsEmpty<l>, List<>,
                Block<Define<f_head, Call<func, Head<l>>>,
                      PushFront<Call<map, Tail<l>, func>, f_head>>>>;
};

}  // namespace crisp

#endif  // CRISP_MAP_HPP
