#include <bits/stdc++.h>
using ll = long long;

namespace Solution {

    int N;  const int _N = 250'005;
    ll K;

    int res[_N];
    auto current = 0LL;  // 当前的组数

    void solve() {
        std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
        std::cin >> N >> K;

        // 先添加一定数量的 1
        // 1 和后面的所有数之间必然会产生贡献，一直添到总和恰好小于 0
        auto i = 1;  // 添加数的指针
        for (i = 1; i <= N; i++) {
            auto del = N - i;  // 新增的贡献
            auto new_sum = current + del;
            if (new_sum > K)  break;
            res[i] = 1, current = new_sum;
        }

        // 向后添加一些连续的数，每一段都会产生一些贡献
        // 避免这些数之间互相影响，只需填充 N, N+1, N+2, ...
        // 直到满足 cur == K
        for (auto j = N; current < K; j++) {  // 接下来，填充若干个 j
            for (auto len = 1; ; len++) {  // 再填充一个新的
                auto new_sum = current + len - 1;
                if (new_sum > K)  break;
                res[i] = j, current = new_sum;
                i++;
            }
        }

        assert(current == K);

        // 剩下的随便填一下即可
        for (; i <= N; i++)  res[i] = N + i - 1;

        for (auto i = 1; i <= N; i++)  std::cout << res[i] << ' ';
        std::cout << std::endl;
    }
}

int main() {
    Solution::solve();
    return 0;
}