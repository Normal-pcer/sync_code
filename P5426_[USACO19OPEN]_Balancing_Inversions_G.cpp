/**
 * @link https://www.luogu.com.cn/problem/P5426
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

/*
假设 seq 序列中全部“1”的下标为 a[i]（1-index），共有 x 个。
可知它的逆序对数量为：

*/
namespace Solution_4832905901407351 {
namespace {
    auto solve() -> void {
        i32 n{}; std::cin >> n;

        std::vector<i32> seq(n + n);
        for (auto &x: seq) std::cin >> x;

    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4832905901407351::solve();
    return 0;
}
