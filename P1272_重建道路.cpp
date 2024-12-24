/**
 * @link https://www.luogu.com.cn/problem/P1272
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_6491926694052276 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        
        int N, P;  std::cin >> N >> P;
        std::vector<std::vector<int>> graph(N+1);
        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        std::vector<int> size(N+1);
        std::vector F(N+1, std::vector(P+1, inf));
        std::function<void(int, int)> dfs = [&](int p, int prev) {
            size[p] = 1, F[p][1] = 1;
            for (auto dest: graph.at(p))  if (dest != prev) {
                dfs(dest, p);
                std::vector Fp_out(F[p]);
                for (auto i: range(1, std::min(size[p] + size[dest], P) + 1)) {
                    for (auto j: range(1, std::min(i, size[dest]) + 1)) {
                        chkMin(Fp_out[i], F[p][i-j] + F[dest][j] + 1);
                    }
                }
                ranges::copy(Fp_out, F[p].begin());
                size[p] += size[dest];
            }
        };
        dfs(1, 0);

        auto ans = inf;
        for (auto i: range(N+1)) {
            for (auto j: range(P+1)) {
                debug  printValues(i, j, F[i][j]);
            }
        }
        for (auto i: range(1, N+1)) {
            chkMin(ans, F[i][P]);
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6491926694052276::solve();
    return 0;
}
