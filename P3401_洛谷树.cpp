/**
 * @link https://www.luogu.com.cn/problem/P3401
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1528516392980455 {
    constexpr const int MaxDigits = 10;

    // 统计每一位中 1 的个数
    struct BitCounter {
        int len;  // 区间长度
        std::array<int, MaxDigits> digits;  // 每一位
        BitCounter(int len = 0): len(len) { digits.fill(0); }
        // 后面拼上一个序列
        auto operator+= (const BitCounter &other) -> BitCounter & {
            if (len == 0)  return *this = other;
            if (other.len == 0)  return *this;
            len += other.len;
            for (auto i = 0; i < MaxDigits; i++) {
                digits[i] += other.digits[i];
            }
            return *this;
        }
        auto operator+ (const BitCounter &other) -> BitCounter {
            auto res = *this;
            res += other;
            return res;
        }
        // 按位反转序列
        auto operator~ () -> BitCounter {
            auto res = *this;
            for (auto i = 0; i < MaxDigits; i++) {
                res.digits[i] = len - res.digits[i];
            }
            return res;
        }
        auto operator^= (int other) -> BitCounter & {
            for (auto i = 0; i < MaxDigits; i++) {
                if (other & (1U << i)) {
                    digits[i] = len - digits[i];
                }
            }
            return *this;
        }
        operator std::string () {
            std::stringstream s;
            s << "{";
            for (auto i = 0; i < MaxDigits; i++) {
                s << i << ": " << digits[i] << ", ";
            }
            s << "len = " << len;
            s << "}";
            return s.str();
        }
    };
    // 点权为上级边权的异或和
    // 维护区间内每一位上 1 的数量
    class SegTree {
        struct Node {
            int begin, end;
            BitCounter val{1};
            int xorTag = 0;
        };
        std::vector<Node> tr;

        static constexpr int lson(int p) { return p << 1; }
        static constexpr int rson(int p) { return p << 1 | 1; }

        auto xorNode(int p, int val) -> void {
            tr[p].val ^= val;
            tr[p].xorTag ^= val;
        }
        auto pushUp(int p) -> void {
            tr[p].val = tr[lson(p)].val + tr[rson(p)].val;
        }
        auto pushDown(int p) -> void {
            if (tr[p].xorTag != 0) {
                for (auto s: {lson(p), rson(p)}) {
                    xorNode(s, tr[p].xorTag);
                }
                tr[p].xorTag = 0;
            }
        }
        auto build(int begin, int end, int p = 1) -> void {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
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
        auto queryRange(int begin, int end, int p = 1) -> BitCounter {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].val;
            }
            pushDown(p);
            BitCounter res;
            if (tr[lson(p)].end > begin)  res += queryRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res += queryRange(begin, end, rson(p));
            return res;
        }
        // 区间异或
        auto xorRange(int begin, int end, int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                xorNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  xorRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  xorRange(begin, end, val, rson(p));
            pushUp(p);
        }
    };
    class Tree {
        std::vector<std::vector<int>> graph;
        std::vector<int> depth, size, fa, son, top, order, index;
        int N;
        SegTree sgt;
        std::vector<int> fa_val;

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
        Tree(int N): graph(N+1), depth(N+1), size(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), N(N), sgt(N), fa_val(N+1) {}

        auto queryPath(int x, int y) -> ll {
            BitCounter res;  // 按位计数器
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                debug  std::cout << std::format("{} queryRange {} {} -> {}", __LINE__, index[top[x]], index[x] + 1, static_cast<std::string>(sgt.queryRange(index[top[x]], index[x] + 1))) << std::endl;
                res += sgt.queryRange(index[top[x]], index[x] + 1);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
            }
            debug  std::cout << std::format("{} queryRange {} {} -> {}", __LINE__, index[y], index[x] + 1, static_cast<std::string>(sgt.queryRange(index[y], index[x] + 1))) << std::endl;
            res += sgt.queryRange(index[y], index[x] + 1);
            debug  std::cout << std::format("res {}", static_cast<std::string>(res)) << std::endl;
            // 统计答案
            auto ans = 0LL;
            for (auto i = 0; i < MaxDigits; i++) {
                auto cur = static_cast<ll>(res.digits[i]) * (res.len - res.digits[i]) << i;
                ans += cur;
            }
            return ans;
        }
        auto assignEdge(int x, int y, int val) -> void {
            if (depth[x] < depth[y])  std::swap(x, y);
            auto cur = fa_val[x];
            auto del = val ^ cur;
            fa_val[x] = val;
            // 子树内均需要加上这个点
            sgt.xorRange(index[x], index[x] + size[x], del);
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
                for (auto x: tr.order) {
                    std::cout << x << " ";
                }
                std::cout << endl;
            }
            std::vector<int> init(N+1);
            for (auto [x, y, val]: edges) {
                if (tr.depth[x] < tr.depth[y])  std::swap(x, y);
                init[x] = val;
                tr.fa_val[x] = val;
            }
            for (auto i = 1; i < N + 1; i++) {
                debug  std::cout << std::format("{} xorRange {} {} {}", __LINE__, tr.index[i], tr.index[i] + tr.size[i], init[i]) << std::endl;
                tr.sgt.xorRange(tr.index[i], tr.index[i] + tr.size[i], init[i]);
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
    Solution_1528516392980455::solve();
    return 0;
}
