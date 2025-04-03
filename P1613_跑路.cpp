/**
 * @link https://www.luogu.com.cn/problem/P1613
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
*/
namespace Solution_9741920748536271 {
    auto solve() -> void {
        using Point = i32;

        i32 pointsCount, edgeCount;
        std::cin >> pointsCount >> edgeCount;
        std::vector<std::pair<Point, Point>> edges(edgeCount);
        std::vector<std::vector<Point>> graph(pointsCount + 1);
        for (auto &[x, y]: edges) {
            std::cin >> x >> y;
            graph[x].push_back(y);
        }

        using LogDistance = i32;
        LogDistance constexpr maxLogDistance = 32;
        // reach[d][i][j] 表示从点 i 到 j 是否有一条长度为 2**d 的路径
        std::vector reach(maxLogDistance, 
            std::vector(pointsCount + 1, std::vector<char>(pointsCount + 1, false))
        );

        for (auto [x, y]: edges) {
            reach[0][x][y] = true;
        }
        for (LogDistance ld = 1; ld != maxLogDistance; ld++) {
            for (Point x = 1; x <= pointsCount; x++) {
                for (Point y = 1; y <= pointsCount; y++) {
                    for (Point mid = 1; mid <= pointsCount; mid++) {
                        reach[ld][x][y] |= reach[ld - 1][x][mid] and reach[ld - 1][mid][y];
                    }
                }
            }
        }

        for (LogDistance ld = 1; ld != maxLogDistance; ld++) {
            for (Point x = 1; x <= pointsCount; x++) {
                for (Point y = 1; y <= pointsCount; y++) {
                    if (reach[ld][x][y]) {
                        graph[x].push_back(y);
                    }
                }
            }
        }

        for (auto &line: graph) {
            ranges::sort(line);
            auto tmp = ranges::unique(line);
            line.erase(tmp.begin(), tmp.end());
        }

        auto ans = [&](i32 start, i32 target) -> i32 {
            struct Node {
                Point p;
                i32 dis;
            };
            std::deque<Node> q;
            q.push_back({start, 0});

            std::vector<char> vis(pointsCount + 1, false);
            while (not q.empty()) {
                auto cur = q.front(); q.pop_front();
                if (vis[cur.p]) continue;
                if (cur.p == target) return cur.dis;
                vis[cur.p] = true;
                for (auto next: graph[cur.p]) {
                    if (not vis[next]) {
                        q.push_back({next, cur.dis + 1});
                    }
                }
            }
            assert(false);
            return -1;
        }(1, pointsCount);
        std::cout << ans << endl;
    }
}

auto main(int dyy_forever, char const *lmy_forever[]) -> int {
    DEBUG_MODE = (dyy_forever != 1) and (std::strcmp("-d", lmy_forever[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_9741920748536271::solve();
    return 0;
}
