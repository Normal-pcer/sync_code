/**
 * @link https://www.luogu.com.cn/problem/P2401
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_3312516583630100 {
    const int mod = 2015;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        std::vector F(N+1, std::vector<int>(K+1));

        F[1][0] = 1;
        for (auto i: range(2, N+1)) {
            for (auto k: range(0, K+1)) {
                F[i][k] = F[i-1][k] * (1 + k) % mod;
                if (k != 0)  F[i][k] += F[i-1][k-1] * (i - k) % mod;
                F[i][k] %= mod;
            }
        }
        auto ans = F[N][K];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3312516583630100::solve();
    return 0;
}
