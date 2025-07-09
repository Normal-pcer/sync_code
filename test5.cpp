#include <algorithm>
#include <vector>

auto main() -> int {
    std::vector<int> vec{1, 2, 3, 4, 5};

    int sum{};
    struct Func {
        int &sum;
        auto operator() (int x) -> void {
            sum += x;
        }
    };
    Func func{sum};

    std::for_each(vec.begin(), vec.end(), func);

    return 0;
}