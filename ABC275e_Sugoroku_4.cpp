/**
 * @link https://www.luogu.com.cn/problem/AT_abc275_e
 */

#include "./lib_v3.hpp"

using namespace lib;

/**
 * 某个时刻走到某个点会有一个概率
 * 设 F[i][j] 表示在第 i 步走到点 j 的概率
 * F[i][j] += F[i-1][j-k] * 1/M
 * 直接转移即可
 */
namespace Solution_1686497379493792 {
    constexpr const int mod = 998244353;
    constexpr auto qpow(ll a, ll b, const int mod) -> int {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1)  res = res * a % mod;
        return res;
    }
    constexpr auto inv(ll a, const int mod) -> int {
        return qpow(a, mod - 2, mod);
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        auto inv_M = inv(M, mod);
        std::vector F(K+1, std::vector(N+1, 0));  // 第 i 步走到 j 点的概率
        F[0][0] = 1;  // 初始在 0 点

        for (auto i = 0; i < K; i++) {
            for (auto j = 0; j < N; j++) {
                for (auto k = 1; k <= M; k++) {
                    auto new_pos = j + k;
                    if (new_pos > N)  new_pos = N - (new_pos - N);

                    F[i+1][new_pos] = (F[i+1][new_pos] + (static_cast<ll>(F[i][j]) * inv_M % mod)) % mod;
                }
            }
        }

        auto ans = 0;
        for (auto i = 1; i <= K; i++) {
            ans = (ans + F[i][N]) % mod;
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1686497379493792::solve();
    return 0;
}
