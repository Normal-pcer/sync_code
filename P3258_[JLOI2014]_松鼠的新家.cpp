/**
 * @link https://www.luogu.com.cn/problem/P3258
 */

#include "./lib"
#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1516458592815387 {

    int N, ST;
    std::vector<std::vector<int>> graph;
    std::vector<int> val, diff;
    std::vector<std::vector<int>> F;  // 倍增，i 向上跳 (1<<j) 次
    std::vector<int> depth;
    std::vector<int> order;

    void dfs_pre(int p, int prev) {
        depth.at(p) = depth.at(prev) + 1;

        F.at(p).at(0) = prev;
        from(j, 1, ST)  F.at(p).at(j) = F.at( F.at(p).at(j-1) ).at(j-1);

        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_pre(dest, p);
        }
    }

    std::pair<int, bool> LCA(int x, int y) {
        assert(depth.at(x) >= depth.at(y));

        auto distance = depth.at(x) - depth.at(y);
        for (auto i = 0; i < ST; i++) {
            if (distance & (1<<i)) {
                x = F.at(x).at(i);
            }
        }

        if (x == y)  return {x, true};

        for (auto i = ST-1; i >= 0; i--) {
            if (F.at(x).at(i) != F.at(y).at(i)) {
                x = F[x][i], y = F[y][i];
            }
        }

        return {F.at(x).at(0), false};
    }

    void modify(int x, int y) {
        auto x_cpy = x;
        if (depth.at(x) < depth.at(y))  std::swap(x, y);
        auto [ancestor, chain] = LCA(x, y);

        if (chain) {
            diff.at(x)++, diff.at(F.at(y).at(0))--;
        } else {
            diff.at(x)++, diff.at(y)++;
            diff.at(ancestor)--, diff.at(F.at(ancestor).at(0))--;
        }
        if (x_cpy != order.front())  diff.at(x_cpy)--, diff.at(F.at(x_cpy).at(0))++;
    }

    void dfs_finally(int p, int prev) {
        val.at(p) = diff.at(p);
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_finally(dest, p);
            val.at(p) += val.at(dest);
        }
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        io >> N;
        ST = std::__lg(N+1) + 1;
        graph.resize(N+1), val.resize(N+1), diff.resize(N+1), depth.resize(N+1), order.resize(N);
        std::fill_n(std::back_inserter(F), N+1, std::vector<int>(ST));

        for (auto &i: order)  io >> i;

        from(_, 0, N-1) {
            int x, y;  io >> x >> y;
            graph.at(x).push_back(y), graph.at(y).push_back(x);
        }

        dfs_pre(1, 0);
        from(i, 1, N) {
            modify(order.at(i-1), order.at(i));
            debug {
                from(i, 0, N+1)  io << diff.at(i) << ' ';
                io << endl;
            }
        }
        dfs_finally(1, 0);
        val.at(order.back())--;

        from(i, 1, N+1) {
            io << val.at(i) << endl;
        }
    }
}

int main() {
    initDebug;
    Solution_1516458592815387::solve();
    return 0;
}
