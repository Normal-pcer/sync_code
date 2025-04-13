#include <iostream>
#include <vector>

struct A {
    int x;
    auto f(int y) const -> void {
        std::cout << x + y << std::endl;
    }
};

auto g() -> int {
    static int cnt = 0;
    return cnt++;
}

auto main() -> int {
    std::vector<A> v;

    v.push_back(A{5});
    v.push_back(A{4});
    v.push_back(A{3});

    v[g()].f(g());
    return 0;
}