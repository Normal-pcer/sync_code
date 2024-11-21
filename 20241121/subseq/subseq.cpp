#include <bits/stdc++.h>
#define filename "subseq"
using ll = long long;
using ull = unsigned long long;
using i128 = __int128;

namespace Solution {
    int L, R;
    const int _N = 20;
    int F[_N][_N];
    std::string s;
    std::string n;

    const int mod = 998244353;

    void solve() {
        std::cin >> L >> R;
        if (L != R) {
            std::cout << "I AK IOI" << std::endl;
            return;
        }

        n = std::to_string(L);
        std::cin >> s;
        // F[0][0][0] = 1;
        for (auto i = 1; i <= (int)n.size(); i++) {
            F[i][0] = 1;
        }
        for (auto i = 1; i <= (int)n.size(); i++) {
            for (auto j = 0; j <= (int)s.size(); j++) {
                // F[i][j] += F[i-1][j], F[i][j] %= mod;
                if (j != 0) {
                    for (auto k = i - 1; k > 0; k--) {
                        if (s[j-1] == '>') {
                            if (n[k-1] > n[i-1]) {
                                F[i][j] += F[k][j-1], F[i][j] %= mod;
                            }
                        } else {
                            if (n[k-1] < n[i-1]) {
                                F[i][j] += F[k][j-1], F[i][j] %= mod;
                            }
                        }
                    }
                }

                // std::cout << i << ' ' << j << ' ' << F[i][j] << std::endl;
            }
        }

        auto ans = 0;
        for (auto i = 1; i <= (int)n.size(); i++) {
            ans += F[i][s.size()], ans %= mod;
        }

        std::cout << ans << std::endl;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(filename".in", "r", stdin);
    std::freopen(filename".out", "w", stdout);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
    return 0;
}