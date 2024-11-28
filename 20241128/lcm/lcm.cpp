#include <bits/stdc++.h>

#if false
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#endif

#define filename "lcm"
#define debug if (debug_mode)
const bool debug_mode = false;
using ll = long long;
using ull = unsigned long long;

const int inf = 0x3f3f3f3f;
const long long infLL = 0x3f3f3f3f3f3f3f3fLL;

template <typename T>  inline T &chkMin(T &base, const T &cmp) {  return (base = std::min(base, cmp));  }
template <typename T>  inline T &chkMax(T &base, const T &cmp) {  return (base = std::max(base, cmp));  }

namespace Solution {

    std::vector<std::vector<int>> graph;

    std::vector<int> order;
    std::vector<int> first, size;

    int lcm(int x, int y) {
        return x / std::__gcd(x, y) * y;
    }

    void dfs(int p, int prev) {
        first.at(p) = order.size(), order.push_back(p);
        size.at(p) = 1;
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs(dest, p);
            size.at(p) += size.at(dest);
        }
    }

    void solve() {
        int N;  std::cin >> N;
        graph.resize(N+1);
        for (auto i = 0; i < N-1; i++) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y), graph.at(y).push_back(x);
        }

        first.resize(N+1), size.resize(N+1);
        dfs(1, 0);
        // for (auto i: size)  std::cout << i << ' ';
        // std::cout << std::endl;

        int Q;  std::cin >> Q;
        for (auto i = 0; i < Q; i++) {
            int r, x;  std::cin >> r >> x;
            auto begin = first.at(r), end = begin + size.at(r);
            auto cnt = 0;
            for (auto i = begin; i != end; i++) {
                for (auto j = begin; j != end; j++) {
                    if (lcm(order.at(i), order.at(j)) <= x)  cnt++;
                }
            }

            std::cout << cnt << ' ';
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(filename ".in", "r", stdin);
    std::freopen(filename ".out", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
    
    return 0;
}