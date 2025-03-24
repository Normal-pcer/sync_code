#include <bits/stdc++.h>
#define FILE_NAME "T2"
#ifdef LOCAL
#define debug if (true)
#else  // not def LOCAL
#define debug if (false)
#endif  // def LOCAL

using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::int16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;

/*
9:05

D 关于 len 似乎有一些单调性。
可以先通过最大长度来获取希望的 D，然后二分查找最小可能的 len。

给定的 len，如何判断 D？
希望获取定长区间的最大值和最小值，滑动窗口即可。
*/
namespace Solution {
    i32 constexpr inf = 0x3f3f3f3f;
    char constexpr endl = '\n';
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U val, Func &&f = {}, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = begin + ((end - begin) >> 1);
            if (cmp(f(mid), val)) begin = mid + 1;
            else end = mid;
        }
        return begin;
    }
    auto solve() -> void {
        i32 N, K;  std::cin >> N >> K;
        std::vector<i32> a(N);
        for (auto &x: a)  std::cin >> x;

        // 获取给定 len 对应的 D 值
        auto getD = [&](i32 len) -> i32 {
            assert(N - len + 1 >= K);
            // 滑动窗口获取每个区间的最大值和最小值

            std::deque<i32> inc;  // a[i] 递增的单调队列
            std::deque<i32> dec;  // a[i] 递减的单调队列

            std::vector<i32> max(N, -inf);  // 以 i 为右端点的区间的最大值
            std::vector<i32> min(N, +inf);

            for (i32 i = 0; i != N; i++) {
                
                // 弹出过期元素
                while (not inc.empty() and inc.front() == i - len) inc.pop_front();
                while (not dec.empty() and dec.front() == i - len) dec.pop_front();

                // 维护单调队列
                while (not inc.empty() and a[inc.back()] >= a[i]) inc.pop_back();
                while (not dec.empty() and a[dec.back()] <= a[i]) dec.pop_back();
                inc.push_back(i), dec.push_back(i);

                // 记录最大/最小值
                if (not inc.empty()) min[i] = a[inc.front()];
                if (not dec.empty()) max[i] = a[dec.front()];

                // for (auto x: inc)  std::cout << x << "(" << a[x] << ") ";
                // std::cout << std::endl;
                // for (auto x: dec)  std::cout << x << "(" << a[x] << ") ";
                // std::cout << std::endl;
                // std::cout << "qwq " << max[i] - min[i] << std::endl;
            }
            // 获取答案
            std::vector<i32> diffs(N);
            std::transform(max.begin(), max.end(), min.begin(), diffs.begin(), std::minus<>{});
            diffs.erase(diffs.begin(), diffs.begin() + len - 1);
            auto mid_index = K - 1;
            std::nth_element(diffs.begin(), diffs.begin() + mid_index, diffs.end(), std::greater<>{});
            // std::cout << "getD " << len << " -> " << diffs[mid_index] << std::endl;
            // std::cout << "diffs: " << std::endl;
            // for (auto x: diffs)  std::cout << x << " ";
            // std::cout << std::endl;
            return diffs[mid_index];
        };
        static_cast<void>(getD);
        auto getD2 = [&](i32 len) -> i32 {
            assert(len < N);
            std::vector<i32> diffs;
            for (i32 i = 0; i + len <= N; i++) {
                auto max = *std::max_element(a.begin() + i, a.begin() + i + len);
                auto min = *std::min_element(a.begin() + i, a.begin() + i + len);
                diffs.push_back(max - min);
            }
            std::sort(diffs.begin(), diffs.end(), std::greater<>{});
            assert(diffs.size() >= static_cast<uz>(K));
            std::cout << "diffs: " << std::endl;
            for (auto x: diffs)  std::cout << x << " ";
            std::cout << std::endl;
            return diffs[K - 1];
        };
        static_cast<void>(getD2);

        auto max_len = N - K + 1;
        auto max_d = getD(max_len);

        auto min_len = lowerBoundValue(1, max_len + 1, max_d, getD);  // 找到第一个合理的区间长度
        std::cout << max_d << " " << min_len << endl;
    }
}

auto main() -> int {
    // std::freopen(FILE_NAME ".in", "r", stdin);
    // std::freopen(FILE_NAME ".out", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T;  std::cin >> T;
    while (T --> 0) Solution::solve();

    return 0;
}