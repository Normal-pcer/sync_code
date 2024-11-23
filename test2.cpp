/**
 * 
 */

#include "./lib"

#include "./libs/__vector.hpp"

using namespace lib;

namespace Solution_1422779361421432 {

    template <typename T>
    concept is_pointer = requires (T a) {
        ++a;  *a;
    };

    template <typename T>
    concept iterable = requires (T a) {
        a.begin() != a.end();
        requires is_pointer<decltype(a.begin())>;
    };

    template <iterable T>
    void print(const T &container) {
        for (auto i: container) {
            std::cout << i << std::endl;
        }
    }

    void solve() {
        std::vector<int> vec = {1, 1, 4, 5, 1, 4, 1, 9, 1, 9, 8, 1, 0};
        unstd::vector<int> vec2(vec);
        print(vec2);
    }
}

int main() {
    initDebug;
    Solution_1422779361421432::solve();
    return 0;
}