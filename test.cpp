#include <iostream>
#include <cmath>
#include <type_traits>

template <typename T>
concept is_int = requires (T a, T b) {
    { a + b } -> std::integral;
};



auto main() -> int {
    is_int<int>;
}