/**
 * @link https://www.luogu.com.cn/problem/AT_abc253_e
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_1766575181289681 {
    constexpr const int mod = 998244353;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        std::vector F(N, std::vector<ll>(M+1));
        
        ranges::fill(F[0], 1), F[0][0] = 0;
        for (auto i: range(1, N)) {
            std::vector<ll> ps(F[i-1].begin(), F[i-1].end());
            std::partial_sum(ps.begin(), ps.end(), ps.begin(), lam(x, y, (x + y) % mod));
            auto sum_bet = [&](int left, int right) -> ll {
                if (left > right)  return 0LL;
                // if (left == 0)  return ps[right] % mod;
                return ((ps[right] - ps[left - 1]) % mod + mod) % mod;
            };

            for (auto j: range(1, M+1)) {
                if (K == 0) {
                    F[i][j] = (sum_bet(1, M) + mod) % mod;
                    continue;
                }
                auto left = static_cast<ll>(j - K);
                auto right = static_cast<ll>(j + K);
                auto sum = (sum_bet(1, left) % mod + sum_bet(right, M) % mod + mod) % mod;
                F[i][j] = sum;
                // debug  printValues(left, right, i, j, F[i][j]);
            }
            assert(F[i][0] == 0);
        }
        auto ans = std::accumulate(F[N-1].begin() + 1, F[N-1].end(), 0LL, lam(x, y, (x + y) % mod)) % mod;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1766575181289681::solve();
    return 0;
}
