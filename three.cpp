/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "three"
namespace Solution_6138187558929015 {
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<i32> a(n);
        for (auto &x: a) std::cin >> x;

        std::sort(a.begin(), a.end());

        i64 ans = 0;
        for (i32 i = 0; i < n; i++) {
            for (i32 j = i + 1; j < n; j++) {
                auto k = a[j] - a[i];
                auto minT = a[j] + k;
                auto maxT = a[j] + k + k;
                auto first = std::lower_bound(a.begin(), a.end(), minT);
                auto last = std::upper_bound(a.begin(), a.end(), maxT);
                ans += std::distance(first, last);
            }
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6138187558929015::solve();
    return 0;
}
