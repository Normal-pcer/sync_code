/**
 * @link https://www.luogu.com.cn/problem/P5227
 */

#include "./lib"

#include "./libs/__vector.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9414518281694381 {

    const int _N = 1e5+5;
    int N, M, K;

    unstd::vector<std::pair<int, int>> edges;
    unstd::vector<std::vector<int>> sets;

    // 将 sub 连向了 super
    struct Modification {
        int super;
        int sub;
    };

    std::deque<Modification> st;  // 对并查集的记录

    unstd::vector<int> F;  // 父亲节点
    unstd::vector<int> size;  // 父亲节点下的子树大小
    int find(int x) {
        if (F.at(x) == x)  return x;
        else  return find(F[x]);
    }

    void merge(int x, int y) {
        auto super = find(x), sub = find(y);
        if (super == sub)  return st.push_back({0, 0});
        if (super < sub)  std::swap(super, sub);
        F[sub] = super, size[super] += size[sub];
        st.push_back({super, sub});
    }

    template <size_t SZ>
    struct SegTree {
        struct Node {
            int l, r;
            unstd::vector<int> edges;  // 可以使用的边
        } tr[SZ<<2];

#define ls (p << 1)
#define rs (p << 1 | 1)
        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r;
            if (l == r)  return;
            auto mid = (l + r) >> 1;
            build(ls, l, mid), build(rs, mid+1, r);
        }

        // 将一个边在生命周期中记为可以使用
        void update(int p, int l, int r, int append) {
            if (l > r)  return;
            if (tr[p].l >= l and tr[p].r <= r) {
                tr[p].edges.push_back(append);
                return;
            }
            if (tr[ls].r >= l)  update(ls, l, r, append);
            if (tr[rs].l <= r)  update(rs, l, r, append);
        }

        void dfs(int p) {
            auto l = tr[p].l, r = tr[p].r;
            auto origin = st.size();  // 记录原始栈大小
            // 加边
            for (auto e: tr[p].edges) {
                auto [x, y] = edges[e];
                merge(x, y); 
            }

            // 向下遍历
            if (l == r) {
                // 检查此时的连通性
                if (size[find(1)] == N) {
                    std::cout << "Connected" << std::endl;
                } else {
                    std::cout << "Disconnected" << std::endl;
                }
            } else {
                dfs(ls), dfs(rs);
            }

            // 删边
            while (st.size() > origin) {
                auto [super, sub] = st.back();  st.pop_back();
                if (super == 0 and 0 == sub)  continue;  // 跳过
                size[super] -= size[sub];
                F[sub] = sub;
            }
        }
#undef ls
#undef rs
    };
    SegTree<_N> seg;
    unstd::vector<unstd::vector<int>> disappear;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N >> M;
        edges.push_back({0, 0});
        for (auto _: range(M)) {
            int x, y;  std::cin >> x >> y;
            edges.push_back({x, y});
        }
        std::cin >> K;
        seg.build(1, 1, K);
        disappear.resize(M+1);
        for (auto time: range(1, K+1)) {
            int cnt;  std::cin >> cnt;
            sets.emplace_back(cnt);
            for (auto &i: sets[sets.size()-1])  std::cin >> i, disappear[i].push_back(time);
        }

        for (auto i: range(1, M+1)) {
            disappear[i].push_back(K+1);
            auto left = 0;  // 上一次中断
            for (auto t: disappear[i]) {
                seg.update(1, left+1, t-1, i);
                left = t;
            }
        }

        // 初始化并查集
        F.resize(N+1), rgs::copy(range(N+1), F.begin());
        size.resize(N+1), rgs::fill(size, 1);

        seg.dfs(1);
    }
}

int main() {
    initDebug;
    Solution_9414518281694381::solve();
    return 0;
}