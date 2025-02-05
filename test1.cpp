#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    std::vector vec{0, 1, 2, 3, 4};

    auto it = std::ranges::lower_bound(vec.begin(), vec.end(), 1, std::less{}, [](int x) { return x - 2; });
    std::cout << std::distance(vec.begin(), it) << std::endl;
    return 0;
}