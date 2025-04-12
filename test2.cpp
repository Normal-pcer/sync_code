/**
 * @link https://www.luogu.com.cn/problem/P2633
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_5558157793800670 {
    class Tree {
        i32 n;
        std::vector<std::vector<i32>> graph;  // 临接表
        std::vector<i32> size, depth, fa, son, top;

        // 处理 size, depth, fa, son
        auto dfs1(i32 u, i32 prev) -> void {
            depth[u] = depth[prev] + 1;
            fa[u] = prev;
            size[u] = 1;
            for (auto v: graph[u]) {
                if (v != prev) {
                    dfs1(v, u);
                    size[u] += size[v];
                    if (size[v] > size[son[u]]) {
                        son[u] = v;
                    }
                }
            }
        }
        // 处理 top, order, index
        auto dfs2(i32 u, i32 t) -> void {
            top[u] = t;
            if (son[u] != 0) {
                dfs2(son[u], t);
            }
            for (auto v: graph[u]) {
                if (v != fa[u] and v != son[u]) {
                    dfs2(v, v);
                }
            }
        }
        
    public:
        Tree(i32 n, std::vector<std::pair<i32, i32>> const &edges, i32 root):
                n(n), graph(n + 1), size(n + 1), depth(n + 1), fa(n + 1), son(n + 1), top(n + 1) {
            for (auto [x, y]: edges) {
                graph[x].push_back(y);
                graph[y].push_back(x);
            }

            dfs1(root, 0);
            dfs2(root, root);
        }
        auto lca(i32 u, i32 v) -> i32 {
            while (top[u] != top[v]) {
                if (depth[top[u]] < depth[top[v]]) std::swap(u, v);
                u = fa[top[u]];
            }
            if (depth[u] > depth[v]) std::swap(u, v);
            return u;
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<std::pair<i32, i32>> edges(n - 1);
        std::vector<char> isChild(n + 1, false); 
        isChild[0] = true;
        for (auto &[x, y]: edges) std::cin >> x >> y, isChild[y] = true;

        i32 root = std::find(isChild.begin(), isChild.end(), false) - isChild.begin();
        Tree tr{n, edges, root};
        for (auto _ = m; _ --> 0; ) {
            i32 u, v;
            std::cin >> u >> v;
            std::cout << tr.lca(u, v) << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_5558157793800670::solve();
    return 0;
}
