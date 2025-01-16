/**
 * @link https://www.luogu.com.cn/problem/P6869
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9651641365887104 {
    struct Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> size, depth, fa, son, top;
        std::vector<ll> diff;  // 差分数组；实际值为 diff[p] 加上整个子树

        auto dfs1(int p, int prev) -> void {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;
            for (auto x: graph.at(p))  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[son[p]] < size[x]) {
                    son[p] = x;
                }
            }
        }
        auto dfs2(int p, int tp) -> void {
            top[p] = tp;
            if (son[p] != 0)  dfs2(son[p], tp);
            for (auto x: graph.at(p)) {
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }
        Tree(int N): N(N), graph(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), diff(N+1) {}
        auto lca(int x, int y) const -> int {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            return y;
        }
        // 保证 y 为 x 的祖先
        // [x, y) 区间加 val
        auto addRange(int x, int y, int val) -> void {
            diff[y] -= val;
            diff[x] += val;
        }
        // 路径上的边权加 val
        auto addPath(int x, int y, int val) -> void {
            auto p = lca(x, y);
            addRange(x, p, val);
            addRange(y, p, val);
        }
        // 计算差分前的点权
        auto query(auto &out, int p = 1, int prev = 0) const -> void {
            out.at(p) = diff[p];
            for (auto x: graph.at(p))  if (x != prev) {
                query(out, x, p);
                out.at(p) += out.at(x);
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        struct Edge {
            int x, y;
            int single, multiple;
        };
        std::vector<Edge> edges(N-1);
        Tree tree{N};
        for (auto &[x, y, s, m]: edges) {
            std::cin >> x >> y >> s >> m;
            tree.graph.at(x).push_back(y);
            tree.graph.at(y).push_back(x);
        }
        tree.dfs1(1, 0);
        tree.dfs2(1, 1);
        for (auto i: range(1, N)) {
            // 从 i 走到 i+1
            tree.addPath(i, i+1, 1);
        }
        std::vector<ll> counts(N+1);
        tree.query(counts);
        // 对于每一条边
        auto ans = 0LL;
        for (auto &[x, y, s, m]: edges) {
            if (tree.depth[x] < tree.depth[y]) {
                std::swap(x, y);
            }
            auto cnt = counts[x];
            if (cnt == 0)  continue;
            ans += std::min<ll>(cnt * s, m);
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9651641365887104::solve();
    return 0;
}
