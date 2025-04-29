/**
 * @link https://www.luogu.com.cn/problem/P2766
 * @link https://neooj.com:8082/oldoj/problem.php?id=2493
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_7304984831769614 {
    auto constexpr inf = std::numeric_limits<i32>::max();
    class Graph {
    public:
        struct EdgeNode {
            i32 to = 0;
            i32 flow = 0;
            i32 next = 0;

            EdgeNode() = default;
            explicit EdgeNode(i32 to, i32 flow, i32 next): to(to), flow(flow), next(next) {}
        };
        i32 n;
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;

        explicit Graph(i32 n): n(n), edges(2), firstEdge(n + 1) {}

        auto addEdge(i32 u, i32 v, i32 val) -> void {
            debug std::cerr << "Add Edge: " << u << " " << v << " " << val << std::endl;
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
                    auto &rev = edges[i xor 1];

                    if (e.flow == 0) continue;
                    if (level[e.to] != level[u] + 1) continue;
                    
                    auto current = tryFlow(tryFlow, e.to, t, std::min(e.flow, flowLimit));
                    e.flow -= current;
                    rev.flow += current;
                    flowLimit -= current;
                    total += current;

                    if (flowLimit == 0) break;
                    if (current == 0) level[e.to] = -1;
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

        std::vector<i32> a(n);
        for (auto &x: a) std::cin >> x;

        std::vector<i32> f(n, 1);  // f[i] 表示以 i 结尾的 LIS
        for (i32 i = 1; i < n; i++) {
            for (i32 j = 0; j < i; j++) {
                if (a[j] <= a[i]) {
                    chkMax(f[i], f[j] + 1);
                }
            }
        }

        auto indexToNode = lam(x, x + 1);
        auto const start = n + 1, target = n + 2;

        // 两个图中一定要连的边
        std::vector<std::pair<i32, i32>> leastEdges;
        for (i32 i = 0; i < n; i++) {
            for (i32 j = i + 1; j < n; j++) {
                if (a[i] <= a[j] and f[j] == f[i] + 1) {
                    leastEdges.emplace_back(indexToNode(i), indexToNode(j));
                }
            }
        }

        auto maxF = *std::max_element(f.begin(), f.end());
        for (i32 i = 1; i <= n - 2; i++) {
            if (f[i] == 1) leastEdges.emplace_back(start, indexToNode(i));
            if (f[i] == maxF) leastEdges.emplace_back(indexToNode(i), target);
        }
        
        auto connectWithStartTarget = [&](Graph &g, i32 i, i32 val) -> void {
            if (f[i] == 1) g.addEdge(start, indexToNode(i), val);
            if (f[i] == maxF) g.addEdge(indexToNode(i), target, val);
        };
        auto doTask = [&](i32 countFirst, i32 countLast) -> void {
            Graph g{n + 2};
            for (auto const &[u, v]: leastEdges) g.addEdge(u, v, 1);

            connectWithStartTarget(g, 0, countFirst);
            connectWithStartTarget(g, n - 1, countLast);
            std::cout << g.getFlow(start, target) << endl;
        };

        std::cout << maxF << endl;
        doTask(1, 1);
        doTask(
            static_cast<i32>(std::count_if(leastEdges.begin(), leastEdges.end(), lam(e, e.first == indexToNode(0)))),
            static_cast<i32>(std::count_if(leastEdges.begin(), leastEdges.end(), lam(e, e.second == indexToNode(n - 1))))
        );
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_7304984831769614::solve();
    return 0;
}
