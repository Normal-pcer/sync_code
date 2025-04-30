/**
 * @link https://www.luogu.com.cn/problem/P3381
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_1965071402445387 {
    auto constexpr inf64 = std::numeric_limits<i64>::max();
    class Flow {
    public:
        struct EdgeNode {
            i32 to = 0;
            i32 next = 0;
            i64 flow = 0;
            i64 cost = 0;
        };
        struct Result {
            i64 flow;
            i64 cost;

            auto operator+= (Result const &other) -> Result & {
                return flow += other.flow, cost += other.cost, *this;
            }
            auto operator+ (Result const &other) const -> Result {
                auto tmp = *this;
                return tmp += other;
            }
        };
        i32 n;
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;
        
        Flow(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, i64 flow, i64 cost) -> void {
            edges.emplace_back(v, firstEdge[u], flow, cost);
            firstEdge[u] = static_cast<i32>(edges.size()) - 1;
            edges.emplace_back(u, firstEdge[v], 0, -cost);
            firstEdge[v] = static_cast<i32>(edges.size()) - 1;
        }

        auto dinic(i32 s, i32 t) -> Result {
            std::vector<i64> minCost;
            auto getMinCost = [&](i32 from) -> void {
                minCost = std::vector<i64>(n + 1, inf64);
                std::deque<i32> q;
                std::vector<std::uint8_t> vis(n + 1); 

                minCost[from] = 0;
                q.push_back(from);
                while (not q.empty()) {
                    auto u = q.front(); q.pop_front();
                    vis[u] = false;

                    for (auto i = firstEdge[u]; i != 0; i = edges[i].next) {
                        auto const &e = edges[i];
                        if (e.flow == 0) continue;
                        if (minCost[u] + e.cost < minCost[e.to]) {
                            minCost[e.to] = minCost[u] + e.cost;
                            if (not vis[e.to]) {
                                vis[e.to] = true;
                                q.push_back(e.to);
                            }
                        }
                    }
                }
            };

            auto tryFlow = [&](auto &&tryFlow, i32 u, i32 t, i64 flowLimit, std::vector<char> &vis) -> Result {
                assert(flowLimit != 0);
                if (u == t) return {flowLimit, 0};
                if (vis[u]) return {0, 0};
                vis[u] = true;

                Result total{};
                for (auto &i = firstEdge[u]; i != 0; i = edges[i].next) {
                    auto &e = edges[i];
                    auto &rev = edges[i ^ 1];

                    if (e.flow == 0) continue;
                    if (minCost[e.to] != minCost[u] + e.cost) continue;

                    Result current = tryFlow(tryFlow, e.to, t, std::min(flowLimit, e.flow), vis);
                    current.cost += current.flow * e.cost;
                    e.flow -= current.flow;
                    rev.flow += current.flow;
                    flowLimit -= current.flow;
                    total += current;

                    if (flowLimit == 0) break;
                }
                return total;
            };

            Result ans{};
            while (getMinCost(s), minCost[t] != inf64) {
                Result current{};
                std::vector<char> vis(n + 1);
                auto copy = firstEdge;
                while (current = tryFlow(tryFlow, s, t, inf64, vis), current.flow != 0) {
                    ans += current;
                }
                firstEdge = std::move(copy);
            }
            return ans;
        }
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

        auto ans = g.dinic(s, t);
        std::cout << ans.flow << " " << ans.cost << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_1965071402445387::solve();
    return 0;
}
