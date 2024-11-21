/**
 * 
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_4153386203017484 {

    std::mt19937 rnd(745184);

    void solve() {
        ull res = 0ULL;
#if true
        for (auto _: range(1LL, 1'000'000'001LL)) {
            res ^= rnd();
        }
#else
        for (auto _ = 1LL; _ < 1'000'000'001LL; _++) {
            res ^= rnd();
        }
#endif
        std::cout << res << std::endl;
    }
}

int main() {
    initDebug;
    Solution_4153386203017484::solve();
    return 0;
}