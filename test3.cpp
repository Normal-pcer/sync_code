#include <bits/stdc++.h>

template <typename T>
struct A {
    T x;
    auto print() -> void {
        std::cout << x << std::endl;
    }
};

template <>
struct A<int> {
    int x;
    auto test() -> int {
        return x;
    }
};

int main() {
    A<int>{5}.print();
    return 0;
}