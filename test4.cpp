#include <vector>

struct A {
    int x = 2, y = 5;
};

int main() {
    std::vector<A> v;
    v.push_back({0, 2});
    return 0;
}