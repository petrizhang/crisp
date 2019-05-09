//
// Created by pengcheng on 5/7/19.
//

#ifndef TISPP_HPP
#define TISPP_HPP


namespace tispp {

#include "type_traits"

    namespace utils {
        /**
         * Fetch return type of a function.
         * e.g. ReturnType<int(int)> = int;
         * @tparam F
         * @tparam Args
         */
        template<class F, class ...Args>
        struct ReturnType;

        template<class F, class ...Args>
        struct ReturnType<F(Args...)> {
            typedef F type;
        };


        /**
         * Fetch type of the first parameter of a function.
         * e.g. ReturnType<int(int)> = int;
         * @tparam F
         * @tparam Args
         */
        template<class F>
        struct ParamType;

        template<class F, class Arg>
        struct ParamType<F(Arg)> {
            typedef Arg type;
        };
    }

    using namespace utils;

    namespace types {
        struct Value {
        };

        struct Functor {
        };

        /**
         *
         * @tparam V
         */
        template<bool V>
        struct Bool : Value {
            typedef bool value_type;
            static const bool value = V;
            static constexpr const char *type_name = "boll";

            struct apply {
                typedef Bool<V> type;
            };
        };

        /**
         *
         * @tparam V
         */
        template<char V>
        struct Char : Value {
            typedef char value_type;
            static const char value = V;
            static constexpr const char *type_name = "char";

            struct apply {
                typedef Char<V> type;
            };
        };

        /**
         * Int type which wraps a integer value.
         * @tparam V
         */
        template<int V>
        struct Int : Value {
            typedef int value_type;
            static const int value = V;
            static constexpr const char *type_name = "int";

            struct apply {
                typedef Int<V> type;
            };
        };

        /**
         * Pack a const value(int/char/bool) to Value type.
         * e.g. PackToValue<int, 1>::packed_type will be Value<Int<1>>
         * @tparam T
         * @tparam V
         */
        template<class T, T V>
        struct PackToType;

        template<bool V>
        struct PackToType<bool, V> {
            typedef Bool<V> type;
        };

        template<char V>
        struct PackToType<char, V> {
            typedef Char<V> type;
        };

        template<int V>
        struct PackToType<int, V> {
            typedef Int<V> type;
        };
    }

    using namespace types;

    namespace builtin {

#define DefineBinaryOperation(BinaryOpName, op) \
        template<class L, class R> \
        struct BinaryOpName : Functor { \
            typedef decltype(L::value op R::value) value_type; \
            typedef class PackToType<value_type, L::value op R::value>::type type; \
        };

#define DefineListOperation(OpName, BinaryOpName) \
        template<class T1, class T2, class ...Params> \
        struct OpName; \
        \
        template<class T1, class T2> \
        struct OpName<T1, T2> : BinaryOpName<T1,T2> { \
        };

#define CreateFunctionForOperation(FunctionName, OpName) ;
//        struct FunctionName##Impl { \
//            template<class ...Args> \
//            auto operator()(Args... args) const { \
//
//            } \
//        }; \
//        \
//        static constexpr const FunctionName##Impl FunctionName;


        DefineBinaryOperation(BinaryPlus, +)
        DefineBinaryOperation(BinaryMinus, -)
        DefineBinaryOperation(BinaryMul, *)
        DefineBinaryOperation(BinaryDiv, /)
        DefineBinaryOperation(BinaryAnd, &&)
        DefineBinaryOperation(BinaryOr, ||)

        DefineListOperation(Plus, BinaryPlus)
        DefineListOperation(Minus, BinaryMinus)
        DefineListOperation(Mul, BinaryMul)
        DefineListOperation(Div, BinaryDiv)
        DefineListOperation(And, BinaryAnd)
        DefineListOperation(Or, BinaryOr)

        CreateFunctionForOperation(plus, Plus)

        CreateFunctionForOperation(minus, Minus)

        CreateFunctionForOperation(mul, Mul)

        CreateFunctionForOperation(div, Div)

        CreateFunctionForOperation(_and, And)

        CreateFunctionForOperation(_or, Or)
    }

    using namespace builtin;

    namespace api {

#define call(f, args...)  ReturnType<decltype(f)>::type<args>::type;

        /**
         *
         * @tparam T
         */
        template<class T>
        struct Eval {
            typedef class T::apply::type type;
        };

    }

    using namespace api;
}


#endif //TISPP_HPP
