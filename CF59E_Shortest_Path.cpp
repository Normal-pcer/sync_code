/**
 * @link https://www.luogu.com.cn/problem/CF59E
 * @link https://codeforces.com/problemset/problem/59/E
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_1466782607436276 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        std::vector<std::vector<int>> graph(N+1);
        for (auto _: range(M)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }
        std::vector<std::tuple<int, int, int>> curse(K);
        std::set<std::tuple<int, int, int>> set;
        for (auto &[x, y, z]: curse)  std::cin >> x >> y >> z, set.insert({x, y, z});

        std::vector ans_prev(N+1, std::map<int, int>{});
        auto bfs = [&](int outset, int destination) {
            struct Point {
                int p, prev;
                int val = 0;
            };
            std::deque<Point> q;
            std::vector vis(N+1, std::vector<char>(N+1));
            q.push_back({outset, 0});
            while (not q.empty()) {
                auto x = q.front();  q.pop_front();
                debug  printValues(x.p, x.prev, x.val);
                if (vis[x.p][x.prev])  continue;
                ans_prev[x.p][x.val] = x.prev;
                if (x.p == destination)  return x.val;
                vis[x.p][x.prev] = true;
                for (auto next: graph.at(x.p)) {
                    if (vis[next][x.p])  continue;
                    if (set.contains({x.prev, x.p, next}))  continue;
                    q.push_back({.p = next, .prev = x.p, .val = x.val + 1});
                }
            }
            return -1;
        };

        auto ans = bfs(1, N);
        std::cout << ans << endl;
        debug {
            for (auto &line: ans_prev) {
                for (auto [i, j]: line) {
                    std::cout << std::format("{{{}: {}}}", i, j) << ' ';
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        if (ans != -1) {
            std::vector<int> steps {N};
            while (steps.back() != 0) {
                steps.push_back(ans_prev[steps.back()][ans]), ans--;
            }
            steps.pop_back(), ranges::reverse(steps);
            for (auto i: steps)  std::cout << i << ' ';
            std::cout << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1466782607436276::solve();
    return 0;
}
