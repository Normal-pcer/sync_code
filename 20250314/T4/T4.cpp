#include <bits/stdc++.h>
#define FILENAME "T4"

using i32 = std::int32_t; using i64 = std::int64_t;
using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
char constexpr endl = '\n';

/**
 * 打个暴力。
 * 
 * 期望得分：7
 */
namespace Solution1 {
    auto solve() -> void {
        i32 N, Q;  std::cin >> N >> Q;
        std::vector<i32> color(N + 1);
        for (i32 i = 1; i <= N; i++)  std::cin >> color[i];

        std::vector<std::vector<i32>> graph(N + 1);
        for (i32 i = 2; i <= N; i++) {
            i32 x = i, y;  std::cin >> y;
            graph[x].push_back(y), graph[y].push_back(x);
        }

        // 从某个点开始合法的数量
        auto walk_from = [&](i32 x, i32 c1, i32 c2) -> i32 {
            std::vector<char> vis(N + 1);
            std::deque<i32> q;
            q.push_back(x);
            i64 ans = 0;
            while (not q.empty()) {
                auto x = q.front();  q.pop_front();
                ans++;
                vis[x] = true;
                for (auto next: graph[x]) {
                    if (not vis[next] and (color[next] == c1 or color[next] == c2))  q.push_back(next);
                }
            }
            return ans;
        };

        for (auto _ = Q; _ --> 0; ) {
            i32 x, y;  std::cin >> x >> y;
            i64 ans = 0;
            for (i32 i = 1; i <= N; i++) {
                if (color[i] == x or color[i] == y) {
                    ans += walk_from(i, x, y);
                }
            }
            std::cout << ans << endl;
        }
    }
}

int main() {
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution1::solve();
    return 0;
}
