/**
 * @link https://www.luogu.com.cn/problem/P3401
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1983728601745017 {
    constexpr const int MaxBits = 10;
    class SegTree {
        struct Node {
            int begin, end;
            int count;
            bool rev_tag;
        };
        std::vector<Node> tr;
        static constexpr auto lson(int x) -> int { return x << 1; }
        static constexpr auto rson(int x) -> int { return x << 1 | 1; }
        auto nodeLength(int p) const -> int { return tr[p].end - tr[p].begin; }
        auto revNode(int p) -> void {
            tr[p].count = nodeLength(p) - tr[p].count;
            tr[p].rev_tag = not tr[p].rev_tag;
        };
        auto pushUp(int p) -> void {
            tr[p].count = tr[lson(p)].count + tr[rson(p)].count;
        }
        auto pushDown(int p) -> void {
            if (tr[p].rev_tag) {
                for (auto s: {lson(p), rson(p)}) {
                    revNode(s);
                }
                tr[p].rev_tag = false;
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
        SegTree() = default;
        SegTree(int N): tr(N << 2) { build(0, N, 1); }
        auto countRange(int begin, int end, int p = 1) -> int {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].count;
            }
            pushDown(p);
            auto res = 0;
            if (tr[lson(p)].end > begin)  res += countRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res += countRange(begin, end, rson(p));
            return res;
        }
        auto revRange(int begin, int end, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                revNode(p);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  revRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  revRange(begin, end, rson(p));
            pushUp(p);
        }
    };
    struct BitsCount {
        int len;
        std::array<int, MaxBits> data;
        std::array<bool, MaxBits> flags;
        BitsCount(int len = 0): len(len) {
            data.fill(0);
            flags.fill(false);
        }
        BitsCount(int len, std::array<int, MaxBits> data): len(len), data(data) {}
        auto setFlag(int idx, bool flag = true) -> void { flags[idx] = flag; }
        auto operator+ (BitsCount other) const -> BitsCount {
            auto res_data = data;
            for (auto i = 0; i < MaxBits; i++)  res_data[i] += other.data[i];
            BitsCount res {len + other.len, res_data};
            res.flags = other.flags;
            return res;
        }
        auto operator+= (BitsCount other) -> BitsCount & {
            *this = *this + other;
            return *this;
        }
        auto operator[] (size_t idx) -> int & {
            return data[idx];
        }

        // 对答案的贡献
        auto statistic() -> ll {
            auto ans = 0LL;
            for (auto i = 0; i < MaxBits; i++) {
                ans += (static_cast<ll>(data[i]) * (len - data[i])) << i;
            }
            return ans;
        }
    };
    class BitsSegTrees {
        std::array<SegTree, MaxBits> sgts;
        int N;
    public:
        BitsSegTrees(int N): N(N) {
            sgts.fill(SegTree{N});
        }
        auto countRange(int begin, int end) -> BitsCount {
            BitsCount res {end - begin};
            for (auto i = 0; i < MaxBits; i++) {
                auto prev = begin == 0? 0: sgts[i].countRange(begin - 1, begin);
                assert(prev == 0 or prev == 1);
                res[i] = sgts[i].countRange(begin, end);
                auto flag = sgts[i].countRange(end - 1, end);
                if (prev)  res[i] = (end - begin) - res[i], flag = not flag;
                if (flag)  res.setFlag(i);
            }
            return res;
        }
        auto xorAt(int pos, int val) -> void {
            for (auto i = 0; i < MaxBits; i++) {
                if (val & (1U << i)) {
                    sgts[i].revRange(pos, pos + 1);
                }
            }
        }
    };
    class Tree {
        std::vector<std::vector<int>> graph;
        std::vector<int> size, depth, fa, son, top, order, index;
        std::vector<int> fa_val;  // 和父节点之间连边的权值
        BitsSegTrees sgts;
        int N;

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
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): graph(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), fa_val(N+1), sgts(N), N(N) {}
        
        auto assignEdge(int x, int y, int val) -> void {
            if (depth[x] < depth[y])  std::swap(x, y);
            auto cur = fa_val[x];  // 当前权值
            auto delta = val ^ cur;  // 增量（异或）
            sgts.xorAt(x, delta);
        }

        auto queryPath(int x, int y) -> ll {
            BitsCount res;
            while (top[x] != top[y]) {
                if (depth[x] < depth[y])  std::swap(x, y);
                debug  std::cout << std::format("{} countRange({}, {})", __LINE__, index[top[x]], index[x]+1) << std::endl;
                res += sgts.countRange(index[top[x]], index[x] + 1);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            debug  std::cout << std::format("{} countRange({}, {})", __LINE__, index[y] + 1, index[x] + 1) << std::endl;
            res += sgts.countRange(index[y] + 1, index[x] + 1);
            return res.statistic();  // 统计答案
        }

        friend auto operator>> (std::istream &s, Tree &tr) -> std::istream & {
            auto N = tr.N;
            std::vector<std::tuple<int, int, int>> edges;
            for (auto _ = 0; _ < N - 1; _++) {
                int x, y, val;  s >> x >> y >> val;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
                edges.push_back({x, y, val});
            }
            tr.dfs1(1, 0);
            tr.dfs2(1, 1);

            debug {
                std::cout << "order: ";
                for (auto x: tr.order)  std::cout << x << ' ';
                std::cout << std::endl;
            }

            std::vector<int> init(N+1);  // 点 i 的初始值
            for (auto [x, y, val]: edges) {
                if (tr.depth[x] < tr.depth[y])  std::swap(x, y);
                tr.fa_val[x] = val;
                init[x] = val;  // 边权 -> 较深点的点权
            }
            std::vector<int> sgt_init(N);  // 树剖序下标 -> 点权的初始值
            for (auto i = 0; i < N; i++) {
                sgt_init[i] = init[tr.order[i]];
            }
            for (auto i = 0; i < N; i++) {
                tr.sgts.xorAt(i, sgt_init[i]);
            }
            return s;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        Tree tree{N};  std::cin >> tree;
        for (auto _ = 0; _ < Q; _++) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                std::cout << tree.queryPath(x, y) << endl;
            } else {
                int z;  std::cin >> z;
                tree.assignEdge(x, y, z);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1983728601745017::solve();
    return 0;
}
