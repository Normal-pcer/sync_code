/**
 * @link https://www.luogu.com.cn/problem/P6146
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}

using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::uint16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
using namespace lib;

/**
 * 不妨先取几条线段，考虑它的所有子集。
 * F[i] 表示前 i 条线段所有子集的复杂度之和
 * 按照线段右端点排序
 * 对于前面的部分线段可以确定不会有联通块，其他的联通块可能会更多。
 */
namespace Solution_8449243432412487 {
    i32 constexpr mod = 1e9 + 7;
    auto qpow(i64 a, i64 b, i32 const mod) {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % mod) {
            if (b & 1)  res = res * a % mod;
        }
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        struct Segment {
            i32 begin = 0, end = 0;
        };
        std::vector<Segment> segs(N);
        for (auto &[x, y]: segs)  std::cin >> x >> y;

        ranges::sort(segs, std::less{}, lam(x, x.begin));  // 按照右端点排序
        std::vector segs_sorted_by_end(segs);
        ranges::sort(segs_sorted_by_end, std::less{}, lam(x, x.end));

        i32 ans = 0;
        for (i32 i = 0; i < N; i++) {
            auto limit = segs[i].begin;
            auto x = std::distance(segs_sorted_by_end.begin(), ranges::lower_bound(segs_sorted_by_end, limit, std::less{}, lam(x, x.end)));
            ans = ((ans + ans) % mod + qpow(2, x, mod)) % mod;
            debug  std::cout << " i = " << i << " " << ans << std::endl;
        }
        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8449243432412487::solve();
    return 0;
}
