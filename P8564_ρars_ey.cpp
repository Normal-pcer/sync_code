/**
 * @link https://www.luogu.com.cn/problem/P8564
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_1358251599126775 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> cost(N+1);
        for (auto &i: cost | views::drop(1) | views::take(N-1))  std::cin >> i;
        std::vector<std::vector<int>> graph(N+1);
        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        std::vector<int> size(N+1);
        std::vector F(N+1, std::vector(N+1, infLL));
        std::function<void(int, int)> dfs = [&](int p, int prev) {
            size[p] = 1, F[p][0] = 0;
            for (auto dest: graph.at(p))  if (dest != prev) {
                dfs(dest, p);
                for (auto i: range(1, size[p] + size[dest]) | views::reverse) {
                    for (auto j: range(std::max(i - size[p], 0), std::min(i, size[dest]) + 1)) {
                        chkMin(F[p][i], F[p][i-j] + F[dest][j]);
                        debug  printValues(p, dest, i, j, F[p][i], F[p][i-j], F[dest][j], F[p][i-j] + F[dest][j]);
                    }
                }
                size[p] += size[dest];
            }
            for (auto x: range(0, size[p]-1)) {
                chkMin(F[p][size[p]-1], F[p][x] + cost[size[p] - 1 - x]);
                debug  printValues(p, x, F[p][size[p]-1], F[p][x], cost[size[p] - 1 - x], F[p][x] + cost[size[p] - 1 - x]);
            }
        };
        dfs(1, 0);
        debug  for (auto i: range(0, N+1)) {
            for (auto j: range(0, N+1)) {
                printValues(i, j, F[i][j]);
            }
        }
        auto ans = F[1][size[1] - 1];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1358251599126775::solve();
    return 0;
}
