#include <bits/stdc++.h>

#if false
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#endif

#define filename ""
#define debug if (debug_mode)
const bool debug_mode = false;
using ll = long long;
using ull = unsigned long long;

const int inf = 0x3f3f3f3f;
const long long infLL = 0x3f3f3f3f3f3f3f3fLL;

template <typename T>  inline T &chkMin(T &base, const T &cmp) {  return (base = std::min(base, cmp));  }
template <typename T>  inline T &chkMax(T &base, const T &cmp) {  return (base = std::max(base, cmp));  }

namespace Solution {
    void solve() {

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