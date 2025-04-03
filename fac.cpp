#include <bits/stdc++.h>
#include "./libs/fixed_int.hpp"
#define FILENAME "fac"

char constexpr endl = '\n';

namespace Solution {
    class FacManager {
    public:    
        std::vector<i32> cnt;

        FacManager(i32 N): cnt(N + 1) {
            for (i32 i = 1; i <= N; i++) {
                for (i32 j = i; j <= N; j += i) {
                    cnt[j]++;
                }
            }
        }
    };
    auto solve() -> void {
        i32 N; std::cin >> N;
        FacManager fm{N};
        i32 ans = 0;
        for (i32 i = 1; i <= N; i++) {
            ans += fm.cnt[i];
        }
        std::cout << ans << endl;
    }
    auto solve2() -> void {
        i32 N; std::cin >> N;
        i32 ans = 0;
        for (i32 i = 1; i <= N; i++) {
            for (i32 j = 1; j <= i; j++) {
                if (i % j == 0) ans++;
            }
        }
        std::cout << ans << endl;
    }
}

auto main() -> int {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);

    Solution::solve();
}
// 111
