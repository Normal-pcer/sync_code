/**
 * @link https://www.luogu.com.cn/problem/P6005
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

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
