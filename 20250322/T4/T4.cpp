#include <bits/stdc++.h>
#define FILE_NAME "T4"
#ifdef LOCAL
#define debug if (true)
#else  // not def LOCAL
#define debug if (false)
#endif  // def LOCAL

using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::int16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;

namespace Solution {
    char constexpr endl = '\n';
    auto solve() -> void {
        i32 N; std::cin >> N;
        std::vector<i32> w(N + 1);
        for (i32 i = 1; i <= N; i++) std::cin >> w[i];
        std::vector<std::vector<i32>> graph(N + 1);
        for (i32 _ = N - 1; _ --> 0; ) {
            i32 x, y;  std::cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x);
        }

        auto walk = [&](i32 u) -> std::vector<char> {
            auto c = w[u];
            std::deque<i32> q;
            q.push_back(u);
            std::vector<char> vis(N + 1);
            std::vector<char> ans(N + 1);
            while (not q.empty()) {
                auto x = q.front(); q.pop_front();
                if (vis[x]) continue;
                vis[x] = true;
                for (auto next: graph[x]) {
                    if (not vis[next]) {
                        if (w[next] == c) ans[next] = true;
                        else q.push_back(next);
                    }
                }
            }
            return ans;
        };

        i64 ans = 0;
        for (i32 i = 1; i <= N; i++) {
            auto cur_ans = walk(i);
            ans += std::count(cur_ans.begin() + i + 1, cur_ans.end(), true);
        }
        std::cout << ans << endl;
    }
}

auto main() -> int {
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T;  std::cin >> T;
    while (T --> 0) Solution::solve();

    return 0;
}