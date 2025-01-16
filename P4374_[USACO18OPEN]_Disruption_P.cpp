/**
 * @link https://www.luogu.com.cn/problem/P4374
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 一个环上，可以去掉任意一个点，此时仍然可以保持连通。
 * 对于每一条新边，原图上连接其两端点的路径中所有边都可以被它代替。
 * 使用线段树维护最小替代项即可。
 */
namespace Solution_1338912477648913 {
    // 区间查询最小值
    // 区间更新最小值
    class SegTree {
        struct Node {
            int begin, end;
            int min = +inf;
            int tag = +inf;

            auto update(int val) -> void {
                chkMin(min, val);
                chkMin(tag, val);
            }
        };
        std::vector<Node> tr;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }
        auto pushUp(int p) -> void {
            tr[p].min = std::min(tr[lson(p)].min, tr[rson(p)].min);
        }
        auto pushDown(int p) -> void {
            if (tr[p].tag < +inf) {
                for (auto s: {lson(p), rson(p)}) {
                    tr[s].update(tr[p].tag);
                }
                tr[p].tag = +inf;
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
        auto minRange(int begin, int end, int p = 1) -> int {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].min;
            }
            auto res = +inf;
            pushDown(p);
            if (tr[lson(p)].end > begin)  chkMin(res, minRange(begin, end, lson(p)));
            if (tr[rson(p)].begin < end)  chkMin(res, minRange(begin, end, rson(p)));
            return res;
        }
        auto checkMinRange(int begin, int end, int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].update(val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  checkMinRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  checkMinRange(begin, end, val, rson(p));
            pushUp(p);
        }
    };
    class Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> size, depth, fa, son, top, order, index;
        std::vector<std::pair<int, int>> edges;
        SegTree sgt;

        auto dfs1(int p, int prev) -> void {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;
            for (auto x: graph.at(p)) {
                if (x != prev) {
                    dfs1(x, p);
                    size[p] += size[x];
                    if (size[son[p]] < size[x]) {
                        son[p] = x;
                    }
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
        Tree(int N): N(N), graph(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), sgt(N) {}

        // 更新整个路径上的所有边
        auto checkMinPath(int x, int y, int val) -> void {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                sgt.checkMinRange(index[top[x]], index[x] + 1, val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
            }
            sgt.checkMinRange(index[y] + 1, index[x] + 1, val);
        }

        // 查询一个边的最小替代项
        auto queryEdge(int idx) -> int {
            auto [x, y] = edges[idx - 1];
            if (depth[x] < depth[y])  std::swap(x, y);
            auto res = sgt.minRange(index[x], index[x] + 1);
            if (res < +inf)  return res;
            else  return -1;
        }

        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            for (auto _: range(N-1)) {
                int x, y;  std::cin >> x >> y;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
                tr.edges.push_back({x, y});
            }
            tr.dfs1(1, 0);
            tr.dfs2(1, 1);
            return st;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        Tree tree{N};  std::cin >> tree;
        for (auto _: range(M)) {
            int x, y, val;  std::cin >> x >> y >> val;
            tree.checkMinPath(x, y, val);
        }
        for (auto i: range(1, N)) {
            std::cout << tree.queryEdge(i) << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1338912477648913::solve();
    return 0;
}
