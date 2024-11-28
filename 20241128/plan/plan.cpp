#include <bits/stdc++.h>

#if false
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#endif

#define filename "plan"
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

    struct MaxItem {
        int value, from;
        bool operator< (MaxItem const &other) const noexcept { return value < other.value; }
        bool operator> (MaxItem const &other) const noexcept { return other < *this; }
        MaxItem operator+ (int other) const noexcept {
            return {value + other, from};
        }
    };

    struct TwoMax {
        MaxItem max = {-inf, 0}, sec = {-inf, 0};

        TwoMax &update(MaxItem const &other) {
            if (other > max)  sec = max, max = other;
            else if (other > sec) sec = other;
            return *this;
        }
    };

    std::vector<TwoMax> maxHalfChain;  // 最长的半链
    // 更新半链长度
    void dfs_halfChain(int p, int prev) {
        maxHalfChain.at(p).update({0, p});
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_halfChain(dest, p);
            maxHalfChain.at(p).update({maxHalfChain.at(dest).max.value + 1, dest});
        }
    }

    std::vector<int> ans;
    void dfs_exchangeRoot(int p, int prev, int up) {
        chkMax(ans.at(p), maxHalfChain.at(p).sec.value + maxHalfChain.at(p).max.value);
        chkMax(ans.at(p), up + maxHalfChain.at(p).max.value);
        for (auto dest: graph.at(p))  if (dest != prev) {
            if (dest == maxHalfChain.at(p).max.from) {
                dfs_exchangeRoot(dest, p, std::max(up, maxHalfChain.at(p).sec.value) + 1);
            } else {
                dfs_exchangeRoot(dest, p, std::max(up, maxHalfChain.at(p).max.value) + 1);
            }
        }
    }

    void solve() {
        int N, M;  std::cin >> N >> M;

        graph.resize(N+1);
        for (auto i = 0; i < M; i++) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y), graph.at(y).push_back(x);
        }

        maxHalfChain.resize(N+1), dfs_halfChain(1, 0);

        ans.resize(N+1), dfs_exchangeRoot(1, 0, 0);

        for (auto it = ans.begin()+1; it != ans.end(); it++) {
            auto i = *it;
            std::cout << i << ' ';
        }
        std::cout << std::endl;
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