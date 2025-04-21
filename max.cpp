#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "max"

/*
每个 a[i] 和 b[i] 处都会发生一些事情
例如样例，有：
5: +2, 8: -3,
3: +2, 4: -3,
13: +2, 20: -3,
7: +2, 10: -3,

先排序再遍历取 max 即可。
*/
namespace Solution_6686914644768679 {
    auto solve() -> void {
        i32 n, x, y, z;
        std::cin >> n >> x >> y >> z;

        
        std::map<i32, i64> ops;
        for (i32 _ = n; _ --> 0; ) {
            i32 a, b;
            std::cin >> a >> b;
            ops[a] += y - x;
            ops[b + 1] += z - y;
        }
        
        i64 cur = static_cast<i64>(x) * n;
        i64 ans = cur;
        for (auto [_, value]: ops) {
            cur += value;
            chkMax(ans, cur);
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    Solution_6686914644768679::solve();
    return 0;
}
