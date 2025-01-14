/**
 * @link https://www.luogu.com.cn/problem/P3976
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1785912187491928 {
    // 区间最大值、最小值
    // 从左到右、从右到左最大收益
    struct Node {
        int begin = 0, end = 0;
        int min = +inf, max = -inf;
        int max_profit = -inf, max_rev_profit = -inf;
        int add_tag = 0;

        auto empty() const -> bool {
            return begin == end;
        }
        // 合并两个节点
        auto operator| (Node other) const -> Node {
            assert(not empty() or not other.empty());
            if (empty())  return other;
            if (other.empty())  return *this;
            // assert(add_tag == 0), assert(other.add_tag == 0);
            Node res {begin, other.end};
            res.min = std::min(min, other.min);
            res.max = std::max(max, other.max);
            res.max_profit = std::max(max_profit, other.max_profit);  // 左右各自的收益
            chkMax(res.max_profit, other.max - min);  // 左侧走到右侧
            res.max_rev_profit = std::max(max_rev_profit, other.max_rev_profit);
            chkMax(res.max_rev_profit, max - other.min);
            return res;
        }
        auto operator|= (Node other) -> Node & {
            *this = *this | other;
            return *this;
        }
        auto operator+= (int other) -> Node & {
            min += other, max += other;
            // 所有值同时增加，不影响收益
            add_tag += other;
            return *this;
        }
        auto operator~ () const -> Node {
            return {begin, end, min, max, max_rev_profit, max_profit, add_tag};
        }
        operator std::string () {
            return std::format("({}..{}, min{}, max{}, ->{}, <-{}, tag{})", begin, end, min, max, max_profit, max_rev_profit, add_tag);
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
            if (tr[p].add_tag != 0) {
                for (auto s: {lson(p), rson(p)}) {
                    tr[s] += tr[p].add_tag;
                }
                tr[p].add_tag = 0;
            }
        }
        auto build(int begin, int end, int p = 1) -> void {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].min = tr[p].max = tr[p].max_profit = tr[p].max_rev_profit = 0;
                return;
            }
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
        auto addRange(int begin, int end, int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p] += val;
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  addRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  addRange(begin, end, val, rson(p));
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
            top[p] = tp;
            index[p] = order.size(), order.push_back(p);
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
            Node base_x;  // x -> lca
            Node base_y;  // y -> lca

            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    // y 向上跳
                    base_y |= ~sgt.queryRange(index[top[y]], index[y] + 1);
                    y = fa[top[y]];
                } else {
                    // x 向上跳
                    base_x |= ~sgt.queryRange(index[top[x]], index[x] + 1);
                    x = fa[top[x]];
                }
                debug  std::cout << std::format("base_x = {}, base_y = {}", static_cast<std::string>(base_x), static_cast<std::string>(base_y)) << std::endl;
            }
            if (depth[x] < depth[y]) {
                base_y |= ~sgt.queryRange(index[x], index[y] + 1);
            } else {
                base_x |= ~sgt.queryRange(index[y], index[x] + 1);
            }
            debug  std::cout << std::format("base_x = {}, base_y = {}", static_cast<std::string>(base_x), static_cast<std::string>(base_y)) << std::endl;
            auto res = base_x | ~base_y;
            debug  std::cout << "res: " << static_cast<std::string>(res) << std::endl;
            return res;
        }
        auto addPath(int x, int y, int val) -> void {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                debug  std::cout << std::format("{} aRange({}, {}, {})", __LINE__, index[top[x]], index[x] + 1, val) << std::endl;
                sgt.addRange(index[top[x]], index[x] + 1, val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            debug  std::cout << std::format("{} aRange({}, {}, {})", __LINE__, index[y], index[x] + 1, val) << std::endl;
            sgt.addRange(index[y], index[x] + 1, val);
        }
        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            std::vector<int> init(N+1);
            for (auto &x: init | views::drop(1))  st >> x;
            for (auto _: range(N-1)) {
                int x, y;  st >> x >> y;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            
            tr.dfs1(1, 0), tr.dfs2(1, 1);
            debug {
                std::cout << "order: ";
                for (auto x: tr.order) {
                    std::cout << x << " ";
                }
                std::cout << std::endl;
            }
            for (auto i: range(N)) {
                auto cur = init[tr.order[i]];
                tr.sgt.addRange(i, i+1, cur);
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
        for (auto _: range(Q)) {
            int x, y, z;  std::cin >> x >> y >> z;
            std::cout << tree.queryPath(x, y).max_profit << std::endl;
            tree.addPath(x, y, z);
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1785912187491928::solve();
    return 0;
}
