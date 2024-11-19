#include <bits/stdc++.h>
#pragma GCC optimize(3)
#pragma GCC optimize("inline")
using ll = long long;
using ull = unsigned long long;

int N, Q;

namespace Prime {
    static const constexpr int _MAX = 4000'0000;
    static const constexpr int _N = 300'0000;

    std::vector<int> primes;
    std::bitset<_MAX> numbers;
    void pre() {
        for (auto p = 2; p < _N; p++) {
            if (!numbers[p])    primes.push_back(p);
            for (auto q=primes.begin(); q!=primes.end() && (*q)*p<=_N; q++) {
                numbers[(*q) * p] = true;
                if (p % (*q) == 0)  break;
            }
        }
    }

    bool check(int num) {
        for (auto p = 2; p * p <= num; p++)  if (num % p == 0)  return false;
        return true;
    }
}
using Prime::primes;

namespace Solution {
    void solve() {

        int N, K;
        std::cin >> N >> K;

        // for (auto i: primes)  std::cout << i << std::endl;
        // auto i = std::lower_bound(primes.begin(), primes.end(), N) - primes.begin() - 1;
        auto i = N;
        for (; i > 0; i--) {
            if (Prime::check(i))  break;
        }

        auto oneCount = N - i;

        if ((ll)(i - K) * (i - K) > (ll)(K - 1) * (K - 1) * i) {
            auto ans = (ll)(i - K) * (i - K);
            ans += oneCount * (K - 1);
            std::cout << ans << std::endl;
        } else {
            auto ans = (ll)(K - 1) * (K - 1) * N;
            std::cout << ans << std::endl;
        }
    }
}

int main() {
    // Prime::pre();
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    std::freopen("T3.in", "r", stdin);
    std::freopen("T3.out", "w", stdout);
#endif

    int T;  std::cin >> T;
    while (T --> 0)
        Solution::solve();
    return 0;
}