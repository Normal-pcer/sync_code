/**
 * @link https://www.luogu.com.cn/problem/P1505
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1823244422247816 {
    // 数据特征
    struct Feature {
        int min = +inf, max = -inf, sum = 0;

        auto operator|= (Feature other) -> Feature & {
            chkMin(min, other.min);
            chkMax(max, other.max);
            sum += other.sum;
            return *this;
        }
        auto operator| (Feature other) const -> Feature {
            auto res = *this;
            res |= other;
            return res;
        }
        auto operator- () const -> Feature {
            return {-max, -min, -sum};
        }
    };
    // 区间查询 min max sum
    // 单点赋值 区间取反（x = -x）
    class SegTree {
        struct Node {
            int begin, end;
            Feature val;
            bool nega_tag = false;
        };
        std::vector<Node> tr;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }
        auto negateNode(int p) -> void {
            tr[p].val = -tr[p].val;
            tr[p].nega_tag = not tr[p].nega_tag;
        }
        auto assignNode(int p, int val) -> void {
            tr[p].val.min = tr[p].val.max = tr[p].val.sum = val;
        }
        auto pushUp(int p) -> void {
            tr[p].val = tr[lson(p)].val | tr[rson(p)].val;
        }
        auto pushDown(int p) -> void {
            if (tr[p].nega_tag) {
                for (auto s: {lson(p), rson(p)}) {
                    negateNode(s);
                }
                tr[p].nega_tag = false;
            }
        }
        auto build(int begin, int end, int p = 1) -> void {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N): tr(N << 2) {
            build(0, N, 1);
        }
        auto assignAt(int begin, int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= begin + 1) {
                assignNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  assignAt(begin, val, lson(p));
            else  assignAt(begin, val, rson(p));
            pushUp(p);
        }
        auto negateRange(int begin, int end, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                negateNode(p);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  negateRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  negateRange(begin, end, rson(p));
            pushUp(p);
        }
        auto queryRange(int begin, int end, int p = 1) -> Feature {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].val;
            }
            pushDown(p);
            Feature res;
            if (tr[lson(p)].end > begin)  res |= queryRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res |= queryRange(begin, end, rson(p));
            return res;
        }
    };
    class Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> depth, size, fa, son, top, order, index;
        std::vector<int> fa_val;
        std::vector<std::tuple<int, int, int>> edges;
        SegTree sgt;
        
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
            if (son[p] != N)  dfs2(son[p], tp);
            for (auto x: graph.at(p)) {
                if (x != fa[p] and x != son[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): N(N), graph(N+1), depth(N+1), size(N+1), fa(N+1), son(N+1, N), top(N+1), index(N+1), fa_val(N+1), edges(N-1), sgt(N) {}

        auto assignEdge(int idx, int val) -> void {
            auto [x, y, prev] = edges[idx - 1];
            if (depth[x] < depth[y])  std::swap(x, y);
            sgt.assignAt(index[x], val);
        }
        auto negativePath(int x, int y) -> void {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                sgt.negateRange(index[top[x]], index[x] + 1);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            sgt.negateRange(index[y] + 1, index[x] + 1);
        }
        auto queryPath(int x, int y) -> Feature {
            Feature res;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                res |= sgt.queryRange(index[top[x]], index[x] + 1);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            res |= sgt.queryRange(index[y] + 1, index[x] + 1);
            return res;
        }

        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            for (auto i = 0; i < N - 1; i++) {
                int x, y, val;  st >> x >> y >> val;
                tr.edges[i] = {x, y, val};
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            tr.dfs1(0, N), tr.dfs2(0, 0);
            std::vector<int> init(N+1);
            for (auto [x, y, val]: tr.edges) {
                if (tr.depth[x] < tr.depth[y]) {
                    std::swap(x, y);
                }
                tr.fa_val[x] = val;
                init[x] = val;
            }
            for (auto i = 0; i < N; i++) {
                tr.sgt.assignAt(i, init[tr.order[i]]);
            }
            return st;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        Tree tree{N};  std::cin >> tree;
        int Q;  std::cin >> Q;
        for (auto _ = 0; _ < Q; _++) {
            std::string op;  std::cin >> op;
            int x, y;  std::cin >> x >> y;
            if (op == "C")  tree.assignEdge(x, y);
            else if (op == "N")  tree.negativePath(x, y);
            else if (op == "SUM")  std::cout << tree.queryPath(x, y).sum << endl;
            else if (op == "MAX")  std::cout << tree.queryPath(x, y).max << endl;
            else  std::cout << tree.queryPath(x, y).min << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1823244422247816::solve();
    return 0;
}
