/**
 * @link https://www.luogu.com.cn/problem/P3979
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1542105660219375 {
    class SegTree {
        struct Node {
            int begin, end;
            int min = 2147483647, assign_tag = -1;
        };
        std::vector<Node> tr;

        static int constexpr lson(int p) { return p << 1; }
        static int constexpr rson(int p) { return p << 1 | 1; }
        void assignNode(int p, int val) {
            tr[p].min = val;
            tr[p].assign_tag = val;
        }
        void pushUp(int p) {
            tr[p].min = std::min(tr[lson(p)].min, tr[rson(p)].min);
        }
        void pushDown(int p) {
            if (tr[p].assign_tag != -1) {
                for (auto s: {lson(p), rson(p)}) {
                    assignNode(s, tr[p].assign_tag);
                }
                tr[p].assign_tag = -1;
            }
        }
        void build(int begin, int end, auto const &init, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].min = init[begin];
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, init, lson(p));
            build(mid, end, init, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N, auto &init): tr(N << 2) {
            build(0, N, init, 1);
        }
        void assignRange(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                assignNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  assignRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  assignRange(begin, end, val, rson(p));
            pushUp(p);
        }
        int minRange(int begin, int end, int p = 1) {
            if (begin >= end)  return 2147483647;
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].min;
            }
            auto res = 2147483647;
            pushDown(p);
            if (tr[lson(p)].end > begin)  chkMin(res, minRange(begin, end, lson(p)));
            if (tr[rson(p)].begin < end)  chkMin(res, minRange(begin, end, rson(p)));
            return res;
        }
    };
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> size, depth, fa, son, top, order, index;
        std::unique_ptr<SegTree> sgt;
        int N;

        void dfs1(int p, int prev = 0) {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;
            for (auto x: forward.at(p))  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[son[p]] < size[x]) {
                    son[p] = x;
                }
            }
        }
        void dfs2(int p, int tp) {
            top[p] = tp, index[p] = order.size(), order.push_back(p);
            if (son[p] != 0)  dfs2(son[p], tp);
            for (auto x: forward.at(p)) {
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N, auto const &init, auto const &edges): forward(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), N(N) {
            for (auto [x, y]: edges) {
                forward.at(x).push_back(y);
                forward.at(y).push_back(x);
            }
            dfs1(1, 0);
            dfs2(1, 1);
            std::vector<int> sgt_init(N);
            for (auto i: range(N))  sgt_init[i] = init[order[i]];
            sgt = std::make_unique<SegTree>(N, sgt_init);
        }
        void assignPath(int x, int y, int val) {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                sgt->assignRange(index[top[x]], index[x] + 1, val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            sgt->assignRange(index[y], index[x] + 1, val);
        }
        int minChild(int x, int root) {
            if (x == root)  return sgt->minRange(0, N);
            // 如果 x 是 root 的父亲
            if (index[x] <= index[root] and index[root] < index[x] + size[x] and root != x) {
                // 查找 x 的儿子 s，s 是 root 的父亲
                auto s = [&]() {
                    auto t = root;
                    while (top[t] != top[x]) {
                        if (fa[top[t]] == x) {  // 链头即为所求
                            return top[t];
                        }
                        t = fa[top[t]];  // 向上跳
                    }
                    return son[x];  // 在重链上
                }();
                // 整棵树，去掉 s 的子树
                auto [begin0, end0] = std::pair{0, index[s]};
                auto [begin1, end1] = std::pair{index[s]+size[s], N};
                auto ans = 2147483647;
                if (end0 > begin0)  chkMin(ans, sgt->minRange(begin0, end0));
                if (end1 > begin1)  chkMin(ans, sgt->minRange(begin1, end1));
                return ans;
            } else {
                // 正常查询
                return sgt->minRange(index[x], index[x] + size[x]);
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, M;  std::cin >> N >> M;

        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;
        std::vector<int> w(N+1);
        w[0] = 2147483647;
        for (auto &x: w | views::drop(1))  std::cin >> x;
        Tree tree{N, w, edges};

        int root;  std::cin >> root;
        for (auto _: range(M)) {
            int op, x;  std::cin >> op >> x;
            if (op == 1) {
                root = x;
            } else if (op == 2) {
                int y, z;  std::cin >> y >> z;
                tree.assignPath(x, y, z);
            } else {
                std::cout << tree.minChild(x, root) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1542105660219375::solve();
    return 0;
}
