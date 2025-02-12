/**
 * @link https://www.luogu.com.cn/problem/AT_abc249_e
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
using namespace lib;

/**
 * F[i][j] 表示将长度为 i 的字符串压到长度为 j 的方案数
 * 考虑转移
 * 在一个字符串后面加一个长度为 k 的连续段，
 * 原字符串长度 i - k -> i
 * 新字符串长度 j - 1 - numLen(k) -> j
 * F[i][j] <- 25 * F[i-k][j-1-len(k)]
 * 分别对于 len(k) = 1, 2, 3, 4 的情况，j - 1 - len(k) 可以看成定值
 * 先枚举 j，再枚举 i，用前缀和维护即可
 * 令 G[i][j] 表示 F[j][i]
 */
namespace Solution_4594102107456167 {
    auto numLen(int x) -> int {
        return 1 + (x >= 10) + (x >= 100) + (x >= 1000);
    }
    auto qpow(ll a, ll b, const int mod) -> int {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, P;  std::cin >> N >> P;

        std::vector G(N+1, std::vector<int>(N+1));  // G[j][i]：长度为 i -> j
        std::vector G_ps(N+1, std::vector<int>(N+1));

        G[0][0] = 1;
        std::partial_sum(G[0].begin(), G[0].end(), G_ps[0].begin());
        for (auto j = 2; j <= N; j++) {
#if false
            for (auto k = 1; k <= N; k++) {
                for (auto i = k; i <= N; i++) {
                    auto original_zipped_len = j-1-numLen(k);
                    if (original_zipped_len < 0)  continue;
                    auto letter_choice = 25 + (original_zipped_len == 0);  // 如果之前没有字母，那么有 26 种选择
                    G[j][i] = (letter_choice * G[original_zipped_len][i-k] + G[j][i]) % P;
                }
            }
#endif
            // 上面是暴力 DP
            // 考虑优化。可以发现，G[j][i] 其实只是需要一个 G[origin][k] 求和
            // origin 一共有至多四个取值，分别计算加起来即可
            // 维护 G[j] 上的前缀和，四个区间和拼起来即为所求
            for (auto i = 1; i <= N; i++) {
                // 对于 1 <= k <= i
                auto get_segment_sum = [&](int begin, int end, int k_len) -> ll {
                    auto row = j - 1 - k_len;
                    if (row < 0)  return 0;
                    assert(0 <= end - 1 and end - 1 < static_cast<int>(G_ps[row].size()));
                    return (G_ps[row][end - 1] - (begin - 1 >= 0? G_ps[row][begin - 1]: 0) + P) % P;
                };
                auto cur = 0;
                if (i >= 1)  cur = (cur + get_segment_sum(i - 9, i, 1) * 25) % P;
                if (i >= 10)  cur = (cur + get_segment_sum(i - 99, i - 9, 2) * 25) % P;
                if (i >= 100)  cur = (cur + get_segment_sum(i - 999, i - 99, 3) * 25) % P;
                if (i >= 1000)  cur = (cur + get_segment_sum(i - 9999, i - 999, 4) * 25) % P;

                G[j][i] = cur;
            }

            std::copy(G[j].begin(), G[j].end(), G_ps[j].begin());
            std::partial_sum(G_ps[j].begin(), G_ps[j].end(), G_ps[j].begin(), lam(x, y, (x + y) % P));
        }

        auto ans = 0;
        for (auto j = 1; j < N; j++) {  // 不能取等
            ans = (ans + G[j][N]) % P;
        }
        // 题解是天才。
        // 最后答案乘上 26/25 即可，因为所有情况下的第一步都少算了一种选择。
        auto inv_25 = qpow(25, P-2, P);
        ans = (static_cast<ll>(ans) * 26 % P) * inv_25 % P;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4594102107456167::solve();
    return 0;
}
