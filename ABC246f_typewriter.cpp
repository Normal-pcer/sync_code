/**
 * @link https://www.luogu.com.cn/problem/AT_abc246_f
 * @link https://atcoder.jp/contests/abc246/tasks/abc246_f
 */

#include "./lib_v3.hpp"

#include "./libs/bit.hpp"

using namespace lib;

namespace Solution_1697270129743971 {
    constexpr const int mod = 998244353;
    ll pow(ll a, ll b, const int mod) {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, L;  std::cin >> N >> L;
        std::vector<unsigned> masks(N);
        for (auto &x: masks) {
            std::string s;  std::cin >> s;
            for (auto ch: s)  x |= 1U << (ch - 'a');
        }

        auto ans = 0LL;
        // 枚举子集
        for (auto sub = 1U; sub < (1U << N); sub++) {
            debug  std::cout << sub << std::endl;
            if (sub == 0)  return;
            // 对于这些的交集
            auto letters = (1U << 26) - 1;
            for (auto i = 0; i < N; i++) {
                if (sub & (1U << i))  letters &= masks[i];
            }
            auto cnt = std::popcount(sub);
            bool nega = not (cnt & 1);  // 这一项的正负
            auto cur = pow(std::popcount(letters), L, mod);
            if (nega)  cur = -cur;
            ans = (ans + cur + mod) % mod;
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1697270129743971::solve();
    return 0;
}
