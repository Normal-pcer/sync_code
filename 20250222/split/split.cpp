#include <bits/stdc++.h>
#define FILE_NAME "split"
using i32 = int32_t; using i64 = int64_t;
using u32 = uint32_t; using u64 = uint64_t;

/**
 * 后面对前面不会产生影响，从后往前找即可
 * 随便写一下
 */
namespace Solution {
    auto solve() -> void {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::freopen(FILE_NAME ".in", "r", stdin);
        std::freopen(FILE_NAME ".out", "w", stdout);

        i32 N;  std::cin >> N;
        std::vector<i32> vec(N);
        for (auto &x: vec)  std::cin >> x;

        i64 cur = 0;
        i32 cnt = 0;
        for (auto i = N; i --> 0; ) {
            auto x = vec[i];
            cur += x;
            if (cur >= 0)  cnt++, cur = 0;
        }
        if (cur < 0) {
            std::cout << 0 << "\n";
        } else {
            std::cout << cnt << "\n";
        }
    }
}

int main() {
    Solution::solve();
    return 0;
}