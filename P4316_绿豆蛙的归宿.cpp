/**
 * @link https://www.luogu.com.cn/problem/P4316
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
记 F[i] 为从 i 点到达终点的期望长度。
有向无环图，按照拓扑序转移即可。
*/
namespace Solution_2073946678825513 {
    using f64 = double;
    auto solveForce() -> void {
        i32 nodesCount, edgesCount;
        std::cin >> nodesCount >> edgesCount;

        struct GraphNode {
            i32 to = 0;
            i32 len = 0;
        };
        std::vector<std::vector<GraphNode>> graph(nodesCount + 1);
        for (auto _ = edgesCount; _ --> 0; ) {
            i32 x, y, len;
            std::cin >> x >> y >> len;
            graph[x].push_back({y, len});
        }

        std::mt19937 rng{std::random_device{}()};

        i32 constexpr iterCount = 1e6;
        i64 totalDis = 0;
        for (auto _ = iterCount; _ --> 0; ) {
            i32 cur = 1;
            auto target = nodesCount;

            while (cur != target) {
                auto childrenCount = static_cast<i32>(graph[cur].size());
                auto nextIndex = std::uniform_int_distribution<i32>{0, childrenCount - 1}(rng);
                auto edge = graph[cur][nextIndex];
                totalDis += edge.len;
                cur = edge.to;
            }
        }
        auto ans = static_cast<f64>(totalDis) / iterCount;
        std::cout << std::fixed << std::setprecision(2) << ans << endl;
    }
    auto solve() -> void {
        i32 pointsCount, edgesCount;
        std::cin >> pointsCount >> edgesCount;

        using Point = i32;
        using EdgeLen = i32;

        struct GraphNode {
            Point to = 0;
            EdgeLen len = 0;
        };
        std::vector<std::vector<GraphNode>> graph(pointsCount + 1);
        for (auto _ = edgesCount; _ --> 0; ) {
            Point x, y;
            EdgeLen len;
            std::cin >> x >> y >> len;
            graph[x].push_back({y, len});
        }

        std::vector<Point> order;
        {
            std::vector<i32> inDeg(pointsCount + 1);
            for (Point u = 1; u <= pointsCount; u++) {
                for (auto [v, len]: graph[u]) {
                    inDeg[v]++;
                }
            }
            std::deque<Point> q;  // 当前入度为 0 的点
            for (Point u = 1; u <= pointsCount; u++) {
                if (inDeg[u] == 0) {
                    q.push_back(u);
                }
            }
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                order.push_back(u);

                for (auto e: graph[u]) {
                    inDeg[e.to]--;
                    if (inDeg[e.to] == 0) {
                        q.push_back(e.to);
                    }
                }
            }
        }

        using Expect = f64;
        // F[i] 表示从 i 点走到终点的期望长度
        std::vector F(pointsCount + 1, (Expect)0);
        Point start = 1, target = pointsCount;
        F[target] = 0;

        for (auto u: order | views::reverse) {
            auto childrenCount = static_cast<i32>(graph[u].size());
            if (childrenCount == 0) continue;
            auto scale = static_cast<f64>(1) / childrenCount;

            F[u] = 0;
            for (auto e: graph[u]) {
                F[u] += F[e.to] + e.len;
            }
            F[u] *= scale;
        }

        auto ans = F[start];
        std::cout << std::fixed << std::setprecision(2) << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    Solution_2073946678825513::solve();
    return 0;
}
