/**
 * @link https://www.luogu.com.cn/problem/P3128
 */

#include "./lib_v2.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_7216935459152744 {

    int N, K;
    const int _N = 5e4+5, _ST = 20;
    int ST;
    std::vector<std::vector<int>> graph;

    std::vector<int> diff;  // 点权的差分
    std::vector<int> val;
    std::vector<int> depth;
    std::vector<std::vector<int>> F;  // 倍增

    void dfs_pre(int p, int prev) {
        depth.at(p) = depth.at(prev) + 1;
        F.at(p).at(0) = prev;
        for (auto i: range(1, (int)F.at(p).size())) {
            F.at(p).at(i) = F.at( F.at(p).at(i-1) ).at(i-1);
        }
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_pre(dest, p);
        }
    }

    std::pair<int, bool> LCA(int x, int y) {
        auto distance = depth.at(x) - depth.at(y);
        for (auto i = 0; i < ST; i++) {
            if (distance & (1<<i)) {
                x = F.at(x).at(i);
            }
        }
        if (x == y)  return {x, true};
        for (int i = ST-1; i >= 0; i--) {
            if (F.at(x).at(i) != F.at(y).at(i)) {
                x = F.at(x).at(i), y = F.at(y).at(i);
            }
        }

        return {F.at(x).at(0), false};
    }

    void modify(int x, int y) {
        if (depth.at(x) < depth.at(y))  std::swap(x, y);
        auto [ancestor, single] = LCA(x, y);

        if (single)  /*std::cout << '!' << std::endl,*/ diff.at(x)++, diff.at(F.at(y).at(0))--;
        else  diff.at(x)++, diff.at(y)++, diff.at(ancestor)--, diff.at(F.at(ancestor).at(0))--;
    }

    void dfs_finally(int p, int prev) {
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_finally(dest, p);
            val.at(p) += val.at(dest);
        }
        val.at(p) += diff.at(p);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N >> K;
        graph.resize(N+1);
        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y), graph.at(y).push_back(x);
        }

        ST = std::__lg(N) + 1;
        depth.resize(N+1), std::fill_n(std::back_inserter(F), N+1, std::vector<int>(ST));
        diff.resize(N+1), val.resize(N+1);

        dfs_pre(1, 0);
        for (auto _: range(K)) {
            int x, y;  std::cin >> x >> y;
            modify(x, y);

            debug {
                for (auto i: range(1, N+1))  std::cout << std::format("[{}]{}", i, diff.at(i)) << ' ';
                std::cout << std::endl;
            }
        }

        dfs_finally(1, 0);

        std::cout << rgs::max(val) << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_7216935459152744::solve();
    return 0;
}