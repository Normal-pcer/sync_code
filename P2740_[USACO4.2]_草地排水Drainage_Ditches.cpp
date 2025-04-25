/**
 * @link https://www.luogu.com.cn/problem/P2740
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_9021501901560324 {
    class Graph {
        struct EdgeNode {
            i32 to;
            i32 flow = 0;
            i32 nextEdge = 0;
        };

        i32 n;
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;

    public:
        Graph(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, i32 val) -> void {
            edges.push_back({.to = v, .flow = val, .nextEdge = firstEdge[u]});
            firstEdge[u] = static_cast<i32>(edges.size()) - 1;
            edges.push_back({.to = u, .flow = 0, .nextEdge = firstEdge[v]});
            firstEdge[v] = static_cast<i32>(edges.size()) - 1;
        }

        auto getFlow(i32 s, i32 t) -> i32 {
            std::vector<i32> level(n + 1, -1);

            // 层次图，返回从 from 是否可能到达 to
            auto getSliced = [&](i32 from, i32 to) -> bool {
                std::fill(level.begin(), level.end(), -1);
                std::deque<i32> q;
                q.push_back(from);

                level[from] = 1;
                while (not q.empty()) {
                    auto u = q.front(); q.pop_front();
                    for (auto i = firstEdge[u]; i != 0; i = edges[i].nextEdge) {
                        auto e = edges[i];
                        if (e.flow == 0) continue;
                        if (level[e.to] != -1) continue;
                        level[e.to] = level[u] + 1;
                        q.push_back(e.to);
                    }
                }
                return level[to] != -1;
            };

            i32 maxFlow = 0;
            // 如果起点和终点仍然可以互相走到
            while (getSliced(s, t)) {
                // 尝试向下继续流
                // 返回可以消耗多少流量
                auto tryFlow = [&](auto &&tryFlow, i32 u, i32 t, i32 flowLimit) -> i32 {
                    if (u == t) return flowLimit;
                    i32 totalFlow = 0;  // 一共消耗了多少流量
                    for (auto &i = firstEdge[u]; i != 0; i = edges[i].nextEdge) {
                        auto &e = edges[i];
                        if (level[u] + 1 == level[e.to] and e.flow != 0) {
                            // 尝试流向这个孩子
                            auto current = tryFlow(tryFlow, e.to, t, 
                                std::min(e.flow, flowLimit));
                            
                            totalFlow += current;
                            e.flow -= current;  // 消耗流量
                            edges[i ^ 1].flow += current;  // 允许回流
                            flowLimit -= current;

                            if (current == 0) {  // 向下无法走通
                                level[e.to] = -1;
                            }
                            if (flowLimit == 0) {  // 不可能继续走
                                break;
                            }
                        }
                    }
                    // debug std::cerr << std::format("try({}, {}, {}) -> {}", u, t, flowLimit, totalFlow) << std::endl;
                    return totalFlow;
                };

                auto copy = firstEdge;
                while (auto flow = tryFlow(tryFlow, s, t, std::numeric_limits<i32>::max())) {
                    maxFlow += flow;
                }

                firstEdge = copy;

            }

            return maxFlow;
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> m >> n;

        Graph g{n};
        for (i32 _ = m; _ --> 0; ) {
            i32 x, y, val;
            std::cin >> x >> y >> val;
            g.addEdge(x, y, val);
        }

        auto ans = g.getFlow(1, n);
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_9021501901560324::solve();
    return 0;
}
