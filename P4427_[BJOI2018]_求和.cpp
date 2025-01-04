/**
 * @link https://www.luogu.com.cn/problem/P4427
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1424075383093656 {
    constexpr const int mod = 998'244'353;
    constexpr const int _K = 50;
    ll qpow(ll a, int b, const int mod) {
        auto res = 1LL;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    class SegTree {
    public:
        std::vector<int> ps;
        SegTree(int) {}
        SegTree(SegTree &&other): ps(std::move(other.ps)) {}
        template <typename T>
        void build(int, int, const T &init) {
            ps.resize(init.size());
            std::partial_sum(init.begin(), init.end(), ps.begin(), lam(x, y, (x + y) % mod));
            ps.insert(ps.begin(), 0);
        }
        int sum(int begin, int end) const {
            return (ps[end] - ps[begin] + mod) % mod;
        }
    };
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> fa, depth, size, son, top, order, index;
        std::vector<SegTree> sgts;
    public:
        template <typename T>
        Tree(int N, const T &edges): forward(N+1), fa(N+1), depth(N+1), size(N+1), son(N+1), top(N+1), index(N+1) {
            for (auto [x, y]: edges)  addEdge(x, y);
            depth[0] = -1;
            dfs1(1, 0), dfs2(1, 1);
            for (auto k: range(0, _K + 1)) {
                std::vector<int> tmp(N);
                for (auto i: range(tmp.size()))  tmp[i] = qpow(depth.at(order.at(i)), k, mod);
                SegTree sgt(N);
                sgt.build(0, tmp.size(), tmp);
                sgts.push_back(std::move(sgt));
            }
        }
        void addEdge(int x, int y) {
            forward.at(x).push_back(y);
            forward.at(y).push_back(x);
        }
        void dfs1(int p, int prev) {
            depth[p] = depth[prev] + 1;
            fa[p] = prev;
            size[p] = 1;
            for (auto x: forward[p])  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[son[p]] < size[x])  son[p] = x;
            }
        }
        void dfs2(int p, int p_top) {
            top[p] = p_top, index[p] = order.size(), order.push_back(p);
            if (son[p])  dfs2(son[p], p_top);
            for (auto x: forward[p])  if (x != fa[p] and x != son[p]) {
                dfs2(x, x);
            }
        }
        int sumPath(int x, int y, int k) {
            auto res = 0;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
                res = (res + sgts[k].sum(index[top[x]], index[x] + 1)) % mod;
                x = fa[top[x]];
            }
            if (depth[x] > depth[y])  std::swap(x, y);
            res = (res + sgts[k].sum(index[x], index[y] + 1)) % mod;
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N;  std::cin >> N;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;
        
        Tree tree(N, edges);
        int M;  std::cin >> M;
        for (auto _: range(M)) {
            int x, y, k;
            std::cin >> x >> y >> k;
            auto ans = tree.sumPath(x, y, k);
            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1424075383093656::solve();
    return 0;
}
