/**
 * @link https://www.luogu.com.cn/problem/P3384
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1450373477842811 {
    int mod;
#if true
    class SegTree {
        struct Node {
            int begin, end;
            ll sum, add_tag;
        };
        std::vector<Node> tree;
        std::vector<int> init;
        static int constexpr lson(int x) noexcept { return x << 1; }
        static int constexpr rson(int x) noexcept { return (x << 1) | 1; }
        void pushUp(int p) {
            tree[p].sum = (tree[lson(p)].sum + tree[rson(p)].sum) % mod;
        }
        void addNode(int p, int val) {
            tree[p].sum += (ll)val * (tree[p].end - tree[p].begin) % mod, tree[p].sum %= mod;
            tree[p].add_tag += (ll)val * 1 % mod, tree[p].add_tag %= mod;
        }
        void pushDown(int p) {
            if (tree[p].add_tag) {
                for (auto son: {lson(p), rson(p)}) {
                    addNode(son, tree[p].add_tag);
                }
                tree[p].add_tag = 0;
            }
        }
        void build(int begin, int end, int p) {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end)  return tree[p].sum = init.at(begin), void();
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N, std::vector<int> &&init): tree(N << 2), init(std::move(init)) {
            build(0, N, 1);
        }
        SegTree(SegTree &&other): tree(std::move(other.tree)), init(std::move(other.init)) {}
        int sum(int begin, int end, int p = 1) {
            if (tree[p].begin >= begin and tree[p].end <= end)  return tree[p].sum;
            pushDown(p);
            auto res = 0;
            if (tree[lson(p)].end > begin)  res += sum(begin, end, lson(p)), res %= mod;
            if (tree[rson(p)].begin < end)  res += sum(begin, end, rson(p)), res %= mod;
            return res;
        }
        void add(int begin, int end, int val, int p = 1) {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  add(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  add(begin, end, val, rson(p));
            pushUp(p);
        }
    };
#else
    class SegTree {
        std::vector<int> init;
    public:
        SegTree(int, std::vector<int> &&init): init(std::move(init)) {}
        int sum(int begin, int end) {
            auto res = 0;
            for (auto i = begin; i != end; i++)  res += init[i], res %= mod;
            return res;
        }
        void add(int begin, int end, int val) {
            for (auto i = begin; i != end; i++)  init[i] += val, init[i] %= mod;
        }
    };
#endif
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> size, depth, fa, son;
        std::vector<int> top;
    public:
        std::vector<int> order, index;
        SegTree sgt;
        auto initSgt(int N, std::vector<int> &&val, std::vector<std::pair<int, int>> &&edges, int root) {
            for (auto [x, y]: edges)  addEdge(x, y);
            dfs1(root), dfs2(root, root);
            std::vector<int> init(N);
            for (auto i: range(N))  init[i] = val.at(order[i]);
            return SegTree(N, std::move(init));
        }
        Tree(int N, std::vector<int> &&val, std::vector<std::pair<int, int>> &&edges, int root): forward(N+1), size(N+1), depth(N+1), 
                fa(N+1), son(N+1), top(N+1), index(N+1), sgt(std::move(initSgt(N, std::move(val), std::move(edges), root))) {}
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
        void dfs2(int p, int t = 0) {
            index.at(p) = order.size(), order.push_back(p);
            top[p] = t;
            if (son[p])  dfs2(son[p], t);
            for (auto x: forward.at(p))  if (x != fa[p] and x != son[p]) {
                dfs2(x, x);
            }
        }
        void addPath(int x, int y, int val) {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                sgt.add(index[top[x]], index[x] + 1, val);
                debug  std::cout << index[top[x]] << ' ' << index[x] + 1 << std::endl;
                x = fa[top[x]];
            }
            if (depth[x] > depth[y])  std::swap(x, y);
            sgt.add(index[x], index[y] + 1, val);
        }
        int sumPath(int x, int y) {
            auto sum = 0;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                sum += sgt.sum(index[top[x]], index[x] + 1), sum %= mod;
                x = fa[top[x]];
            }
            if (depth[x] > depth[y])  std::swap(x, y);
            sum += sgt.sum(index[x], index[y] + 1), sum %= mod;
            return sum;
        }
        void addSub(int root, int val) {
            auto idx = index[root];
            auto sz = size[root];
            sgt.add(idx, idx + sz, val);
        }
        int sumSub(int root) {
            auto idx = index[root];
            auto sz = size[root];
            return sgt.sum(idx, idx + sz);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, R;  std::cin >> N >> M >> R >> mod;
        std::vector<int> init(N+1);
        for (auto &x: init | views::drop(1))  std::cin >> x;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;

        Tree tree(N, std::move(init), std::move(edges), R);
        for (auto _: range(M)) {
            int op, x;  std::cin >> op >> x;
            if (op == 1) {
                int y, z;  std::cin >> y >> z;
                tree.addPath(x, y, z);
            } else if (op == 2) {
                int y;  std::cin >> y;
                std::cout << tree.sumPath(x, y) % mod << endl;
            } else if (op == 3) {
                int z;  std::cin >> z;
                tree.addSub(x, z);
            } else {
                std::cout << tree.sumSub(x) % mod << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1450373477842811::solve();
    return 0;
}
