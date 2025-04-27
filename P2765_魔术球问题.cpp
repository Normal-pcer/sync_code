/**
 * @link https://www.luogu.com.cn/problem/P2765
 * @link https://neooj.com:8082/oldoj/problem.php?id=2491
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
using namespace lib;

namespace Solution_8770241179195370 {
    i32 constexpr inf = 0x3f3f3f3f;
    class Graph {
    public:
        struct EdgeNode {
            i32 to;
            i32 flow = 0;
            i32 next = 0;

            explicit EdgeNode(i32 to = 0, i32 flow = 0, i32 next = 0): to(to), flow(flow), next(next) {}
        };

        i32 n;
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;

        explicit Graph(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, i32 val) -> void {
            // std::cerr << "addEdge " << u << " " << v << " " << val << std::endl;
            edges.emplace_back(v, val, firstEdge[u]);
            firstEdge[u] = static_cast<i32>(edges.size()) - 1;
            edges.emplace_back(u, 0, firstEdge[v]);
            firstEdge[v] = static_cast<i32>(edges.size()) - 1;
        }

        auto expand() -> void {
            n++;
            firstEdge.push_back(0);
        }

        auto getFlow(i32 s, i32 t) -> i32 {
            std::vector<i32> level;

            auto getLevel = [&](i32 from, i32 to) -> bool {
                level = std::vector<i32>(n + 1, -1);
                std::deque<i32> q;
                q.push_back(from);
                level[from] = 1;
                while (not q.empty()) {
                    auto u = q.front(); q.pop_front();
                    for (auto i = firstEdge[u]; i != 0; i = edges[i].next) {
                        auto const &e = edges[i];
                        if (e.flow == 0) continue;
                        if (level[e.to] != -1) continue;
                        level[e.to] = level[u] + 1;
                        q.push_back(e.to);
                    }
                }
                return level[to] != -1;
            };
            auto tryFlow = [&](auto &&tryFlow, i32 u, i32 t, i32 flowLimit) -> i32 {
                if (u == t) return flowLimit;
                i32 total = 0;
                for (auto &i = firstEdge[u]; i != 0; i = edges[i].next) {
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
            while (getLevel(s, t)) {
                auto copy = firstEdge;
                while (auto flow = tryFlow(tryFlow, s, t, inf)) {
                    ans += flow;
                }
                firstEdge = std::move(copy);
            }
            return ans;
        }
    };

    auto solve() -> void {
        i32 n;
        std::cin >> n;

        i32 constexpr start = 1;
        i32 constexpr target = 2;
        auto getOut = lam(x, x * 2 + 1);
        auto getIn = lam(x, x * 2 + 2);

        Graph g{2};
        std::vector<std::vector<i32>> prevAns;
        i32 flow = 0;
        for (i32 i = 1; ; i++) {
            // std::cerr << "i = " << i << std::endl;
            g.expand(), g.expand();

            g.addEdge(start, getOut(i), 1);
            g.addEdge(getIn(i), target, 1);

            for (auto j = static_cast<i32>(std::sqrt(i * 2)); ; j--) {
                auto other = j * j - i;
                if (other >= i) continue;
                if (other <= 0) break;
                g.addEdge(getOut(i), getIn(other), 1);
            }

            if (auto pillar = i - (flow += g.getFlow(start, target)); pillar > n) {
                // std::cerr << "pillar = " << pillar << std::endl;
                std::cout << i - 1 << endl;
                for (auto const &line: prevAns) {
                    auto first = true;
                    for (auto const &x: line) {
                        if (not first) std::cout << ' ';
                        std::cout << x;
                        first = false;
                    }
                    std::cout << endl;
                }
                break;
            } else {
                std::vector<std::vector<i32>> ans;
                std::vector<i32> prev(i + 1);
                std::vector<i32> next(i + 1);

                std::vector<std::pair<i32, i32>> newEdges;
                for (i32 u = 1; u <= i; u++) {
                    for (i32 j = g.firstEdge[getOut(u)]; j != 0; j = g.edges[j].next) {
                        auto const &e = g.edges[j];
                        if (e.flow != 0) continue;
                        if (e.to == start) continue;
                        newEdges.emplace_back(u, (e.to - 2) / 2);
                    }
                }

                for (auto const &[u, v]: newEdges) {
                    assert(next[u] == 0 and prev[v] == 0);
                    next[u] = v, prev[v] = u;
                }

                for (i32 u = 1; u <= i; u++) {
                    if (prev[u] == 0) {
                        ans.emplace_back();
                        for (auto p = u; p != 0; p = next[p]) {
                            ans.back().emplace_back(p);
                        }
                        std::reverse(ans.back().begin(), ans.back().end());
                    }
                }
                prevAns = ans;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_8770241179195370::solve();
    return 0;
}
