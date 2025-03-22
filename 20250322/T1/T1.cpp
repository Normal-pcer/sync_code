#include <bits/stdc++.h>
#define FILE_NAME "T1"
#ifdef LOCAL
#define debug if (true)
#else  // not def LOCAL
#define debug if (false)
#endif  // def LOCAL

using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::int16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;

char constexpr endl = '\n';

/*
考虑最劣的情况。此时一个似乎有道理的贪心是让大家的分数都尽可能相等。
这样最坏的情况可以把主角的排名卡到 a + b - 1。
特别地，需要和 N 取 min。

考虑最优的情况。如果 a + b <= N 一定可以通过一些方式把主角送上第一名。
观察大样例发现即使是最大的样例也有特别多的 1，似乎可以印证这个猜想。
否则，可以让靠后的几个数互相配对，留几个特别好的。
排名似乎为 a + b - N + 1。
似乎需要特判。如果两次排名都足够差，最优的情况也必须是最后一名。
*/
namespace Solution {
    auto solve() -> void {
        i64 N, a, b;  std::cin >> N >> a >> b;

        auto ans_best = (a + b <= N? 1: a + b - N + 1);
        if (a + b >= N + N - 1)  ans_best = N;
        auto ans_worst = std::min(N, a + b - 1);

        std::cout << ans_best << " " << ans_worst << endl;
    }
}

auto main() -> int {
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T;  std::cin >> T;
    while (T --> 0) {
        Solution::solve();
    }

    return 0;
}