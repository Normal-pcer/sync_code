/**
 * @link https://www.luogu.com.cn/problem/P3254
 * @link https://neooj.com:8082/oldoj/problem.php?id=2492
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_4668616589250339 {
    auto constexpr inf = std::numeric_limits<i32>::max();
    class Graph {
    public:
        struct EdgeNode {
            i32 to = 0;
            i32 flow = 0;
            i32 next = 0;

            EdgeNode(i32 to = 0, i32 flow = 0, i32 next = 0): to(to), flow(flow), next(next) {}
        };
        i32 n;
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;

        Graph(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, i32 val) -> void {
            edges.emplace_back(v, val, firstEdge[u]);
            firstEdge[u] = static_cast<i32>(edges.size()) - 1;
            edges.emplace_back(u, 0, firstEdge[v]);
            firstEdge[v] = static_cast<i32>(edges.size()) - 1;
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
                        q.push_back(e.to);
                        level[e.to] = level[u] + 1;
                    }
                }
                return level[to] != -1;
            };

            auto tryFlow = [&](auto &&tryFlow, i32 u, i32 t, i32 flowLimit) -> i32 {
                if (u == t) {
                    return flowLimit;
                }
                
                i32 totalFlow = 0;
                for (auto &i = firstEdge[u]; i != 0; i = edges[i].next) {
                    auto &e = edges[i];
                    auto &rev = edges[i ^ 1];

                    if (level[e.to] != level[u] + 1) continue;
                    if (e.flow == 0) continue;

                    auto current = tryFlow(tryFlow, e.to, t, std::min(e.flow, flowLimit));
                    e.flow -= current;
                    rev.flow += current;
                    flowLimit -= current;
                    totalFlow += current;

                    if (current == 0) level[e.to] = -1;
                    if (flowLimit == 0) break;
                }
                return totalFlow;
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
        i32 companies, tables;
        std::cin >> companies >> tables;

        std::vector<i32> personsOf(companies);
        std::vector<i32> sizeOf(tables);

        for (auto &x: personsOf) std::cin >> x;
        for (auto &x: sizeOf) std::cin >> x;

        auto nodeOfCompany = lam(x, x + 1);
        auto nodeOfTable = lam(x, x + companies + 1);
        auto const start = companies + tables + 1;
        auto const target = companies + tables + 2;

        Graph g{companies + tables + 2};
        for (i32 c = 0; c < companies; c++) {
            g.addEdge(start, nodeOfCompany(c), personsOf[c]);
        }
        for (i32 t = 0; t < tables; t++) {
            g.addEdge(nodeOfTable(t), target, sizeOf[t]);
        }
        for (i32 c = 0; c < companies; c++) {
            for (i32 t = 0; t < tables; t++) {
                g.addEdge(nodeOfCompany(c), nodeOfTable(t), 1);
            }
        }

        {
            auto flow = g.getFlow(start, target);
            auto const &firstEdge = g.firstEdge;
            auto const &edges = g.edges;

            auto expected = std::accumulate(personsOf.begin(), personsOf.end(), (i32)0);
            if (flow != expected) {
                std::cout << 0 << endl;
                return;
            }
            std::cout << 1 << endl;

            
            std::vector<std::vector<i32>> ans(companies);
            // 统计答案
            // 如果一个公司到一个桌子的连边被切断即可
            for (i32 c = 0; c < companies; c++) {
                for (auto i = firstEdge[nodeOfCompany(c)]; i != 0; i = edges[i].next) {
                    auto const &e = edges[i];
                    if (e.flow != 0) continue;
                    if (e.to == start) continue;
                    auto t = e.to - companies - 1;
                    ans[c].emplace_back(t + 1);
                }
            }

            for (auto const &line: ans) {
                for (auto first = true; auto const &item: line) {
                    if (not first) std::cout << " ";
                    std::cout << item, first = false;
                }
                std::cout << endl;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4668616589250339::solve();
    return 0;
}