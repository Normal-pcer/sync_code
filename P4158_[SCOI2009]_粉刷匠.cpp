/**
 * @link https://www.luogu.com.cn/problem/P4158
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
using namespace lib;

/*
F[i][j] 表示前 i 行完成粉刷，剩余 j 步，此时的最大分数。

希望转移，需要想办法算出如何在若干步之内粉刷单独的一行。
G[i][j] 表示前 i 个格子完成粉刷，剩余 j 步的最大分数。

G[i][j] <- G[prev][j + 1] + extra，extra 为 [prev + 1, i] 区间内 0 和 1 数量的较大值。
那么其实不需要显式地分出 F 和 G 两个步骤。重新设置状态：
F[i][j][t] 表示前 i - 1 行、以及第 i 行的前 j 个格子完成了粉刷，还剩余 t 步，此时的最大分数。
转移分为两个部分：
F[i][j][t] <- F[i][prev][t + 1] + extra，extra 见上文。（a）
F[i][0][t] <- F[i - 1][any][t]（b）

状态为 n * m * t，6.25e6 级别
转移需要枚举 prev，还需要乘以 50，可能有点极限。
考虑优化。
对于每一行，记录 pre0 和 pre1 为前缀的 0/1 计数。
那么 extra 的计算可以变成：
max {
    pre0[i] - pre0[prev]
    pre1[i] - pre1[prev]
}
把 pre0 和 pre1 两个系列分开来看，可以把 F[i][prev][t] - pre0[prev] 绑在一起记录一个前缀最小值。
pre1 同理。最后可以做到 O(1) 转移。
*/
namespace Solution_7127839051736593 {
    auto solve() -> void {
        i32 rowCount, blockCount, timeLimit;
        std::cin >> rowCount >> blockCount >> timeLimit;

        // 行列均为 1-index
        std::vector mat(rowCount + 1, std::vector<char>(blockCount + 1));
        for (i32 i = 1; i <= rowCount; i++) {
            for (i32 j = 1; j <= blockCount; j++) {
                char ch; std::cin >> ch;
                mat[i][j] = ch == '1';
            }
        }

        std::vector F(rowCount + 1, std::vector(blockCount + 1, std::vector(timeLimit + 1, (i32)-inf)));
        F[0][0][timeLimit] = 0;
        for (i32 row = 1; row <= rowCount; row++) {
            // 预处理这一行的相关信息
            // 前缀 0 和 1 的数量
            std::vector preCount(2, std::vector<i32>(blockCount + 1, 0));
            for (i32 i = 1; i <= blockCount; i++) {
                preCount[mat[row][i]][i]++;
            }
            std::partial_sum(preCount[0].begin(), preCount[0].end(), preCount[0].begin());
            std::partial_sum(preCount[1].begin(), preCount[1].end(), preCount[1].begin());

            // 转移 b
            for (i32 t = 0; t <= timeLimit; t++) {
                i32 max = -inf;
                for (i32 j = 0; j <= blockCount; j++) {
                    chkMax(max, F[row - 1][j][t]);
                }
                F[row][0][t] = max;
            }

            // preMax[i][t] 表示 F[row][j][t] - preCount[i][j] 的前缀最大值，j 小于当前枚举到的 i。
            std::vector preMax(2, std::vector(timeLimit + 1, (i32)-inf));
            for (i32 block = 1; block <= blockCount; block++) {
                // block - 1 现在有效了
                for (auto num: {0, 1}) {
                    for (i32 t = 0; t <= timeLimit; t++) {
                        chkMax(preMax[num][t], F[row][block - 1][t] - preCount[num][block - 1]);
                    }
                }

                // a 转移
                for (i32 t = 0; t < timeLimit; t++) {
                    chkMax(F[row][block][t], preMax[0][t + 1] + preCount[0][block]);
                    chkMax(F[row][block][t], preMax[1][t + 1] + preCount[1][block]);
                }
            }
        }

        i32 ans = -inf;
        for (i32 i = 0; i <= rowCount; i++) {
            for (i32 j = 0; j <= blockCount; j++) {
                for (i32 t = 0; t <= timeLimit; t++) {
                    chkMax(ans, F[i][j][t]);
                }
            }
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_7127839051736593::solve();
    return 0;
}
