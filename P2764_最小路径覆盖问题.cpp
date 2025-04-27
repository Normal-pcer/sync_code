/**
 * @link https://www.luogu.com.cn/problem/P2764
 * @link https://neooj.com:8082/oldoj/problem.php?id=2490
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
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
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
using namespace lib;

/*
新的图包含这些部分：
1. 起点和终点
2. “入点”
3. “出点”

原图中的连边，只需在新图中把对应的出点和入点相连，边权为 1。起点到所有出点、所有入点到终点也连边权为 1 的边。
这个图上的最大流，可以保证一个点最多一次入、一次出，一定是符合题意的。
出边到入边流量为 0 的，表示路径包括这个点。
*/
namespace Solution_7951965096626351 {
    class Graph {
        struct EdgeNode {
            i32 to;
            i32 flow = 0;
            i32 next = 0;

            EdgeNode(i32 to = 0, i32 flow = 0, i32 next = 0): to(to), flow(flow), next(next) {}
        };
        i32 n;
    public:
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;

        Graph(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, i32 val) -> void {
            edges.emplace_back(v, val, firstEdge[u]);
            firstEdge[u] = static_cast<i32>(edges.size() - 1);
            edges.emplace_back(u, 0, firstEdge[v]);
            firstEdge[v] = static_cast<i32>(edges.size() - 1);
        }
        
        auto getFlow(i32 s, i32 t) -> i32 {
            std::vector<i32> level;
            auto getSliced = [&](i32 from, i32 to) -> bool {
                level = std::vector<i32>(n + 1, -1);
                std::deque<i32> q;
                q.push_back(from);
                level[from] = 1;
                while (not q.empty()) {
                    auto u = q.front(); q.pop_front();
                    for (auto i = firstEdge[u]; i != 0; i = edges[i].next) {
                        auto const &e = edges[i];
                        if (level[e.to] != -1) continue;
                        if (e.flow == 0) continue;
                        q.push_back(e.to);
                        level[e.to] = level[u] + 1;
                    }
                }
                return level[to] != -1;
            };
            auto tryFlow = [&](auto &&tryFlow, i32 u, i32 t, i32 flowLimit) -> i32 {
                if (u == t) return flowLimit;
                i32 total = 0;
                for (auto i = firstEdge[u]; i != 0; i = edges[i].next) {
                    auto &e = edges[i];
                    if (level[e.to] != level[u] + 1) continue;
                    if (e.flow == 0) continue;
                    auto current = tryFlow(tryFlow, e.to, t, std::min(e.flow, flowLimit));

                    e.flow -= current;
                    edges[i ^ 1].flow += current;
                    flowLimit -= current;
                    total += current;

                    if (current == 0) level[e.to] = -1;
                    if (flowLimit == 0) break;
                }
                return total;
            };

            i32 ans = 0;
            while (getSliced(s, t)) {
                while (auto flow = tryFlow(tryFlow, s, t, inf)) {
                    ans += flow;
                }
            }
            return ans;
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<std::pair<i32, i32>> edges(m);
        for (auto &[u, v]: edges) std::cin >> u >> v;

        auto getOut = lam(x, x);  // 出点
        auto getIn = lam(x, x + n);  // 入点
        i32 const start = n + n + 1, target = n + n + 2;

        Graph g{n + n + 2};
        for (i32 i = 1; i <= n; i++) g.addEdge(start, getOut(i), 1);
        for (i32 i = 1; i <= n; i++) g.addEdge(getIn(i), target, 1);
        for (auto const &[u, v]: edges) {
            g.addEdge(getOut(u), getIn(v), 1);
        }

        {
            auto ans = n - g.getFlow(start, target);
            std::vector<std::pair<i32, i32>> newEdges;
            newEdges.reserve(n);
            for (i32 u = 1; u <= n; u++) {
                for (i32 i = g.firstEdge[u]; i != 0; i = g.edges[i].next) {
                    auto const &e = g.edges[i];
                    if (e.flow != 0) continue;
                    if (e.to < getIn(1)) continue;
                    if (e.to > getIn(n)) continue;
                    newEdges.emplace_back(u, e.to - n);
                }
            }
            std::vector<i32> next(n + 1, 0);
            std::vector<i32> prev(n + 1, 0);
            for (auto const &[u, v]: newEdges) {
                assert(next[u] == 0 and prev[v] == 0);
                next[u] = v, prev[v] = u;
            }

            // 是链首，顺着向下走
            for (i32 u = 1; u <= n; u++) {
                if (prev[u] == 0) {
                    std::cout << u;
                    for (auto p = next[u]; p != 0; p = next[p]) {
                        std::cout << " " << p;
                    }
                    std::cout << endl;
                }
            }
            std::cout << ans << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_7951965096626351::solve();
    return 0;
}
