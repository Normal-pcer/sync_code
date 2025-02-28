/**
 * @link https://www.luogu.com.cn/problem/P6005
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
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
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * F[i][j] 表示在第 i 天到达点 j 能够获取的最大收益
 * 直接转移即可。
 * 天数不会太多，(2T - 1) * C > max{m} 时就绝无可能获得更高收益了。
 */
namespace Solution_2443595398376977 {
    void solve() {
        i32 N, M, C;  std::cin >> N >> M >> C;
        std::vector<i32> m(N + 1);
        for (auto &x: m | views::drop(1))  std::cin >> x;

        auto days_limit = (ranges::max(m) / C + 1) / 2 + 2;  // 只需要到这个天数
        std::vector F(days_limit, std::vector<i32>(N + 1, -inf));

        std::vector<std::vector<i32>> graph(N + 1);
        for (i32 _ = 0; _ < M; _++) {
            i32 x, y;  std::cin >> x >> y;
            graph[x].push_back(y);
        }

        F[0][1] = 0;
        for (i32 i = 0; i < days_limit - 1; i++) {
            for (i32 j = 1; j <= N; j++) {
                for (auto next: graph[j]) {
                    chkMax(F[i + 1][next], F[i][j] + m[next]);
                }
            }
        }

        i64 ans = 0;
        for (i32 i = 0; i < days_limit; i++) {
            chkMax(ans, F[i][1] - static_cast<i64>(C) * i * i);
        }
        std::cout << ans << endl;
    }
}

int main(int dyy_forever, char const *lmy_forever[]) {
    DEBUG_MODE = (dyy_forever-1) and not strcmp("-d", lmy_forever[1]);
    Solution_2443595398376977::solve();
    return 0;
}
