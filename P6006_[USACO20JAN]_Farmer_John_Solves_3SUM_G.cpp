/**
 * @link https://www.luogu.com.cn/problem/P6006
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * F[i][j] 表示 F[i] + F[j] + F[k] = 0 的三元组数量（k 在 [i, j] 中）。
 * F[i][j] 比较好算，略过。
 * 
 * G[i][j] 表示这个区间的答案。
 * 可以发现，x >= i, y <= j 的 F[x][y] 均可以符合要求。
 * F[i][j] 求一个二维前缀和即可
 */
namespace Solution_8202410420946550 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, Q;  std::cin >> N >> Q;
        std::vector<i32> a(N);
        for (auto &x: a)  std::cin >> x;

        auto min = ranges::min(a);
        auto max = ranges::max(a);
        // 计算 F[i][j]
        // 统计出每个数的出现次数
        std::vector<i32> cnt(max - min + 1);
        std::vector F_ps(N, std::vector<i64>(N));
        for (i32 i = 0; i < N; i++) {
            for (i32 j = i + 1; j < N; j++) {
                auto value = -a[i] - a[j];
                if (value >= min and value <= max)  F_ps[i][j] = cnt[value - min];
                cnt[a[j] - min]++;
            }
            // 清空 cnt
            // 再开一遍 F 或者使用 memset 清零是 O(NV)，似乎复杂度不是特别正确
            for (i32 j = i + 1; j < N; j++) {
                cnt[a[j] - min]--;
            }
        }

        for (i32 i = 0; i < N; i++) {
            for (i32 j = 0; j < N; j++) {
                if (i != 0)  F_ps[i][j] += F_ps[i - 1][j];
                if (j != 0)  F_ps[i][j] += F_ps[i][j - 1];
                if (i != 0 and j != 0)  F_ps[i][j] -= F_ps[i - 1][j - 1];
            }
        }

        for (i32 _ = Q; _ --> 0; ) {
            i32 x, y;  std::cin >> x >> y, x--, y--;
            auto ans = F_ps[N - 1][y];
            if (x != 0)  ans -= F_ps[x - 1][y];

            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8202410420946550::solve();
    return 0;
}
