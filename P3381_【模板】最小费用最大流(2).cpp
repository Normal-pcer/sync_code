/**
 * @link https://www.luogu.com.cn/problem/P3381
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_3352154504570398 {
    using Value = i64;
    class Flow {
    public:
        struct EdgeNode {
            i32 to = 0, next = 0;
            Value flow{}, cost{};
        };
        struct Result {
            Value flow{};
            Value cost{};
        };
        i32 n;
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;

        auto static constexpr maxValue = std::numeric_limits<Value>::max();

        Flow(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, Value flow, Value cost) -> void {
            edges.emplace_back(v, firstEdge[u], flow, cost);
            firstEdge[u] = static_cast<i32>(edges.size()) - 1;
            edges.emplace_back(u, firstEdge[v], 0, -cost);
            firstEdge[v] = static_cast<i32>(edges.size()) - 1;
        }

        auto minCostMaxFlow(i32 s, i32 t) -> Result {
            std::vector<Value> minCost;
            std::vector<i32> prev;  // 最小费用路径上，从前一个节点的连边
            auto getMinCost = [&](i32 from) -> void {
                minCost = std::vector<Value>(n + 1, maxValue);
                prev = std::vector<i32>(n + 1, 0);
                std::vector<char> inQueue(n + 1);
                std::deque<i32> q;
                q.push_back(from), inQueue[from] = true;
                minCost[from] = 0;

                while (not q.empty()) {
                    auto u = q.front(); q.pop_front();
                    inQueue[u] = false;
                    for (auto i = firstEdge[u]; i != 0; i = edges[i].next) {
                        auto const &e = edges[i];
                        if (e.flow == 0) continue;
                        if (minCost[e.to] > minCost[u] + e.cost) {
                            minCost[e.to] = minCost[u] + e.cost;
                            prev[e.to] = i;
                            if (not inQueue[e.to]) q.push_back(e.to), inQueue[e.to] = true;;
                        }
                    }
                }
            };

            Result ans{};
            while (getMinCost(s), minCost[t] != maxValue) {
                auto flow = maxValue;
                for (i32 u = t; u != s; u = edges[prev[u] ^ 1].to) {
                    chkMin(flow, edges[prev[u]].flow);
                }

                for (i32 u = t; u != s; u = edges[prev[u] ^ 1].to) {
                    edges[prev[u]].flow -= flow;
                    edges[prev[u] ^ 1].flow += flow;
                }
                ans.cost += flow * minCost[t];
                ans.flow += flow;
            }
            return ans;
        };
    };
    auto solve() -> void {
        i32 n, m, s, t;
        std::cin >> n >> m >> s >> t;

        Flow g{n};
        for (auto _ = m; _ --> 0; ) {
            i32 u, v, flow, cost;
            std::cin >> u >> v >> flow >> cost;
            g.addEdge(u, v, flow, cost);
        }

        auto ans = g.minCostMaxFlow(s, t);
        std::cout << ans.flow << " " << ans.cost << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_3352154504570398::solve();
    return 0;
}
