/**
 * @link https://www.luogu.com.cn/problem/P5127
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 考虑一个连续段上的情况：
 * 按位考虑，每一位上有若干个 1，当取了奇数个 1 的时候会对这一位产生贡献。
 * 1 的个数为奇偶的情况均分，所以为 2**(N-1) 种情况产生贡献。
 * 特别地，如果某一位都是 0，一定不会有贡献。
 * 只需所有数取或，最后乘以 2**(N-1)。
 */
namespace Solution_1245959757901944 {
    const int mod = 1'000'000'007;
    constexpr auto qpow(ll a, ll b, const int mod) -> ll {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    /**
     * 考虑使用线段树维护区间或。
     * 要求支持区间异或操作。
     * 仍然按位考虑，区间异或即为若干位上反转。
     * 某一个位上可能是全 1、全 0 或者二者兼有，仅全 0 时 Or 值为 0
     * 考虑用两个整数表示，a 表示某一位上是否全是 1，b 表示全是 0。
     * 不全为 0 即为区间或。
     * 合并：两个区间上的 a 和 b 求按位与。
     * 异或：一些位上的全 1 会变成全 0，即 a' 包含翻转的 b 和未翻转的 a
     * a' = a & ~x | b & x
     * b' = b & ~x | a & x
     */
    class SegTree {
        struct Node {
            int begin = 0, end = 0;
            unsigned int all_one = 0U, all_zero = ~0U, xor_tag = 0U;

            auto size() const -> int { return end - begin; }
            auto empty() const -> bool { return size() == 0; }
            // 合并两个区间
            auto operator| (Node other) const -> Node {
                Node res{begin, other.end};
                res.all_one = all_one & other.all_one;  // 只有两侧均全为 1，大区间才全为 1
                res.all_zero = all_zero & other.all_zero;
                return res;
            }
            auto operator|= (Node other) -> Node & {
                return *this = *this | other;
            }
            // 对区间取异或
            auto operator^ (int x) const -> Node {
                Node res{begin, end};
                res.all_one = (all_one & ~x) | (all_zero & x);
                res.all_zero = (all_zero & ~x) | (all_one & x);
                res.xor_tag = xor_tag ^ x;
                return res;
            }
            auto operator^= (int x) -> Node & {
                return *this = *this ^ x;
            }
        };
        std::vector<Node> tr;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }
        auto pushUp(int p) -> void {
            tr[p] = tr[lson(p)] | tr[rson(p)];
        }
        auto pushDown(int p) -> void {
            if (tr[p].xor_tag != 0U) {
                for (auto x: {lson(p), rson(p)}) {
                    tr[x] ^= tr[p].xor_tag;
                }
                tr[p].xor_tag = 0U;
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
        auto queryOrRange(int begin, int end, int p = 1) -> unsigned int {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return ~tr[p].all_zero;
            }
            auto res = 0U;
            pushDown(p);
            if (tr[lson(p)].end > begin)  res |= queryOrRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res |= queryOrRange(begin, end, rson(p));
            return res;
        }
        auto xorRange(int begin, int end, unsigned int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p] ^= val;
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  xorRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  xorRange(begin, end, val, rson(p));
            pushUp(p);
        }
    };
    class Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> depth, size, fa, son, top, order, index;
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
            if (son[p] != 0)  dfs2(son[p], tp);
            for (auto x: graph.at(p)) {
                if (x != fa[p] and x != son[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): N(N), graph(N+1), depth(N+1), size(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), sgt(N) {}

        auto queryPath(int x, int y) -> int {
            unsigned int or_sum = 0U;
            int len = 0;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                or_sum |= sgt.queryOrRange(index[top[x]], index[x] + 1);
                len += index[x] + 1 - index[top[x]];
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
            }
            or_sum |= sgt.queryOrRange(index[y], index[x] + 1);
            len += index[x] + 1 - index[y];
            auto ans = static_cast<ll>(or_sum) * qpow(2, len - 1, mod) % mod;
            return ans;
        }
        auto xorPath(int x, int y, unsigned int val) -> void {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                sgt.xorRange(index[top[x]], index[x] + 1, val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
            }
            sgt.xorRange(index[y], index[x] + 1, val);
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
            for (auto i: range(1, N+1)) {
                int x;  st >> x;
                tr.sgt.xorRange(tr.index[i], tr.index[i]+1, x);
            }
            return st;
        }
    };
    void solve() { 
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        Tree tree{N};  std::cin >> tree;
        for (auto _: range(M)) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                std::cout << tree.queryPath(x, y) << endl;
            } else {
                int z;  std::cin >> z;
                tree.xorPath(x, y, z);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1245959757901944::solve();
    return 0;
}
