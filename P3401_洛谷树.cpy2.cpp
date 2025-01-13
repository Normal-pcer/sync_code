/**
 * @link https://www.luogu.com.cn/problem/P3401
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1528516392980454 {
    constexpr const int MaxDigits = 3;
    // 由若干个布尔值组成的序列的特征
    struct BoolSeq {
        int length = 0;  // 长度
        int popcount = 0;  // 1 的数量
        bool front = false, back = false;

        auto empty() const -> bool {
            return length == 0;
        }

        // 在后面拼接
        auto operator+= (BoolSeq other) -> BoolSeq & {
            if (empty())  return *this = other;
            if (other.empty())  return *this;
            length += other.length, popcount += other.popcount;
            back = other.back;
            return *this;
        }
        auto operator+ (BoolSeq other) const -> BoolSeq {
            BoolSeq res = *this;
            res += other;
            return res;
        }
        // 每一位取反
        auto operator~ () const -> BoolSeq {
            BoolSeq res = *this;
            res.popcount = res.length - res.popcount;
            res.front = not res.front;
            res.back = not res.back;
            return res;
        }
        auto operator^= (bool x) -> BoolSeq & {
            if (x)  *this = ~*this;
            return *this;
        }
        auto operator^ (bool x) -> BoolSeq {
            if (x)  return ~*this;
            else  return *this;
        }
        operator std::string () {
            return std::format("({}, {}, {}..{})", length, popcount, static_cast<int>(front), static_cast<int>(back));
        }
    };
    // 前缀异或和序列特征
    struct PrefixXorSumSeq {
        std::array<BoolSeq, MaxDigits> digits;  // 每一位
        // 后面拼上一个序列
        // 给定序列 A 和 B 的前缀和，返回 A+B 的前缀和
        auto operator+= (const PrefixXorSumSeq &other) -> PrefixXorSumSeq & {
            for (auto i = 0; i < MaxDigits; i++) {
                auto flag = digits[i].back;
                if (flag)  digits[i] += ~other.digits[i];
                else  digits[i] += other.digits[i];
            }
            return *this;
        }
        auto operator+ (const PrefixXorSumSeq &other) -> PrefixXorSumSeq {
            auto res = *this;
            res += other;
            return res;
        }
        // 按位反转序列
        auto operator~ () -> PrefixXorSumSeq {
            auto res = *this;
            for (auto i = 0; i < MaxDigits; i++) {
                res.digits[i] = ~res.digits[i];
            }
            return res;
        }
        auto operator^= (int other) -> PrefixXorSumSeq & {
            for (auto i = 0; i < MaxDigits; i++) {
                if (other & (1U << i)) {
                    digits[i] = ~digits[i];
                }
            }
            return *this;
        }
        operator std::string () {
            std::stringstream s;
            s << "{";
            for (auto i = 0; i < MaxDigits; i++) {
                s << i << ": " << static_cast<std::string>(digits[i]) << ", ";
            }
            s << "}";
            return s.str();
        }
    };
    // 维护每个区间内的前缀和序列特征
    class SegTree {
        struct Node {
            int begin, end;
            PrefixXorSumSeq val;  // [begin, end) 区间内的前缀异或和序列
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
                for (auto i = 0; i < MaxDigits; i++) {
                    tr[p].val.digits[i].length = 1;
                }
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
        // 查询区间的前缀异或和序列
        auto queryRange(int begin, int end, int p = 1) -> PrefixXorSumSeq {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].val;
            }
            pushDown(p);
            PrefixXorSumSeq res;
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
            // 获取一整个路径上的前缀和序列
            PrefixXorSumSeq base_x;  // lca -> x
            PrefixXorSumSeq base_y;  // lca -> y
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                    std::swap(base_x, base_y);
                }
                debug  std::cout << std::format("{} queryRange {} {} -> {}", __LINE__, index[top[x]], index[x] + 1, static_cast<std::string>(sgt.queryRange(index[top[x]], index[x] + 1))) << std::endl;
                base_x = sgt.queryRange(index[top[x]], index[x] + 1) + base_x;
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
                std::swap(base_x, base_y);
            }
            debug  std::cout << std::format("{} queryRange {} {} -> {}", __LINE__, index[y] + 1, index[x] + 1, static_cast<std::string>(sgt.queryRange(index[y] + 1, index[x] + 1))) << std::endl;
            base_x = sgt.queryRange(index[y] + 1, index[x] + 1) + base_x;
            auto rev_x = base_x;  // x -> lca
            for (auto i = 0; i < MaxDigits; i++)  std::swap(rev_x.digits[i].back, rev_x.digits[i].front);
            auto seq = rev_x + base_y;
            debug  std::cout << std::format("seq {}", static_cast<std::string>(seq)) << std::endl;
            // 统计答案
            auto ans = 0LL;
            for (auto i = 0; i < MaxDigits; i++) {
                auto cur = static_cast<ll>(seq.digits[i].popcount) * (seq.digits[i].length + 1 - seq.digits[i].popcount) << i;
                ans += cur;
            }
            return ans;
        }
        auto assignEdge(int x, int y, int val) -> void {
            if (depth[x] < depth[y])  std::swap(x, y);
            auto cur = fa_val[x];
            auto del = val ^ cur;
            sgt.xorRange(index[x], index[x] + 1, del);
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
            }
            std::vector<int> sgt_init(N);
            for (auto i = 0; i < N; i++) {
                sgt_init[i] = init[tr.order[i]];
            }
            for (auto i = 0; i < N; i++) {
                tr.sgt.xorRange(i, i+1, sgt_init[i]);
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
    Solution_1528516392980454::solve();
    return 0;
}
