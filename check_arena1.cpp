#include <cstdint>
#include <iostream>
#include <set>
#include <vector>

using i32 = std::int32_t; using i64 = std::int64_t;

namespace Solution {
namespace {

    auto solve() -> void {
        i32 n, q; std::cin >> n >> q;
        
        std::vector<std::pair<i32, i32>> adds;
        std::vector<std::set<i32>> graph(n + 1);
        std::vector<std::uint8_t> active(n + 1, true);

        adds.reserve(q + 1);
        adds.emplace_back();  // adds: 1-index

        // 每个点，它所在的连通块是否存在活跃点
        auto calc = [&]() -> std::vector<std::uint8_t> {
            std::vector<std::uint8_t> res(n + 1);
            std::vector<std::uint8_t> vis(n + 1);

            auto flood = [&](auto &flood, i32 p) -> void {
                if (vis[p]) return;
                vis[p] = true, res[p] = true;
                for (auto to: graph[p]) {
                    flood(flood, to);
                }
            };

            for (i32 i = 1; i <= n; ++i) {
                if (active[i]) flood(flood, i);
            }

            return res;
        };

        std::vector<std::vector<std::uint8_t>> history;
        history.reserve(q + 1);
        history.emplace_back();

        for (auto _ = 0; _ < q; ++_) {
            char op; std::cin >> op;

            if (op == 'A') {
                i32 u, v;
                std::cin >> u >> v;
                graph[u].insert(v);
                graph[v].insert(u);

                adds.emplace_back(u, v);
            } else if (op == 'R') {
                i32 index;
                std::cin >> index;

                auto [u, v] = adds[index];
                graph[u].erase(v);
                graph[v].erase(u);
            } else {
                i32 p; std::cin >> p;
                active[p] = false;
            }

            history.emplace_back(calc());
        }

        std::vector<i32> ans(n + 1);
        for (i32 i = 1; i <= q; ++i) {
            for (i32 p = 1; p <= n; ++p) {
                if (history[i][p]) ans[p] = i;
            } 
        }
        for (i32 i = 1; i <= n; ++i) std::cout << ans[i] << '\n';
    }
}
}

auto main() -> int {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
}