/**
 * @link https://www.luogu.com.cn/problem/P6221
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_7647931536201714 {
    constexpr const int mod = 1e9+7;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        std::vector s(N, std::vector<std::string>(K));
        std::vector F(N, std::vector<int>(K));

        for (auto &line: s) {
            for (auto &item: line) {
                std::cin >> item;
            }
        }
        ranges::fill(F[0], 1);
        for (auto i: range(1, N)) {
            for (auto j: range(0, K)) {
                for (auto k: range(0, K)) {
                    if (s[i][k].starts_with(s[i-1][j]) or s[i][k].ends_with(s[i-1][j])) {
                        F[i][k] += F[i-1][j], F[i][k] %= mod;
                    }
                }
            }
        }

        auto res = std::accumulate(F[N-1].begin(), F[N-1].end(), 0, lam(x, y, (x + y) % mod));
        std::cout << res << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7647931536201714::solve();
    return 0;
}
