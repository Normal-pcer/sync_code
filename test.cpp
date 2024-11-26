/**
 * 
 */

#include "./lib"

using namespace lib;

namespace Solution_4710779360095706 {

    template <typename T>
    inline T min(T x) { return x; }

    template <typename T, typename ...tps>
    inline T min(T x, tps ...args) {
        auto y = min(args...);
        return x>y? y: x;
    }

    void solve() {
        std::cout << min(1, 2, 3, 4, 5, 6, 7, 8) << std::endl;
    }
}

int main() {
    initDebug;
    Solution_4710779360095706::solve();
    return 0;
}
