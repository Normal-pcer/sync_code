/**
 * @link https://www.luogu.com.cn/problem/P2051
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
那么，似乎是要求一行内不能有三个棋子，一列内也不能有三个棋子。
考虑如何描述状态。
可以记录有几列已经放了 2 个棋子，有几列已经放了 1 个棋子。
似乎是有道理的。

F[i][cnt0][cnt1] 表示：
当前一共遍历了 i 行，上一行有 cnt0 列没有放棋子，cnt1 列恰好放了 1 个棋子。这种情况的方案数。

考虑转移。感觉向外递推好写一些。
F[i][cnt0][cnt1]，希望继续取若干列。

希望在 cnt0 上面取 x 个，cnt1 上面取 y 个。
F[i + 1][cnt0 - x][cnt1 - y + x] <- F[i][cnt0][cnt1] * C(cnt0, x) * C(cnt1, y);

初始值：F[0][cols][0] = 1。
答案：F[rows] 求和。

这个东西是 5 次方，复杂度有点劣了。

一行最多有两个棋子，所以随便转移就行了。

*/
namespace Solution_6431247307212205 {
    i32 constexpr mod = 9999973;
    auto constexpr addMod(i32 a, i32 b) -> i32 {
        a += b;
        if (a >= mod) a -= mod;
        return a;
    }
    auto constexpr mulMod(i32 a, i32 b) -> i32 {
        return static_cast<i32>(static_cast<i64>(a) * b % mod);
    }
    auto constexpr powMod(i64 a, i64 b) -> i32 {
        i64 res = 1;
        for (; b != 0; b >>= 1, a = a * a % mod) {
            if (b & 1) res = res * a % mod;
        }
        return res;
    }
    auto solveForce() -> void {
        i32 rowsCount, colsCount;
        std::cin >> rowsCount >> colsCount;

        auto blocksCount = rowsCount * colsCount;
        using Stat = u32;
        auto statCount = (Stat)1 << blocksCount;

        i32 ans = 0;
        for (Stat s = 0; s != statCount; s++) {
            auto getAt = [&](i32 row, i32 col) -> Stat {
                auto pos = row * colsCount + col;
                return s & ((Stat)1 << pos);
            };

            auto correct = [&]() -> bool {
                for (i32 i = 0; i != rowsCount; i++) {
                    for (i32 j = 0; j != colsCount; j++) {
                        if (not getAt(i, j)) continue;
                        // 是否存在棋子可以互相攻击
                        auto walkAxis = [&](i32 x, i32 y, i32 dx, i32 dy) -> bool {
                            auto isPointLegal = [&](i32 x, i32 y) -> bool {
                                return 0 <= x and x < rowsCount and 0 <= y and y < colsCount;
                            };
                            i32 cnt = 0;
                            while (isPointLegal(x, y)) {
                                cnt += static_cast<bool>(getAt(x, y));
                                if (cnt == 3) return true;
                                x += dx, y += dy;
                            }
                            return false;
                        };
                        auto correct = [&]() -> bool {
                            auto flag = false;
                            flag |= walkAxis(i, j, 0, 1);
                            flag |= walkAxis(i, j, 0, -1);
                            flag |= walkAxis(i, j, 1, 0);
                            flag |= walkAxis(i, j, -1, 0);
    
                            return not flag;
                        }();
                        if (not correct) return false;
                    }
                }
                return true;
            }();
            if (correct) std::cout << std::bitset<4>(s) << std::endl;
            ans += correct;
        }

        std::cout << ans % mod << endl;
    }
    struct FacManager {
        std::vector<i32> fac;
        std::vector<i32> invFac;

        FacManager(i32 n): fac(n + 1), invFac(n + 1) {
            fac[0] = 1;
            for (i32 i = 1; i <= n; i++) {
                fac[i] = mulMod(fac[i - 1], i);
            }
            invFac[n] = powMod(fac[n], mod - 2);
            for (i32 i = n; i --> 0; ) {
                invFac[i] = mulMod(invFac[i + 1], i + 1);
            }
            assert(invFac[0] == 1);
        }

        auto calcC(i32 a, i32 b) -> i32 {
            assert(a >= b and b >= 0);
            return mulMod(mulMod(fac[a], invFac[b]), invFac[a - b]);
        }
    };
    auto solve() -> void {
        i32 rowsCount, colsCount;
        std::cin >> rowsCount >> colsCount;

        i32 constexpr maxColsCount = 101;
        FacManager fm{maxColsCount};
        
        std::vector F(rowsCount + 1, std::vector(colsCount + 1, std::vector<i32>(colsCount + 1, 0)));
        F[0][colsCount][0] = 1;
        for (i32 i = 0; i < rowsCount; i++) {
            for (i32 cnt0 = 0; cnt0 <= colsCount; cnt0++) {
                for (i32 cnt1 = 0; cnt1 <= colsCount; cnt1++) {
                    // 从 F[i][cnt0][cnt1] 向外转移
                    // 如果在这一行放置两个？
                    auto checkTransform = [&](i32 ncnt0, i32 ncnt1, i32 scale) -> void {
                        if (ncnt0 < 0 or ncnt1 < 0) return;
                        if (ncnt0 + ncnt1 > colsCount) return;
                        F[i + 1][ncnt0][ncnt1] = addMod(F[i + 1][ncnt0][ncnt1], mulMod(F[i][cnt0][cnt1], scale));
                    };

                    checkTransform(cnt0 - 1, cnt1, mulMod(cnt0, cnt1));  // 0 1
                    checkTransform(cnt0 - 2, cnt1 + 2, fm.calcC(std::max(2, cnt0), 2)); // 0 0
                    checkTransform(cnt0, cnt1 - 2, fm.calcC(std::max(2, cnt1), 2));  // 1 1
                    checkTransform(cnt0 - 1, cnt1 + 1, cnt0); // 0
                    checkTransform(cnt0, cnt1 - 1, cnt1);  // 1
                    checkTransform(cnt0, cnt1, 1);  // 无
                }
            }
        }

        i32 ans = 0;
        for (i32 cnt0 = 0; cnt0 <= colsCount; cnt0++) {
            for (i32 cnt1 = 0; cnt1 <= colsCount; cnt1++) {
                ans = addMod(ans, F[rowsCount][cnt0][cnt1]);
            }
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_6431247307212205::solve();
    debug Solution_6431247307212205::solveForce();
    return 0;
}
