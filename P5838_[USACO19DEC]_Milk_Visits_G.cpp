/**
 * @link https://www.luogu.com.cn/problem/P5838
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1334361110313078 {
    class Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> size, depth, fa, son, top, order, index;
        std::vector<std::vector<int>> types;  // 类型为 i 的所有点的 index

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
            top[p] = tp, index[p] = order.size(), order.push_back(p);
            if (son[p] != 0)  dfs2(son[p], tp);
            for (auto x: graph.at(p)) {
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): N(N), graph(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), types(N+1) {}

        auto findType(int begin, int end, int type) const -> bool {
            return ranges::lower_bound(types[type], begin) != ranges::lower_bound(types[type], end); 
        }
        auto queryPath(int x, int y, int type) const -> bool {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                if (findType(index[top[x]], index[x] + 1, type))  return true;
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            if (findType(index[y], index[x] + 1, type))  return true;
            return false;
        }

        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            std::vector<int> values(N);
            for (auto &x: values)  st >> x;
            for (auto _: range(N-1)) {
                int x, y;  st >> x >> y;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            tr.dfs1(1, 0);
            tr.dfs2(1, 1);
            for (auto i: range(N)) {
                auto type = values[tr.order[i] - 1];
                tr.types[type].push_back(i);  // 可以确保有序
            }
            return st;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        Tree tree{N};  std::cin >> tree;
        for (auto _: range(M)) {
            int x, y, z;  std::cin >> x >> y >> z;
            std::cout << tree.queryPath(x, y, z);
        }
        std::cout << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1334361110313078::solve();
    return 0;
}
