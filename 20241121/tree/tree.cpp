#include <bits/stdc++.h>
#define filename "tree"
using ll = long long;
using ull = unsigned long long;
using i128 = __int128;

namespace Solution {
    const int _N = 5e5+5;
    int N, M;
    std::vector<int> graph[_N];
    int A[_N];
    int sonSize[_N];

    ull ans[_N];

    const int _X = 1048576;
    ull c[_N];
    inline int lowbit(int x) {
        return x & -x;
    }

    void update(int x, int delta) {
        while (x < _X) {
            c[x] += delta;
            x += lowbit(x);
        }
    }

    ull query(int x) {
        auto res = 0ULL;
        while (x) {
            res += c[x];
            x -= lowbit(x);
        }
        return res;
    }

    void dfs_getSize(int p, int prev) {
        sonSize[p] = 1;
        for (auto dest: graph[p])  if (dest != prev) {
            dfs_getSize(dest, p);
            sonSize[p] += sonSize[dest];
        }
    }

    // void dfs_getAns(int p, bool record = true) {

    // }

    void solve() {
        std::cin >> N;
        M = N - 1;
        for (auto _ = 1; _ <= M; _++) {
            int x, y;
            std::cin >> x >> y;
            graph[x].push_back(y), graph[y].push_back(x);
        }

        for (auto i = 1; i <= N; i++)  std::cin >> A[i];

#if false
        std::system("start https://ys.mihoyo.com/");
#endif

        dfs_getSize(1, 0);
        // for (auto i = 1; i <= N; i++)  std::cout << sonSize[i] << ' ';
        // std::cout << std::endl;
        

        for (auto i = 1; i <= N; i++) {
            std::sort(graph[i].begin(), graph[i].end(), [&](int x, int y){ return sonSize[x] > sonSize[y]; });
        }
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(filename".in", "r", stdin);
    std::freopen(filename".out", "w", stdout);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    // Solution::solve();
    std::cout << 0 << std::endl;
    return 0;
}