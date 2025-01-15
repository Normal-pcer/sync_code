/**
 * @link https://www.luogu.com.cn/problem/P7735
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9288857495666426 {
    /**
     * 给每个点设置一个点权。
     * 令端点权值相同的边为重边，不同的为轻边。
     * 查询，直接维护即可。
     * 修改，将一整条链“剥离”，只需设置一个全新的权值。
     */
    struct Node {
        int begin = 0, end = 0;
        // 区间内的重边数量
        // 仅当区间位于一条链上时有效
        int count = 0;
        int lc = -1;  // 区间左侧的颜色
        int rc = -1;  // 区间右侧的颜色
        int assign_tag = -1;

        auto size() const -> int { return end - begin; }
        auto empty() const -> bool { return size() == 0; }
        // 整体染色为 val
        auto assign(int val) -> void {
            count = size() - 1;  // 所有边均为重边
            lc = rc = val;
            assign_tag = val;
        }

        // 按顺序合并区间
        auto operator| (Node other) const -> Node {
            if (empty())  return other;
            if (other.empty())  return *this;
            Node res {begin, other.end};
            res.count = count + other.count;
            if (rc == other.lc)  res.count++;  // 中间的边也为重边
            res.lc = lc, res.rc = other.rc;
            return res;
        };
        auto operator|= (Node other) -> Node & {
            return *this = *this | other;
        }
        // 反转一个区间
        auto operator~ () -> Node {
            return {begin, end, count, rc, lc, assign_tag};
        }
        operator std::string () {
            return std::format("({}..{}, {}, l={}, r={}, tag={})", begin, end, count, lc, rc, assign_tag);
        }
    };
    class SegTree {
        std::vector<Node> tr;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }
        auto pushUp(int p) -> void {
            tr[p] = tr[lson(p)] | tr[rson(p)];
        }
        auto pushDown(int p) -> void {
            if (tr[p].assign_tag != -1) {
                for (auto s: {lson(p), rson(p)}) {
                    tr[s].assign(tr[p].assign_tag);
                }
                tr[p].assign_tag = -1;
            }
        }
        auto build(int begin, int end, int p) -> void {
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
        std::vector<int> depth, size, fa, son, top, order, index;
        SegTree sgt;
        int val = 0;  // 给新的修改操作分配的全新权值

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
                if (x != fa[p] and x != son[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): N(N), graph(N+1), depth(N+1), size(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), sgt(N) {}
        // 操作 1
        // 将 x->y 所有点的临边变为轻边，再将所有路径上的边变成重边
        auto updatePath(int x, int y) -> void {
            auto cur_val = val++;  // 本次操作用到的权值
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                debug  std::cout << std::format("{} assign {} {} {}, x = {}, y = {}", __LINE__, index[top[x]], index[x] + 1, cur_val, x, y) << std::endl;
                sgt.assignRange(index[top[x]], index[x] + 1, cur_val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
            }
            debug  std::cout << std::format("{} assign {} {} {}, x = {}, y = {}", __LINE__, index[y], index[x] + 1, cur_val, x, y) << std::endl;
            sgt.assignRange(index[y], index[x] + 1, cur_val);
        }
        // 操作 2
        // 查询 x->y 路径上的重边数量
        auto queryPath(int x, int y) -> int {
            Node x_res;  // x -> lca
            Node y_res;  // y -> lca
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                    std::swap(x_res, y_res);
                }
                x_res |= ~sgt.queryRange(index[top[x]], index[x] + 1);
                debug  std::cout << std::format("{} query {} {}, x_res = {}, x = {}, y = {}", __LINE__, index[top[x]], index[x] + 1, static_cast<std::string>(x_res), x, y) << std::endl;
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
                std::swap(x_res, y_res);
            }
            x_res |= ~sgt.queryRange(index[y], index[x] + 1);  // [x, lca]
            debug  std::cout << std::format("{} query {} {}, x_res = {}, x = {}, y = {}", __LINE__, index[y], index[x] + 1, static_cast<std::string>(x_res), x, y) << std::endl;
            auto y_res_rev = ~y_res;  // (lca, y]
            auto res = x_res | y_res_rev;  // [x, y]
            return res.count;
        }
        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            for (auto _: range(N-1)) {
                int x, y;  st >> x >> y;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            tr.dfs1(1, 0);
            tr.dfs2(1, 1);
            debug {
                std::cout << "order: ";
                for (auto x: tr.order)  std::cout << x << " ";
                std::cout << std::endl;
            }
            // 初始设成互不相同，全是轻边
            for (auto i: range(N)) {
                tr.sgt.assignRange(i, i + 1, i);
                tr.val = N;
            }
            return st;
        }
    };
    void solve() {
        int N, M;  std::cin >> N >> M;
        Tree tree{N};  std::cin >> tree;
        for (auto _: range(M)) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                tree.updatePath(x, y);
            } else {
                std::cout << tree.queryPath(x, y) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int T;  std::cin >> T;
    while (T --> 0) {
        Solution_9288857495666426::solve();
    }
    return 0;
}
