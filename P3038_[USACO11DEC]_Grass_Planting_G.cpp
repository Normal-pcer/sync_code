/**
 * @link https://www.luogu.com.cn/problem/P3038
 */

#include "./libs/debug_macros.hpp"


#include "./lib_v3.hpp"


using namespace lib;

namespace Solution_3050143155000094 {
    class SegTree {
        struct Node {
            int begin, end;
            int add_tag;
        };
        std::vector<Node> tree;
        static int constexpr lson(int p) { return (p << 1); }
        static int constexpr rson(int p) { return (p << 1) | 1; }
        void addNode(int p, int val) {
            tree[p].add_tag += val;
        }
        void pushDown(int p) {
            if (tree[p].add_tag != 0) {
                for (auto son: {lson(p), rson(p)}) {
                    addNode(son, tree[p].add_tag);
                }
                tree[p].add_tag = false;
            }
        }
        void pushUp(int) {}
        void build(int begin, int end, int p = 1) {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N): tree(N << 2) {
            build(0, N, 1);
        }
        int at(int pos, int p = 1) {
            if (tree[p].begin == pos and pos + 1 == tree[p].end) {
                return tree[p].add_tag;
            }
            pushDown(p);
            if (tree[lson(p)].end > pos)  return at(pos, lson(p));
            else  return at(pos, rson(p));
        }
        void add(int begin, int end, int val, int p = 1) {
            debug  std::cout << std::format("add({}, {}, {}, {})", begin, end, val, p) << std::endl;
            if (tree[p].begin >= begin and tree[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  add(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  add(begin, end, val, rson(p));
        }
    };
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> fa, size, son, depth, top;
        std::vector<int> order, index;
        SegTree sgt;
    public:
        Tree(int N, std::vector<std::pair<int, int>> const &edges): 
                forward(N+1), fa(N+1), size(N+1), son(N+1), depth(N+1), top(N+1), index(N+1), sgt(N+1) {
            for (auto [x, y]: edges)  addBinEdge(x, y);
            dfs1(1, 0);
            dfs2(1, 1);
            debug  std::cout << "order:" << std::endl;
            debug  for (auto x: order)  std::cout << x << ' ';
            debug  std::cout << std::endl;
        }
        void addBinEdge(int x, int y) {
            forward.at(x).push_back(y);
            forward.at(y).push_back(x);
        }
        void dfs1(int p, int prev = 0) {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;
            for (auto x: forward.at(p))  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[x] > size[son[p]])  son[p] = x;
            }
        }
        void dfs2(int p, int p_top) {
            top[p] = p_top, index.at(p) = order.size(), order.push_back(p);
            if (son[p])  dfs2(son[p], p_top);
            for (auto x: forward.at(p)) {
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }

        void addPath(int x, int y) {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                debug  std::cout << std::format("ccc {} {}", x, fa[top[x]]) << std::endl;
                sgt.add(index[top[x]], index[x] + 1, 1);
                x = fa[top[x]];
            }
            if (depth[x] > depth[y])  std::swap(x, y);
            debug  std::cout << std::format("ccc {} {}", y, x) << std::endl;
            sgt.add(index[x] + 1, index[y] + 1, 1);
        }

        int queryEdge(int x, int y) {
            if (fa[x] == y)  std::swap(x, y);
            assert(x == fa[y]);
            return sgt.at(index[y]);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, M;  std::cin >> N >> M;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;
        Tree tree{N, edges};
        for (auto _ = 0; _ < M; _++) {
            char op;
            int u, v;
            std::cin >> op >> u >> v;
            if (op == 'P') {
                tree.addPath(u, v);
            } else {
                std::cout << tree.queryEdge(u, v) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3050143155000094::solve();
    return 0;
}
