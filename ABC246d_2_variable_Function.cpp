/**
 * @link https://www.luogu.com.cn/problem/AT_abc246_d
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1294146099159800 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        ll N;  std::cin >> N;
        auto initial = static_cast<ll>(std::pow(static_cast<double>(N) / 4, 0.33) * 2);
        auto x = 0LL, y = initial;
        auto check = [&](ll x, ll y) -> bool {
            return (x + y) * (x * x + y * y) >= N;
        };
        auto ans = infLL;
        for (; x <= y; x++) {
            for (; x <= y; y--) {
                if (not check(x, y)) {
                    break;
                }
                chkMin(ans, (x + y) * (x * x + y * y));
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1294146099159800::solve();
    return 0;
}
