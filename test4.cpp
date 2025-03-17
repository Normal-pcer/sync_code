/**
 * @link https://www.luogu.com.cn/problem/P10138
 */
#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 一个限制 (a, h) 可以要求 [h] > [1], [2], ..., [h - 1]
 * 同时 [a + 1], [a + 2], ..., [h - 1] 一定比较小。
 * 
 * 考虑以下问题：
 * 共有 N 个数，指定部分数一定是前缀最大值，有多少种取法。
 * F[i][j] 表示前 i 个数，最大值为 j 的取法。
 * 转移：F[i][j] -> F[i + 1][k]（k > j）(a)
 * F[i][j] -> F[i + 1][j] * j (b)
 * 
 * 另外，本题的一个限制还要求一些牛不可以是前缀最大值。
 * 如果只能是前缀最大值，只能进行转移 a。
 * 如果不能是前缀最大值，只能进行转移 b。
 * 否则，正常进行转移。
 * 
 * 似乎这样可以保证正确性，但是复杂度比较劣，可以荣获 50 分。
 */
namespace Solution_9091860887338826 {
    i32 constexpr mod = 1e9 + 7;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, Q, C;
        std::cin >> N >> Q >> C;

        std::vector<std::pair<i32, i32>> limits(Q);
        for (auto &[x, y]: limits)  std::cin >> x >> y;

        enum CowType {
            NoneType, IsMax, NotMax
        };
        std::vector<CowType> cows(N + 1);

        for (auto [x, h]: limits) {
            for (i32 i = x + 1; i < h; i++)  cows[i] = NotMax;
            cows[h] = IsMax;
        }

        std::vector F(N + 1, std::vector<i32>(C + 1));
        ranges::fill(F[1], 1);
        for (i32 i = 1; i < N; i++) {
            for (i32 j = 1; j <= C; j++) {
                if (cows[i + 1] != IsMax) {
                    F[i + 1][j] = (F[i + 1][j] + static_cast<i64>(F[i][j]) * j % mod) % mod;
                }
                if (cows[i + 1] != NotMax) {
                    for (i32 k = j + 1; k <= C; k++) {
                        F[i + 1][k] = (F[i + 1][k] + F[i][j]) % mod;
                    }
                }
            }
        }

        for (i32 i = 1; i <= N; i++) {
            for (i32 j = 1; j <= C; j++) {
                std::cout << std::format("F[{}][{}] = {}", i, j, F[i][j]) << std::endl;
            }
        }

        i32 ans = 0;
        for (auto x: F[N])  ans = (ans + x) % mod;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9091860887338826::solve();
    return 0;
}
