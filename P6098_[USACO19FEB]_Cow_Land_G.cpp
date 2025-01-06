/**
 * @link https://www.luogu.com.cn/problem/P6098
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9883444830861043 {
    class BIT {
        std::vector<int> c;
        static constexpr int lowbit(int x) noexcept { return x & -x; }
    public:
        BIT(int N): c(N+1) {}
        void xorUpdate(int x, int val) {
            x++;
            while (x < (int)c.size()) {
                c.at(x) ^= val;
                x += lowbit(x);
            }
        }
        int xorPrefix(int x) {
            x++;  auto res = 0;
            while (x) {
                res ^= c.at(x);
                x -= lowbit(x);
            }
            return res;
        }
        int xorRange(int begin, int end) {
            auto res = xorPrefix(end-1);
            if (begin)  res ^= xorPrefix(begin-1);
            return res;
        }
        void assign(int x, int val) {
            auto cur = xorRange(x, x + 1);
            xorUpdate(x, val ^ cur);
        }
    };
    class Tree {
    public:
        std::vector<std::vector<int>> forward;
        std::vector<int> size, depth, fa, son, top, order, index;
        BIT bit;
        Tree(int N, std::vector<std::pair<int, int>> const &edges): 
                forward(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), bit(N+1) {
            for (auto [x, y]: edges)  addEdge(x, y);
            dfs1(1, 0);
            dfs2(1, 1);
        }
        void addEdge(int x, int y) {
            forward.at(x).push_back(y);
            forward.at(y).push_back(x);
        }
        void dfs1(int p, int prev = 0) {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;

            for (auto x: forward[p])  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[x] > size[son[p]])  son[p] = x;
            }
        }
        void dfs2(int p, int p_top) {
            top[p] = p_top, index[p] = order.size(), order.push_back(p);
            if (son[p])  dfs2(son[p], p_top);
            for (auto x: forward[p]) {
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }
        void assignPoint(int x, int val) {
            bit.assign(index[x], val);
        }
        int xorPath(int x, int y) {
            auto res = 0;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                res ^= bit.xorRange(index[top[x]], index[x]+1);
                debug  std::cout << std::format("res ^= xorRange({}, {}) -> {}", index[top[x]], index[x]+1, bit.xorRange(index[x], index[y]+1)) << std::endl;
                x = fa[top[x]];
            }
            if (depth[x] > depth[y])  std::swap(x, y);
            res ^= bit.xorRange(index[x], index[y]+1);
            debug  std::cout << std::format("res ^= xorRange({}, {}) -> {}", index[x], index[y]+1, bit.xorRange(index[x], index[y]+1)) << std::endl;
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        std::vector<int> e(N+1);
        for (auto &x: e | views::drop(1))  std::cin >> x;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;

        Tree tree(N, edges);
        debug  for (auto x: tree.order)  std::cout << x << ' ';
        debug  std::cout << std::endl;
        for (auto i: range(1, N+1))  tree.bit.assign(tree.index[i], e[i]);
        for (auto _: range(Q)) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                tree.assignPoint(x, y);
            } else {
                std::cout << tree.xorPath(x, y) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9883444830861043::solve();
    return 0;
}
