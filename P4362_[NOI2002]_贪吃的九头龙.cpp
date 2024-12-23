/**
 * @link https://www.luogu.com.cn/problem/P4362
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/debug_log.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_2128936322064865 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;
        std::cin >> N >> M >> K;
        if(N - K < M - 1) {
            std::cout << "-1" << std::endl;
            return;
        }
        struct Node { int to, val; };
        std::vector<std::vector<Node>> graph(N+1);
        for (auto _: range(N-1)) {
            int a, b, c;  std::cin >> a >> b >> c;
            graph.at(a).push_back({b, c});
            graph.at(b).push_back({a, c});
        }

        std::vector F(N+1, std::vector(N+1, std::vector(2, inf)));
        std::vector<int> size(N+1);
        std::function<void(int, int)> dfs = [&](int p, int prev) {
            F[p][1][1] = F[p][0][0] = 0;
            for (auto dest: graph.at(p))  if (dest.to != prev) {
                dfs(dest.to, p);
                size[p] += size[dest.to];
                std::vector tmp(N+1, std::vector(2, inf));
                for (auto i: range(K + 1)) {
                    for (auto j: range(i + 1)) {
                        // if (j > size[dest.to])  break;
                        if (M == 2) {
                            debug  printValues(p, i, j, dest.to, dest.val, tmp[i][0], F[p][i-j][0] + F[dest.to][j][0] + dest.val);
                            chkMin(tmp[i][0], F[p][i-j][0] + F[dest.to][j][0] + dest.val);
                        } else {
                            debug  printValues(p, i, j, dest.to, dest.val, tmp[i][0], F[p][i-j][0] + F[dest.to][j][0]);
                            chkMin(tmp[i][0], F[p][i-j][0] + F[dest.to][j][0]);
                        }
                        debug  printValues(p, i, j, dest.to, dest.val, tmp[i][1], F[p][i-j][1] + F[dest.to][j][0]);
                        chkMin(tmp[i][1], F[p][i-j][1] + F[dest.to][j][0]);
                        debug  printValues(p, i, j, dest.to, dest.val, tmp[i][0], F[p][i-j][0] + F[dest.to][j][1]);
                        chkMin(tmp[i][0], F[p][i-j][0] + F[dest.to][j][1]);
                        debug  printValues(p, i, j, dest.to, dest.val, tmp[i][1], F[p][i-j][1] + F[dest.to][j][1] + dest.val);
                        chkMin(tmp[i][1], F[p][i-j][1] + F[dest.to][j][1] + dest.val);
                    }
                }
                ranges::copy(tmp, F[p].begin());
            }
            chkMax(size[p], 1);
        };

        dfs(1, 0);
        debug  for (auto i: range(0, N+1)) {
            for (auto j: range(0, N+1)) {
                for (auto k: {0, 1}) {
                    printValues(i, j, k, F[i][j][k]);
                }
            }
        }

        auto ans = F[1][K][1];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2128936322064865::solve();
    return 0;
}
