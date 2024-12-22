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
        std::vector size(N+1, 1);
        
        auto dfs = [&](this auto &&dfs, int p, int prev) {
            for (auto dest: graph.at(p))  if (dest.to != prev) {
                for (auto j: range(size.at(dest.to) + size.at(p) - 1, 0, -1)) {
                    for (auto k: range(std::max(0, j - size.at(p)), std::min(size.at(dest.to), j + 1))) {
                        chkMax(F[p][j], F[p][j - k - 1] + F[dest.to][k] + dest.val);
                    }
                }
                size.at(p) += size.at(dest.to);
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
