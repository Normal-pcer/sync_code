/**
 * @link https://www.luogu.com.cn/problem/P4310
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_3491323731760918 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> a(N);
        for (auto &x: a)  std::cin >> x;

        std::array<int, std::numeric_limits<int>::digits> max1;
        max1.fill(0);
        for (auto x: a) {
            auto cur = 0;
            for (auto j: range(std::numeric_limits<int>::digits)) {
                auto mask = 1U << j;
                if (mask & x)  chkMax(cur, max1.at(j));
            }
            cur++;
            for (auto j: range(std::numeric_limits<int>::digits)) {
                auto mask = 1U << j;
                if (mask & x)  chkMax(max1.at(j), cur);
            }
        }
        auto ans = ranges::max(max1);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3491323731760918::solve();
    return 0;
}
