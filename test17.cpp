#include <functional>
#include <iostream>

void f(auto x) {
    std::cout << x << '\n';
}

int main() {
    f(5);
}