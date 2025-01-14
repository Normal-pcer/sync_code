/**
 * @link https://www.luogu.com.cn/problem/SP6779
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1053946509494966 {
    struct Node {
        int begin = 0, end = 0;
        int sum = 0;
        int max = -inf, lmax = -inf, rmax = -inf;
        int assignTag = -inf;

        auto length() const -> int { return end - begin; }
        auto empty() const -> bool { return length() == 0; }
        auto assign(int val) -> void {
            if (val >= 0)  sum = max = lmax = rmax = length() * val;
            else  sum = length() * val, max = lmax = rmax = 0;
            assignTag = val;
        }
        auto operator| (Node other) const -> Node {
            if (empty())  return other;
            if (other.empty())  return *this;
            Node res;
            res.begin = begin, res.end = other.end;
            res.sum = sum + other.sum;
            res.max = std::max({max, other.max, rmax + other.lmax});
            res.lmax = std::max(lmax, sum + other.lmax);
            res.rmax = std::max(other.rmax, rmax + other.sum);
            return res;
        }
        auto operator|= (Node other) -> Node & {
            *this = *this | other;
            return *this;
        }
        auto operator~ () const -> Node {
            return {begin, end, sum, max, rmax, lmax};
        }
        // operator std::string () const {
        //     return std::format("({}..{}, sum={}, max={}, lmax={}, rmax={}, tag={})", begin, end, sum, max, lmax, rmax, assignTag);
        // }
    };
    class SegTree {
        std::vector<Node> tr;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }
        auto pushDown(int p) -> void {
            if (tr[p].assignTag != -inf) {
                for (auto s: {lson(p), rson(p)}) {
                    tr[s].assign(tr[p].assignTag);
                }
                tr[p].assignTag = -inf;
            }
        }
        auto pushUp(int p) -> void {
            tr[p] = tr[lson(p)] | tr[rson(p)];
        }
        auto build(int begin, int end, int p) -> void {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = begin + ((end - begin) >> 1);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N): tr(N << 2) {
            build(0, N, 1);
        }
        auto queryRange(int begin, int end, int p = 1) -> Node {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p];
            }
            Node res;
            pushDown(p);
            if (tr[lson(p)].end > begin)  res |= queryRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res |= queryRange(begin, end, rson(p));
            return res;
        }
        auto assignRange(int begin, int end, int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].assign(val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  assignRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  assignRange(begin, end, val, rson(p));
            pushUp(p);
        }
    };
    class Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> size, depth, fa, son, top, order, index;
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
            top[p] = tp, index[p] = order.size();
            order.push_back(p);
            if (son[p] != 0)  dfs2(son[p], tp);
            for (auto x: graph.at(p)) {
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): N(N), graph(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), sgt(N) {}
        auto queryPath(int x, int y) -> Node {
            Node x_res;  // x -> lca
            Node y_res;  // y -> lca
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                    std::swap(x_res, y_res);
                }
                x_res |= ~sgt.queryRange(index[top[x]], index[x] + 1);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
                std::swap(x_res, y_res);
            }
            x_res |= ~sgt.queryRange(index[y], index[x] + 1);  // x -> lca（包含）
            auto y_res_rev = ~y_res;  // （不含）lca -> y
            return x_res | y_res_rev;
        }
        auto assignPath(int x, int y, int val) -> void {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                sgt.assignRange(index[top[x]], index[x] + 1, val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
            }
            sgt.assignRange(index[y], index[x] + 1, val);
        }
        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            std::vector<int> w(N);
            for (auto &x: w)  st >> x;
            for (auto _ = 0; _ < N - 1; _++) {
                int x, y;  st >> x >> y;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            tr.dfs1(1, 0), tr.dfs2(1, 1);
            for (auto i = 0; i < N; i++)  tr.sgt.assignRange(tr.index[i+1], tr.index[i+1] + 1, w[i]);
            return st;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        Tree tr{N};  std::cin >> tr;
        int Q;  std::cin >> Q;
        for (auto _ = 0; _ < Q; _++) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                std::cout << tr.queryPath(x, y).max << endl;
            } else {
                int z;  std::cin >> z;
                tr.assignPath(x, y, z);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1053946509494966::solve();
    return 0;
}
