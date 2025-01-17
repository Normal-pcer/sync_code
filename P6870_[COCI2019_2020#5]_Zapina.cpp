/**
 * @link https://www.luogu.com.cn/problem/P6870
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1152672094401331 {
    constexpr const int mod = 1'000'000'007;
    constexpr auto pow(ll a, ll b, const int mod) -> ll {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        // F[i][j] 表示分配前 i 个人、前 j 个题，至少有一个人“开心”
        // 此时的方案数
        std::vector F(N+1, std::vector<int>(N+1));
        std::vector<std::vector<int>> C(N+1);
        C[1] = {1, 1};
        for (auto y: range(1, N+1)) {
            C[y].resize(y+1);
            C[y][0] = C[y][y] = 1;
            for (auto j: range(1, y)) {
                C[y][j] = (C[y-1][j] + C[y-1][j-1]) % mod;
            }
        }
        F[1][1] = 1;
        for (auto i: range(2, N+1)) {
            for (auto j: range(1, N+1)) {
                // 如果第 i 个人满意
                // 随意分配 i 道题目给他，剩下的题目也随意分配给其他人
                if (j >= i) {
                    F[i][j] += static_cast<ll>(C[j][i]) * pow(i-1, j-i, mod) % mod;
                    F[i][j] %= mod;
                }
                // 给第 i 个人分配任意数量
                // 保证剩余的有人满意
                for (auto k: range(0, j+1)) {
                    if (k == i)  continue;
                    F[i][j] += static_cast<ll>(C[j][k]) * F[i-1][j-k] % mod;
                    F[i][j] %= mod;
                }
            }
        }
        auto ans = F[N][N];
        debug {
            std::cout << "F: ";
            for (auto const &line: F) {
                for (auto item: line) {
                    std::cout << item << " ";
                }
                std::cout << endl;
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1152672094401331::solve();
    return 0;
}
