// #include <bits/stdc++.h>
#include <iostream>

template <int x>
struct F {
    const constexpr static unsigned value = F<x-1>::value + F<x-2>::value;
};
template <>
struct F<1> {
    const constexpr static unsigned value = 1;
};
template <>
struct F<0> {
    const constexpr static unsigned value = 1;
};

int main() {
    unsigned ans = F<8917>::value;
    std::cout << ans << std::endl;
    return 0;
}