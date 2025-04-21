#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "travel"

/*

*/
namespace Solution_6686914644768679 {
    auto solve() -> void {
        i32 n, m, t, s, e;
        std::cin >> n >> m >> t >> s >> e;

        std::vector<i32> costOf(n);
        for (auto &x: costOf) std::cin >> x;

        std::vector<i32> score(n);
        for (auto &x: score) std::cin >> x;

        struct GraphNode {
            i32 to, len;
        };
        std::vector<std::vector<GraphNode>> graph(n);

        for (auto _ = m; _ --> 0; ) {
            i32 x, y, len;
            std::cin >> x >> y >> len;

            graph[x].push_back({y, len});
            graph[y].push_back({x, len});
        }

        auto ans = [&]() -> i32 {
            i32 constexpr maxS = 101;
            i32 constexpr maxT = 301;
            std::vector<std::vector<std::vector<char>>> vis(maxS + 1, std::vector<std::vector<char>>(maxT, std::vector<char>(n, false)));
            
            struct QueueNode {
                i32 p, time, minScore, score;

                auto operator< (QueueNode const &other) const -> bool {
                    if (time != other.time) return time > other.time;
                    if (minScore != other.minScore) return minScore > other.minScore;
                    return score < other.score;
                }
            };
            std::priority_queue<QueueNode> q;

            q.push({s, 0, 0, 0});
            i32 ans = 0;
            while (not q.empty()) {
                auto x = q.top(); q.pop();

                std::cerr << x.p << " " << x.time << " " << x.minScore << " " << x.score << std::endl;

                if (x.time > t) continue;
                if (vis[x.minScore][x.time][x.p]) continue;
                if (x.p == t) chkMax(ans, x.score);
                vis[x.minScore][x.time][x.p] = true;

                std::cerr << x.p << " " << x.time << " " << x.minScore << " " << x.score << std::endl;

                // 游览景点
                if (x.minScore < score[x.p]) {
                    q.push({x.p, x.time + costOf[x.p], score[x.p], x.score + score[x.p]});
                }

                // 前往其他地方
                for (auto e: graph[x.p]) {
                    // 同时游览景点
                    // if (x.minScore < score[x.p]) {
                        // q.push({x.p, x.time + costOf[x.p] + e.len, score[x.p], x.score + score[x.p]});
                    // }
                    q.push({e.to, x.time + e.len, x.minScore, x.score});
                }
            }
            return ans;
        }();

        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    Solution_6686914644768679::solve();
    return 0;
}
