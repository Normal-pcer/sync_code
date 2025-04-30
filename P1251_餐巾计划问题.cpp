/**
 * @link https://www.luogu.com.cn/problem/P1251
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_8338596025146685 {
    bool constexpr isLuogu = __cplusplus >= 202000L;
    i64 constexpr inf = 0x3f3f3f3f3f3f3f3fLL;
    class Flow {
    public:
        struct EdgeNode {
            i32 to = 0, next = 0;
            i64 flow = 0, cost = 0;

            EdgeNode() = default;
            EdgeNode(i32 to, i32 next, i64 flow, i64 cost): to(to), next(next), flow(flow), cost(cost) {}
        };
        struct Result {
            i64 flow, cost;

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

        auto minCostMaxFlow(i32 s, i32 t) -> Result {
            std::vector<i64> minCost;

            auto getMinCost = [&](i32 from) -> void {
                minCost = std::vector<i64>(n + 1, inf);
                std::vector<std::uint8_t> inQueue(n + 1, false);
                std::deque<i32> q;
                q.emplace_back(from), inQueue[from] = true;
                minCost[from] = 0;

                while (not q.empty()) {
                    auto u = q.front(); q.pop_front();
                    inQueue[u] = false;

                    for (auto i = firstEdge[u]; i != 0; i = edges[i].next) {
                        auto const &e = edges[i];
                        if (e.flow == 0) continue;

                        if (minCost[e.to] > minCost[u] + e.cost) {
                            minCost[e.to] = minCost[u] + e.cost;
                            if (not inQueue[e.to]) q.emplace_back(e.to), inQueue[e.to] = true;
                        }
                    }
                }
            };

            auto tryFlow = [&](auto &&tryFlow, i32 u, i32 t, i64 flowLimit, std::vector<std::uint8_t> &vis) -> Result {
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
                    current.cost += e.cost * current.flow;
                    e.flow -= current.flow;
                    rev.flow += current.flow;
                    flowLimit -= current.flow;
                    total += current;

                    if (flowLimit == 0) break;
                    // if (current.flow == 0) minCost[e.to] = inf;
                }

                return total;
            };

            auto copy = firstEdge;
            Result ans{};
            while (getMinCost(s), minCost[t] != inf) {
                Result current;
                std::vector<std::uint8_t> vis(n + 1, false);
                while (current = tryFlow(tryFlow, s, t, inf, vis), current.flow != 0) {
                    ans += current;
                }
                firstEdge = copy;
            }
            return ans;
        }
    };
    auto solve() -> void {
        i32 days, newCost, fastDays, fastCost, slowDays, slowCost;

        std::cin >> days;
        std::vector<i32> needs(days);
        if constexpr (isLuogu) for (auto &x: needs) std::cin >> x;
        std::cin >> newCost >> fastDays >> fastCost >> slowDays >> slowCost;
        if constexpr (not isLuogu) for (auto &x: needs) std::cin >> x;

        auto dayBeginToNode = lam(day, day);
        auto dayEndToNode = lam(day, day + days);
        auto const start = days * 2, target = days * 2 + 1;

        auto const nodeCount = days * 2 + 2;
        Flow g{nodeCount};
        for (i32 day = 0; day != days; ++day) {
            if (day != 0) {
                g.addEdge(dayBeginToNode(day - 1), dayBeginToNode(day), inf, 0);
            } else {
                g.addEdge(start, dayBeginToNode(day), inf, newCost);
            }
            g.addEdge(start, dayEndToNode(day), needs[day], 0);
            g.addEdge(dayBeginToNode(day), target, needs[day], 0);

            if (day + slowDays < days) {
                g.addEdge(dayEndToNode(day), dayBeginToNode(day + slowDays), inf, slowCost);
            }
            if (day + fastDays < days) {
                g.addEdge(dayEndToNode(day), dayBeginToNode(day + fastDays), inf, fastCost);
            }
        }

        auto ans = g.minCostMaxFlow(start, target);
        assert(ans.flow == std::accumulate(needs.begin(), needs.end(), (i64)0));
        std::cout << ans.cost << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_8338596025146685::solve();
    return 0;
}
