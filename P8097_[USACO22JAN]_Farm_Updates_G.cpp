/**
 * @link https://www.luogu.com.cn/problem/P8097
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define _lambda_1(expr) [&]() { return expr; }
#define _lambda_2(a, expr) [&](auto a) { return expr; }
#define _lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define _lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define _lambda_overload(a, b, c, d, e, ...) _lambda_##e
#define lambda(...) _lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
    namespace views = std::views;
#endif
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_1265264999774934 {
namespace {
    class DSU {
    public:
        std::vector<i32> pa;
        std::vector<i32> size;

        DSU(i32 n): pa(n + 1), size(n + 1, 1) {
            ranges::iota(pa, 0);
        }

        auto find(i32 p) -> i32 {
            if (pa[p] == p) return p;
            else return pa[p] = find(pa[p]);
        }
        auto add(i32 u, i32 v) -> void {
            auto x = find(u), y = find(v);
            if (size[u] > size[v]) std::swap(u, v);
            pa[x] = y, size[y] += size[x];
        }
    };
    using u8 = std::uint8_t;
    auto solve() -> void {
        i32 n{}, q{}; std::cin >> n >> q;

        std::vector<std::set<i32>> graph(n + 1);
        std::vector<u8> active(n + 1, true);
        std::vector<std::pair<i32, i32>> adds;

        adds.reserve(q + 1), adds.emplace_back();

        std::vector<std::tuple<char, i32, i32>> ops;
        ops.reserve(q + 1);
        for (i32 _ = 0; _ < q; ++_) {
            char op{}; std::cin >> op;
            if (op == 'D') {
                i32 p{}; std::cin >> p;
                active[p] = false;

                ops.emplace_back(op, p, 0);
            } else if (op == 'A') {
                i32 u{}, v{};
                std::cin >> u >> v;

                graph[u].insert(v);
                graph[v].insert(u);
                ops.emplace_back(op, u, v);
                adds.emplace_back(u, v);
            } else {
                i32 index{}; std::cin >> index;

                auto [u, v] = adds[index];
                graph[u].erase(v);
                graph[v].erase(u);
                ops.emplace_back(op, u, v);
            }
        }

        DSU dsu{n};
        std::vector<u8> dead(n + 1, true);
        // 处理终止状态（连通块）
        {
            std::vector<u8> vis(n + 1);
            auto flood = [&](this auto &flood, i32 p) -> void {
                if (vis[p]) return;
                vis[p] = true;

                for (auto next: graph[p]) {
                    dsu.add(p, next);
                    flood(next);
                }
            };

            for (i32 p = 1; p <= n; ++p) {
                flood(p);
                if (active[p]) dead[dsu.find(p)] = false;
            }
        }

        std::vector<i32> ans(n + 1, 0);
        for (i32 p = 1; p <= n; ++p) {
            if (not dead[dsu.find(p)]) ans[p] = q;
        }

        std::vector<u8> vis(n + 1);
        // 反向考虑过程
        for (i32 index = q; auto [op, u, v]: ops | views::reverse) {
            --index;
            auto flood = [&](this auto flood, i32 p) -> void {
                if (vis[p]) return;
                vis[p] = true, ans[p] = index;
                for (auto next: graph[p]) flood(next);
            };
            if (op == 'D') {
                // 对于一个非活跃块进行反向操作，则把它变为活跃
                auto p = dsu.find(u);
                if (dead[p]) {
                    dead[p] = false, ans[p] = index;
                    flood(p);
                }
            } else if (op == 'A') {
                // 连边操作前后，两个连通块均活跃，无需任何操作
                assert(not dead[dsu.find(u)] and not dead[dsu.find(v)]);
            } else {
                // 删除了一条道路，反向操作即联通两个块
                // 新的块是否活跃，取决于任一原块活跃
                auto x = dsu.find(u), y = dsu.find(v);
                auto dead_new = dead[x] and dead[y];
                if (dead[x] and not dead_new) flood(x);
                if (dead[y] and not dead_new) flood(y);
                dsu.add(u, v);
                dead[dsu.find(u)] = dead_new;
                graph[u].insert(v);
                graph[v].insert(u);
            }
        }

        for (i32 i = 1; i <= n; ++i) {
            std::cout << ans[i] << endl;
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_1265264999774934::solve();
    return 0;
}
