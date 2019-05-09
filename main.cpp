#include <iostream>
#include <functional>
#include "tispp.hpp"

using namespace tispp;

//#define s(a, ...) ;
//#define call(f, args...) Call<f,args>
//#define lambda(f, ...) ;
#define let(name, body) typedef PackToType<decltype(body), body>::type name
//#define let(name, body) typedef Eval<body>::type name
//
//#define def(name, body) ;
//#define display(v) ;
//#define q(a, b) (a+b)


#define temp(v, args...) ;

template<auto ...Args>
struct A {
    static const int value = 0;
};

template<typename T>
constexpr auto pack(T &&v) {
    constexpr auto x = v;
    return Int<std::integral_constant<T, v>::value>();
}

template<int ...Args>
struct F;

template<int N, int...Remain>
struct F<N, Remain...> {
    static const int value = N + F<Remain...>::value;
};

template<int N>
struct F<N> {
    static const int value = N;
};

//struct Q {
//    template<class ...Args>
//    auto operator()(Args &&... args) const {
//        return F<args...>::value;
//    }
//};
//
//static constexpr const Q q;

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::cout << A<5,Int<1>::value>::value;
    return 0;
}