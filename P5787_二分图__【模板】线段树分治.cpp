/**
 * @link https://www.luogu.com.cn/problem/P5787
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1103724470616732 {
    const int _N = 1e5+5;
    int N, M, K;

    struct Modification {
        int from, to;  // 从 from 到 to 连边一次
    };
    std::deque<Modification> st;

    std::vector<int> F, size;
    int find(int x) {
        if (F[x] == x)  return x;
        else  return find(F[x]);
    }

    void merge(int x, int y) {
        auto a = find(x), b = find(y);
        if (a == b)  return;
        if (size[a] < size[b])  return merge(y, x);
        st.push_back({x, y});
        F[b] = a;
        size[a] += size[b];
    }

    struct Edge { int x, y; };
    std::vector<Edge> edges;

    template <size_t SZ>
    struct SegTree {
        struct Node {
            int l, r;
            std::vector<int> edges;
        } tr[SZ];

#define ls (p << 1)
#define rs (p << 1 | 1)

        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r;
            if (l == r)  return;
            auto mid = (l + r) >> 1;
            build(ls, l, mid), build(rs, mid+1, r);
        }

        void insert(int p, int l, int r, int x) {
            if (tr[p].l >= l and tr[p].r <= r)  return tr[p].edges.push_back(x);
            if (tr[ls].r >= l)  insert(ls, l, r, x);
            if (tr[rs].l <= r)  insert(rs, l, r, x);
        }

        void dfs(int p) {
            auto l = tr[p].l, r = tr[p].r;
            // 记录加边前的深度
            auto origin = st.size();
            // 加边
            for (auto i: tr[p].edges) {
                auto [u, v] = edges[i];
                if (find(u) == find(v)) {
                    for (auto _: range(l, r+1))  std::cout << "No" << std::endl;  // 失效之前一直不合法
                    goto egg;
                }
                merge(u+N, v), merge(u, v+N);
            }
            // 继续向下
            if (l == r)  std::cout << "Yes" << std::endl;
            else  dfs(ls), dfs(rs);
        egg:
            while (st.size() > origin) {
                // 取消一次连边
                auto [super, sub] = st.back();  st.pop_back();
                size[super] -= size[sub];
                F[sub] = sub;
            }
        }

#undef ls
#undef rs
    };

    SegTree<1048576> seg;

    
    
    void solve() {
        std::cin >> N >> M >> K;
        seg.build(1, 1, K);
        F.resize(N*2+1), rgs::copy(range(2*N+1), F.begin());
        size.resize(N*2+1), rgs::fill(size, 1);

        for (auto _: range(M)) {
            int x, y, l, r;  std::cin >> x >> y >> l >> r;
            seg.insert(1, l, r-1, edges.size());
            edges.push_back({x, y});
        }

        seg.dfs(1);
    }
}

int main() {
    initDebug;
    Solution_1103724470616732::solve();
    return 0;
}