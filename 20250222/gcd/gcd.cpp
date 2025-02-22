#pragma GCC optimize("Ofast")
#ifdef LOCAL
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif

#include <bits/stdc++.h>
#define FILE_NAME "gcd"
using i32 = int32_t; using i64 = int64_t;
using u32 = uint32_t; using u64 = uint64_t;

char constexpr endl = '\n';

/*
考虑求多少个区间的 gcd 为 x
固定区间的左端点为 l，区间右端点向右移动。
这个过程中，区间 gcd 一定不减。
可以二分查找最小和最大的区间。

对于查询的每个 gcd 都可以这么做，40% 的数据 gcd 的种类不会太多，40% 的数据查询次数不会太多。
单次复杂度是 O(N*logN*logN)
加个记忆化感觉可以获得 80 分。
经过验证，手动构造方案，N = 3000 还是可以过的。
不过这样明显可以被卡。

*/
namespace Solution {
    template <typename T>  T gcd(T x, T y) { return std::__gcd(x, y); }
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lower_bound_mapping(T begin, T end, U val, Func &&mapping, Comp cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = begin + ((end - begin) >> 1);
            if (cmp(mapping(mid), val)) {
                begin = mid + 1;
            } else {
                end = mid;
            }
        }
        return begin;
    }
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto upper_bound_mapping(T begin, T end, U val, Func &&mapping, Comp cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = begin + ((end - begin) >> 1);
            if (not cmp(val, mapping(mid))) {
                begin = mid + 1;
            } else {
                end = mid;
            }
        }
        return begin;
    }
    auto solve() -> void {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::freopen(FILE_NAME ".in", "r", stdin);
        std::freopen(FILE_NAME ".out", "w", stdout);

        i32 N;  std::cin >> N;
        std::vector<i32> a(N);
        for (auto &x: a)  std::cin >> x;

        // F[j][i] 表示 [i, i + 1 << j) 个数的区间 gcd
        auto log_N = std::__lg(N);
        std::vector<std::vector<i32>> F(log_N + 1, std::vector<i32>(N));
        F[0] = a;
        for (i32 j = 1; j <= log_N; j++) {
            for (i32 i = 0; i < N; i++) {
                if (i + (1 << j) > N)  break;
                F[j][i] = gcd(F[j - 1][i], F[j - 1][i + (1 << (j - 1))]);
            }
        }

        auto query = [&](i32 begin, i32 end) -> i32 {
            if (begin == end)  return 0x3f3f3f3f;
            auto dis = end - begin;
            auto log_dis = std::__lg(dis);

            return gcd(F[log_dis][begin], F[log_dis][end - (1 << log_dis)]);
        };

        std::unordered_map<i32, i64> mem;  // query_segs 的记忆化
        auto query_segs = [&](i32 x) -> i64 {
            auto it = mem.find(x);
            if (it != mem.end())  return it->second;

            i64 ans = 0;
            // 枚举左端点
            for (i32 l = 0; l < N; l++) {
                auto r1 = lower_bound_mapping(l, N + 1, x, [&](int x) { return query(l, x); }, std::greater<>{});
                auto r2 = upper_bound_mapping(l, N + 1, x, [&](int x) { return query(l, x); }, std::greater<>{});

                ans += r2 - r1;
            }
            mem[x] = ans;
            return ans;
        };

        i32 Q;  std::cin >> Q;
        while (Q --> 0) {
            i32 l, r;  std::cin >> l >> r;
            l--, r--;  // 下标从 0 开始
            auto x = query(l, r + 1);
            std::cout << x << " " << query_segs(x) << endl;
        }
    }
}

int main() {
    Solution::solve();
    return 0;
}