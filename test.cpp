#include <bits/stdc++.h>
struct Foo {
    int bar;
    Foo(int bar = 0): bar(bar) {}
    ~Foo() {
        std::cout << std::format("Delete Object {}!", bar) << std::endl;
    }
};
int main() {
    std::vector<Foo> vec {0, 1, 2, 3, 4};
    std::cout << "!" << std::endl;
    for (auto byte: std::views::iota(0ULL, sizeof(vec))) {
        int num = *((char*)(&vec) + byte);
        std::cout << std::bitset<8>(num) << ' ';
    }
    std::cout << std::endl;
    std::cout << std::format("size: {}, capacity: {}", vec.size(), vec.capacity()) << std::endl;
    return 0;
}