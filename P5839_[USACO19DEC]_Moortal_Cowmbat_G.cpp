/**
 * @link https://www.luogu.com.cn/problem/P5839
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
using namespace lib;

/*
首先跑一下最短路，判断一个字母修改为另外一个字母的最优方案。

一个显然的 n^2 做法：
F[i] 表示前 i 个字母的最优解。
枚举一个断点 j，F[i] <- F[j] + ...(全变为某个字母的花费)

考虑优化。首先可以记录一个前缀和 s[i][j]，表示把一个前缀 [1, i] 全部修改为颜色 j 需要的代价。
这样上述的式子可以转化为：
F[i] <- min( F[k] - s[k][j] + s[i][j] )
    <- min( F[k] - s[k][j] ) + s[i][j]

记录一个前缀最小值。由于 k 需要足够小，所以不能立即更新前缀最小值。
在计算 F[i] 之前，使用 F[i - K][j] 更新即可。
*/
namespace Solution_5068112500600457 {
    auto solveForce() -> void {
        i32 len, lettersCount, segLenLimit;
        std::cin >> len >> lettersCount >> segLenLimit;

        std::string s;
        std::cin >> s;
        s.insert(s.begin(), '_');

        using Cost = i32;
        std::vector mat(lettersCount, std::vector<Cost>(lettersCount));
        for (auto &line: mat) {
            for (auto &item: line) {
                std::cin >> item;
            }
        }

        for (i32 mid = 0; mid != lettersCount; mid++) {
            for (i32 from = 0; from != lettersCount; from++){
                for (i32 to = 0; to != lettersCount; to++) {
                    chkMin(mat[from][to], mat[from][mid] + mat[mid][to]);
                }
            }
        }

        std::vector F(len + 1, (Cost)inf);
        F[0] = 0;
        for (i32 i = 1; i <= len; i++) {
            for (i32 letter = 0; letter != lettersCount; letter++) {
                Cost totalCost = 0;
                for (i32 j = i; j --> 0; ) {
                    auto len = i - j;
                    totalCost += mat[s[j + 1] - 'a'][letter];
                    if (len >= segLenLimit) {
                        chkMin(F[i], F[j] + totalCost);
                    }
                }
            }
        }
        auto ans = F[len];
        std::cout << ans << endl;
    }
    auto solve() -> void {
        i32 len, lettersCount, segLenLimit;
        std::cin >> len >> lettersCount >> segLenLimit;

        std::string s;
        std::cin >> s;
        s.insert(s.begin(), '_');

        using Cost = i32;
        std::vector mat(lettersCount, std::vector<Cost>(lettersCount));
        for (auto &line: mat) {
            for (auto &item: line) {
                std::cin >> item;
            }
        }

        for (i32 mid = 0; mid != lettersCount; mid++) {
            for (i32 from = 0; from != lettersCount; from++){
                for (i32 to = 0; to != lettersCount; to++) {
                    chkMin(mat[from][to], mat[from][mid] + mat[mid][to]);
                }
            }
        }

        // psCost[i][j] 表示把一个前缀 [1, i] 全部修改为字母 j 需要的代价
        std::vector psCost(len + 1, std::vector<Cost>(lettersCount, 0));
        for (i32 letter = 0; letter != lettersCount; letter++) {
            for (i32 i = 1; i <= len; i++) {
                psCost[i][letter] = psCost[i - 1][letter] + mat[s[i] - 'a'][letter];
            }
        }

        std::vector F(len + 1, (Cost)inf);
        // 每个字母对应的 F[i] - s[i][j] 前缀最小值（i 必须可以取到）
        std::vector preMin(lettersCount, (Cost)inf);

        F[0] = 0;
        for (i32 i = 1; i <= len; i++) {
            // k = i - limit 的 pMin[k] 现在变得合法
            if (auto k = i - segLenLimit; k >= 0) {
                for (i32 letter = 0; letter != lettersCount; letter++) {
                    chkMin(preMin[letter], F[k] - psCost[k][letter]);
                }
            }
            for (i32 letter = 0; letter != lettersCount; letter++) {
                chkMin(F[i], preMin[letter] + psCost[i][letter]);
            }
        }

        auto ans = F[len];
        std::cout << ans << endl;
    }
}

auto main(int cyy, char const *gzh[]) -> int {
    DEBUG_MODE = (cyy != 1) and (std::strcmp("-d", gzh[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_5068112500600457::solve();
    return 0;
}
