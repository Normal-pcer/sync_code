#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "brike"

/*
*/
namespace Solution_1585286303122099 {
    auto solveForce() -> void {
    }
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<std::vector<i32>> mat(n);
        for (i32 i = 0; i < n; i++) {
            mat[i].resize(n - i);
            for (auto &x: mat[i]) {
                std::cin >> x;
            }
        }

        using Stat = u32;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    Solution_1585286303122099::solve();
    return 0;
}
