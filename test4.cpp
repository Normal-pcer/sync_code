#include <bits/stdc++.h>

#include "./libs/__vector.hpp"

struct test {
    int x;
    test(int x = 0): x(x) {
        std::cout << std::format("test({})", x) << std::endl;
    }
    ~test() {
        std::cout << std::format("{}: ~test()", x) << std::endl;
    }
};

int main() {
    // unstd::vector<test> vec;
    // for (auto i = 0; i < 10; i++)  vec.push_back({i});
    std::vector<int> vec{1, 1, 4, 5, 1, 4};
    std::vector vec2(std::move(vec));
    for (auto i: vec)  std::cout << i << std::endl;
    return 0;
}