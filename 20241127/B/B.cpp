#include <bits/stdc++.h>
#define FILE_NAME "B"
#if false
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#endif  // if false
using ll = long long;
using ull = unsigned long long;

namespace Solution {

    std::vector<std::vector<int>> graph;

    std::vector<char> vis;
    auto dfs(int p, int prev, int depth, const int limit) -> void {
        if (depth > limit)  return;
        vis.at(p) = true;
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs(dest, p, depth+1, limit);
        }
    }

    void solve(int N) {
        graph.resize(N+1);
        for (auto i = 0; i < N-1; i++) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        int Q;  std::cin >> Q;
        for (auto _ = 0; _ < Q; _++) {
            vis.clear(), vis.resize(N+1);
            int M;  std::cin >> M;
            std::vector<std::pair<int, int>> nodes(M);
            for (auto &[x, y]: nodes)  std::cin >> x >> y;
            for (auto [origin, depth]: nodes) {
                dfs(origin, 0, 0, depth);
            }

            auto cnt = 0;
            for (auto i = 1; i < N+1; i++) {
                if (vis.at(i)) {
                    cnt++;
                }
            }

            std::cout << cnt << std::endl;
        }
    }
}

namespace Chain {
    std::vector<std::vector<int>> graph;
    std::vector<int> depth;

    struct SegTree {
        struct Node {
            int l, r;
            int val, tag = -1;
        };

        std::vector<Node> tr;

#define ls (p << 1)
#define rs (p << 1 | 1)
#define len(x) (tr[x].r - tr[x].l + 1)

        void push_down(int p) {
            if (tr[p].tag != -1) {
                tr[ls].tag = tr[rs].tag = tr[p].tag;
                tr[ls].val = tr[p].tag * len(ls);
                tr[rs].val = tr[p].tag * len(rs);
                tr[p].tag = -1;
            }
        }

        void push_up(int p) {
            tr[p].val = tr[ls].val + tr[rs].val;
        }

        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r;
            if (l == r)  return;

            auto mid = (l + r) >> 1;
            build(ls, l, mid);
            build(rs, mid+1, r);
            push_up(p);
        }

        void update(int p, int l, int r, int val) {
            // std::cout << "update " << p << " " << l << " " << r << " " << val << std::endl;
            if (tr[p].l >= l and tr[p].r <= r) {
                tr[p].tag = val;
                tr[p].val = val * len(p);
                return;
            }
            push_down(p);
            if (tr[ls].r >= l)  update(ls, l, r, val);
            if (tr[rs].l <= r)  update(rs, l, r, val);
            push_up(p);
        }

        int query(int p, int l, int r) {
            // std::cout << "query " << p << " " << l << " " << r << std::endl;
            if (tr[p].l >= l and tr[p].r <= r) {
                return tr[p].val;
            }
            push_down(p);
            auto res = 0;
            if (tr[ls].r >= l)  res += query(ls, l, r);
            if (tr[rs].l <= r)  res += query(rs, l, r);
            // std::cout << "returns " << res << std::endl;
            return res;
        }

#undef len
#undef ls
#undef rs
        SegTree(int N): tr((N+1)<<2) {
            build(1, 0, N);
        }
    };

    std::vector<int> order;
    void dfs(int p, int prev) {
        order.push_back(p);
        depth.at(p) = depth.at(prev) + 1;
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs(dest, p);
        }
    }

    void solve(int N) {
        graph.resize(N+1);
        for (auto i = 0; i < N-1; i++) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        depth.resize(N+1);
        dfs(1, 0);

        auto root = std::max_element(depth.begin(), depth.end()) - depth.begin();  // 一个端点
        // for (auto i: depth | std::views::drop(1))  std::cout << i << ' ';
        // std::cout << std::endl;
        order.clear(); std::fill(depth.begin(), depth.end(), 0); dfs(root, 0);

        std::vector<int> buc(N+1);
        for (auto i = 0; i < (int)order.size(); i++) {
            buc.at(order.at(i)) = i;
        }

        SegTree seg(N);

        int Q;  std::cin >> Q;
        while (Q --> 0) {
            int cnt, pnt, dis;
            seg.update(1, 0, (int)order.size()-1, 0);
            
            std::cin >> cnt;
            while (cnt --> 0) {
                std::cin >> pnt >> dis;
                auto index = buc.at(pnt);
                // std::cout << "index of " << pnt << " is " << index << std::endl;
                auto left = std::max(0, index - dis);
                auto right = std::min((int)order.size()-1, index + dis);
                seg.update(1, left, right, 1);
            }

            std::cout << seg.query(1, 0, (int)order.size()-1) << std::endl;
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int N;
    std::cin >> N;
    if (N <= 1000)  Solution::solve(N);
    else {
        Chain::solve(N);
    }

    return 0;
}