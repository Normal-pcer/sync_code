/**
 * @link https://www.luogu.com.cn/problem/AT_abc246_g
 * @link https://atcoder.jp/contests/abc246/tasks/abc246_g
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_6922078573265128 {
    class Tree {
        int N;
        std::vector<std::vector<int>> forward;
    public:
        std::vector<int> val;
        Tree(int N, std::vector<std::pair<int, int>> const &edges): N(N), forward(N+1), val(N+1) {
            for (auto [x, y]: edges)  addEdge(x, y);
        }
        void addEdge(int x, int y) {
            forward.at(x).push_back(y);
            forward.at(y).push_back(x);
        }
        void _dfs(int p, int prev, int x, std::vector<int> &F) const {
            for (auto dest: forward.at(p))  if (dest != prev) {
                _dfs(dest, p, x, F);
                F[p] += F[dest];
            }
            F[p] -= 1, chkMax(F[p], 0);
            if (val[p] >= x)  F[p]++;
        }
        auto check(int x) -> bool {
            std::vector<int> F(N+1);
            _dfs(1, 0, x, F);
            return F[1] != 0;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> sorted(N+1);
        for (auto &x: sorted | views::drop(2))  std::cin >> x;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;
        
        Tree tree{N, edges};
        tree.val = sorted;
        
        ranges::sort(sorted, ranges::less{});
        auto point = ranges::partition_point(sorted, lam(x, tree.check(x)));
        auto ans = *--point;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6922078573265128::solve();
    return 0;
}
