/**
 * @link https://www.luogu.com.cn/problem/P3833
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1528974063237535 {
    class SegTree {
        struct Node {
            int begin, end;
            ll sum = 0, add_tag = 0;
        };
        std::vector<Node> tr;
        static int constexpr lson(int p) { return p << 1; }
        static int constexpr rson(int p) { return p << 1 | 1; }
        void pushUp(int p) {
            tr[p].sum = tr[lson(p)].sum + tr[rson(p)].sum;
        }
        void addNode(int p, int val) {
            tr[p].sum += val * (tr[p].end - tr[p].begin);
            tr[p].add_tag += val;
        }
        void pushDown(int p) {
            if (tr[p].add_tag != 0) {
                for (auto s: {lson(p), rson(p)}) {
                    addNode(s, tr[p].add_tag);
                }
                tr[p].add_tag = 0;
            }
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N): tr((N+1) << 2) {
            build(0, N, 1);
        }
        void addRange(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  addRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  addRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto sumRange(int begin, int end, int p = 1) -> ll {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].sum;
            }
            pushDown(p);
            auto res = 0LL;
            if (tr[lson(p)].end > begin)  res += sumRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res += sumRange(begin, end, rson(p));
            return res;
        }
    };
    class Tree {
        int N;
        std::vector<std::vector<int>> forward;
        std::vector<int> size, depth, fa, son, top, order, index;
        SegTree sgt;
        void dfs1(int p, int prev) {
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
            if (son[p] != N)  dfs2(son[p], tp);
            for (auto x: forward.at(p)) {
                if (x != fa[p] and x != son[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N, auto const &edges): N(N), forward(N+1), size(N+1, 0), depth(N+1, 0), fa(N, N), son(N, N), top(N), index(N), sgt(N) {
            for (auto [x, y]: edges) {
                forward.at(x).push_back(y);
                forward.at(y).push_back(x);
            }
            dfs1(0, N);  // N 表示无效点
            dfs2(0, 0);
        }
        void addPath(int x, int y, int val) {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                sgt.addRange(index[top[x]], index[x] + 1, val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            sgt.addRange(index[y], index[x] + 1, val);
        }
        auto sumChildren(int p) -> ll {
            auto idx = index[p];
            auto sz = size[p];
            return sgt.sumRange(idx, idx + sz);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;

        Tree tree{N, edges};
        int Q;  std::cin >> Q;
        for (auto _ = 0; _ < Q; _++) {
            char op;  std::cin >> op;
            int x;  std::cin >> x;
            if (op == 'A') {
                int y, z;  std::cin >> y >> z;
                tree.addPath(x, y, z);
            } else {
                std::cout << tree.sumChildren(x) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1528974063237535::solve();
    return 0;
}
