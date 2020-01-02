/*
 * Copyright 2019 by Crisp Project
 *
 * Licensed under the Apache License, version 2.0 (the "License");
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

#ifndef CRISP_DUMPTOLISP_HPP
#define CRISP_DUMPTOLISP_HPP

#include <iostream>

#include "crisp/ast/CoreAST.hpp"

namespace crisp {

template <typename...>
struct DumpList {};

template <class T>
struct DumpToLisp;

template <>
struct DumpToLisp<Nil> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << "nil";
  }
};

template <>
struct DumpToLisp<Undefined> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << "undefined";
  }
};

template <bool v>
struct DumpToLisp<Bool<v>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    if (v) {
      os << "true";
    } else {
      os << "false";
    }
  }
};

template <char v>
struct DumpToLisp<Char<v>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << "'" << v << "'";
  }
};

template <int v>
struct DumpToLisp<Int<v>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << v;
  }
};

template <char... chars>
struct DumpToLisp<Str<chars...>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << "\"" << Str<chars...>::value << "\"";
  }
};

template <char... chars>
struct DumpToLisp<Var<chars...>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << Str<chars...>::value;
  }
};

template <template <typename...> class T, typename... Args>
struct DumpToLisp<T<Args...>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << "\n";
    for (int i = 0; i < indent; i++) {
      os << " ";
    }

    os << "(" << T<Args...>::repr() << " ";
    DumpToLisp<DumpList<Args...>>::dump(os, unit, indent + unit);
    os << ")";
  }
};

template <typename... Args>
struct DumpToLisp<Call<Args...>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << "\n";
    for (int i = 0; i < indent; i++) {
      os << " ";
    }

    os << "(";
    DumpToLisp<DumpList<Args...>>::dump(os, unit, indent + unit);
    os << ")";
  }
};

template <typename... Args>
struct DumpToLisp<ParamList<Args...>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    os << "\n";
    for (int i = 0; i < indent; i++) {
      os << " ";
    }

    os << "(";
    DumpToLisp<DumpList<Args...>>::dump(os, unit, indent + unit);
    os << ")";
  }
};

template <>
struct DumpToLisp<DumpList<>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {}
};

template <typename T>
struct DumpToLisp<DumpList<T>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    DumpToLisp<T>::dump(os, unit, indent);
  }
};

template <typename T, typename... Args>
struct DumpToLisp<DumpList<T, Args...>> {
  static void dump(std::ostream &os, int unit = 2, int indent = 0) {
    DumpToLisp<T>::dump(os, unit, indent);
    os << " ";
    DumpToLisp<DumpList<Args...>>::dump(os, unit, indent);
  }
};

}  // namespace crisp

#endif  //CRISP_DUMPTOLISP_HPP
