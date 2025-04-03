/**
 * @link https://www.luogu.com.cn/problem/P1772
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
#include "./libs/rolling_container.hpp"
using namespace lib;

/*
看上去会和最短路有关。

状压描述一条路径经过的点集。每个点开始跑一遍最短路，即可记录“经过某个点集的最短路径”。

F[i][j] 表示第 i 天，点集为 j 的最小代价

每次转移只需要：
F[i][j] <- F[i - 1][j] + cost[j]
        <- (max F[i - 1]) + K + cost[j]

然后好像就完事了？

*/
namespace Solution_2498288732531001 {
    auto solve() -> void {
        i32 daysCount, nodesCount, modifyCost, edgesCount;
        std::cin >> daysCount >> nodesCount >> modifyCost >> edgesCount;

        std::vector<std::tuple<i32, i32, i32>> edges(edgesCount);
        for (auto &[x, y, len]: edges) std::cin >> x >> y >> len, x--, y--;

        // 若干个港口的集合，二进制状态。
        // 港口为 0-index。
        // 发现不取 0 和 nodeCount - 1 的状态毫无意义。
        using Stat = u32;
        Stat maxStat = (Stat)1 << (nodesCount - 2);

        // 某一天有哪些港口关闭了；天数为 1-index。
        std::vector blockedOnDay(daysCount + 1, (Stat)0);
        i32 eventCount; std::cin >> eventCount;
        for (auto e = eventCount; e --> 0; ) {
            i32 port, a, b;
            std::cin >> port >> a >> b;
            port--;
            
            if (port == 0 or port == nodesCount - 1) continue;
            for (auto d = a; d <= b; d++) {
                blockedOnDay[d] |= (Stat)1 << (port - 1);
            }
        }

        debug {
            for (i32 i = 0; i <= daysCount; i++) {
                std::cout << std::format("blockedOnDay[{}] = ", i) << std::bitset<4>(blockedOnDay[i]) << std::endl;
            }
        }

        // 建图
        using PathLength = i64;
        struct GraphNode {
            i32 to;
            PathLength len;
        };
        std::vector<std::vector<GraphNode>> graph(nodesCount);
        for (auto [x, y, len]: edges) {
            graph[x].push_back({y, len});
            graph[y].push_back({x, len});
        }

        // 指定点集的最短路
        std::vector shortestPathOfPoints(maxStat, (PathLength)inf);
        auto walkOnGraph = [&](Stat subGraph) -> PathLength {
            struct Node {
                PathLength len;
                i32 p;

                auto operator<=> (Node const &) const = default;
            };
            std::priority_queue<Node, std::vector<Node>, std::greater<>> q;

            i32 constexpr maxNodesCount = 20;
            std::array<bool, maxNodesCount> vis{};
            q.push({.len = 0, .p = 0});
            while (not q.empty()) {
                auto x = q.top(); q.pop();
                if (x.p == nodesCount - 1) return x.len;
                if (vis[x.p]) continue;
                vis[x.p] = true;

                for (auto e: graph[x.p]) {
                    if (vis[e.to]) continue;
                    if (e.to != nodesCount - 1 and (((Stat)1 << (e.to - 1)) & subGraph) == 0) continue;
                    q.push({.len = x.len + e.len, .p = e.to});
                }
            }
            return inf;
        };

        for (Stat i = 0; i != maxStat; i++) {
            shortestPathOfPoints[i] = walkOnGraph(i);
        }

        using Cost = i64;
        // 第 i 天选取子图为 j 的最优解
        using BasicContainer = std::vector<std::vector<Cost>>;
        RollingContainer<BasicContainer, 2> F(std::vector<Cost>(maxStat, (Cost)inf));
        // minF[i] 为 F[i][j] 的最小值，任取 j
        std::vector minF(daysCount + 1, (Cost)inf);

        ranges::fill(F[0], 0);  // 第一天可以随便取，没有额外代价
        for (i32 day = 1; day <= daysCount; day++) {
            for (Stat sub = 0; sub != maxStat; sub++) {
                if (sub & blockedOnDay[day]) continue; // 走不通
                auto curCost = shortestPathOfPoints[sub];
                // 接着走上一次的图
                chkMin(F[day][sub], F[day - 1][sub] + curCost);
                // 从一个新图过来
                chkMin(F[day][sub], minF[day - 1] + curCost + modifyCost);
                chkMin(minF[day], F[day][sub]);
            }
        }

        debug {
            for (i32 i = 0; i <= daysCount; i++) {
                std::cout << std::format("minF[{}] = {}", i, minF[i]) << std::endl;
            }
        }

        auto ans = minF[daysCount];
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_2498288732531001::solve();
    return 0;
}
