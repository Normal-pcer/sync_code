/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6314180276493067 {
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<i32> a(n + 1);
        for (i32 i = 1; i <= n; i++) std::cin >> a[i];
        
        for (auto _ = m; _ --> 0; ) {
            char op;
            std::cin >> op;
            if (op == 'Q') {
                i32 l, r, k;
                std::cin >> l >> r >> k;
                std::vector<i32> cp(a.begin() + l, a.begin() + r + 1);

                std::nth_element(cp.begin(), cp.begin() + k - 1, cp.end());
                std::cout << *(cp.begin() + k - 1) << endl;
            } else {
                i32 x, y;
                std::cin >> x >> y;
                a[x] = y;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6314180276493067::solve();
    return 0;
}
