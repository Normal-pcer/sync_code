/**
 * @link https://www.luogu.com.cn/problem/P2015
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_4488269194715641 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        struct Node {
            int to, val;
        };
        std::vector<std::vector<Node>> graph(N+1);
        for (auto _: range(N-1)) {
            int x, y, val;
            std::cin >> x >> y >> val;
            graph.at(x).push_back({y, val});
            graph.at(y).push_back({x, val});
        }
        std::vector F(N+1, std::vector<int>(Q+1));
        std::function<void(int, int)> dfs = [&](int p, int prev) {
            F[p][0] = 0;
            for (auto dest: graph.at(p))  if (dest.to != prev) {
                dfs(dest.to, p);
                for (auto j: range(Q, 0, -1)) {
                    for (auto k: range(0, j)) {
                        chkMax(F[p][j], F[p][j - 1 - k] + F[dest.to][k] + dest.val);
                    }
                }
            }
        };
        dfs(1, 0);
        auto ans = F[1][Q];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4488269194715641::solve();
    return 0;
}
