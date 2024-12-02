#include <bits/stdc++.h>

#define FILE_NAME "query"
#define FILE_IO_ENABLED false

using ll = long long;
using ull = unsigned long long;

const int inf = 0x3f3f3f3f;
const long long infLL = 0x3f3f3f3f3f3f3f3fLL;

template <typename T>  T &chkMax(T &base, const T &cmp) { return (base = std::max(base, cmp)); }
template <typename T>  T &chkMin(T &base, const T &cmp) { return (base = std::min(base, cmp)); }

// #define at operator[]

namespace Solution {

    int N, ST;

    std::vector<std::vector<int>> graph;

    std::vector<int> depth;
    std::vector<std::vector<int>> F;

    int const _RECORD_LIMIT = 5000;
    int mem[_RECORD_LIMIT][_RECORD_LIMIT];

    int LCA(int x, int y) {
        if (x == -1)  return y;
        if (y == -1)  return x;
        if (x < _RECORD_LIMIT and y < _RECORD_LIMIT) {
            auto record = mem[x][y];
            if (record)  return record;
        }
        if (depth.at(x) < depth.at(y))  std::swap(x, y);
        auto dis = depth.at(x) - depth.at(y);

        for (auto i = 0; i < ST; i++) {
            if (dis & (1<<i))  x = F.at(x).at(i);
            if (x == y)  return x;
        }

        if (x == y)  return x;

        for (auto i = ST - 1; i --> 0; ) {
            if (F[x].at(i) != F[y].at(i)) {
                x = F[x][i], y = F[y][i];
            }
        }

        return F[x].at(0);
    }

    void dfs_init(int p, int prev) {
        depth.at(p) = depth.at(prev) + 1;

        F[p].at(0) = prev;
        for (auto i = 1; i < ST; i++)  F[p].at(i) = F[ F[p].at(i-1) ].at(i-1);

        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_init(dest, p);
        }
    }

    struct SegTree {
        struct Node {
            int l, r;
            int LCA;
        };

        std::vector<Node> tr;

        SegTree(int l, int r): tr((r-l+1) << 2) {
            build(1, l, r);
        }

#define ls (p << 1)
#define rs (p << 1 | 1)
        void push_up(int p) {
            tr[p].LCA = LCA(tr[ls].LCA, tr[rs].LCA);
        }

        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r;
            if (l == r)  return tr[p].LCA = l, void();

            auto mid = (l+r) >> 1;
            build(ls, l, mid), build(rs, mid+1, r);
            push_up(p);
        }

        int query(int p, int l, int r) {
            if (tr[p].l >= l and tr[p].r <= r)  return tr[p].LCA;
            auto res = -1;
            if (tr[ls].r >= l)  res = LCA(res, query(ls, l, r));
            if (tr[rs].l <= r)  res = LCA(res, query(rs, l, r));
            return res;
        }
#undef ls
#undef rs
    };

    void solve() {
        std::cin >> N;  ST = std::__lg(N+1)+1;
        graph.resize(N+1), depth.resize(N+1);
        F.resize(N+1, std::vector<int>(ST));

        for (auto i = 0; i < N-1; i++) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        };

        dfs_init(1, 0);

        // int x, y;  std::cin >> x >> y;
        
        SegTree seg(1, N);
        int Q;  std::cin >> Q;
        while (Q --> 0) {
            int L, R, K;  std::cin >> L >> R >> K;
            auto ans = -inf;
            for (auto l = L; l + K - 1 <= R; l++) {
                // Assert that we can get answer when r - l + 1 = K
                auto r = l + K - 1;
                auto lca = seg.query(1, l, r);
                chkMax(ans, depth.at(lca));
            }
            std::cout << ans << std::endl;
        }
    }
}

int main() {
#if FILE_IO_ENABLED
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();

    return 0;
}