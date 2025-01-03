/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3055
 */

#include "./lib_v3.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_4353409486511738 {
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> size, depth, son, fa;
        std::vector<int> top;
    public:
        Tree(int N): forward(N+1), size(N+1), depth(N+1), son(N+1), fa(N+1), top(N+1) {}
        void addEdge(int x, int y) {
            forward.at(x).push_back(y);
            forward.at(y).push_back(x);
        }
        // size, depth, fa, son
        void dfs1(int p, int prev = 0) {
            size[p] = 1;
            depth[p] = depth[prev] + 1;
            fa[p] = prev;
            for (auto x: forward.at(p))  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[x] > size[son[p]])  son[p] = x;
            }
        }

        // top
        void dfs2(int p, int prev = 0) {
            top[p] = prev;
            if (son[p])  dfs2(son[p], prev);
            for (auto x: forward.at(p))  if (x != fa[p] and x != son[p]) {
                dfs2(x, x);
            }
        }

        int LCA(int x, int y) {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                assert(depth[top[x]] >= depth[top[y]]);
                x = fa[top[x]];
            }
            if (depth[x] > depth[y])  return y;
            else  return x;
        }
    };
    void solve() {
        int N, K;  io >> N >> K;
        std::vector<char> isSon(N+1);
        Tree tree(N);
        for (auto _ = 0; _ < N-1; _++) {
            int x, y;  io >> x >> y;
            isSon.at(y) = true;
            tree.addEdge(x, y);
        }
        isSon.at(0) = true;
        auto root = std::distance(isSon.begin(), std::find(isSon.begin(), isSon.end(), 0));
        tree.dfs1(root), tree.dfs2(root);
        for (auto _ = 0; _ < K; _++) {
            int x, y;  io >> x >> y;
            io << tree.LCA(x, y) << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4353409486511738::solve();
    return 0;
}
