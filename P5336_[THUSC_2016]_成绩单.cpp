/**
 * @link https://www.luogu.com.cn/problem/P5336
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
using namespace lib;

/*
看起来就很想按照分数排序。
1 2 6 7 7 7 9 10 10 10
然后决定几个分界点，分成若干组。
例如用 F[i][j] 表示已经分了 i 段，最后一段的结尾在 j 点时的最小代价。
转移，只需要决定最后一段从谁开始划分。
即 F[i][j] <- F[i - 1][k] + a + b * (score[i] - score[k + 1])，取最小值。
然后好像就可以了。

什么啊。不能排序。
那也差不多？把这个改成最大值减最小值即可。

读错题了。
可以考虑把 min 和 max 打进状态里。最后大概是这样的：
F[l][r][min][max] 表示 在 [l, r] 区间内发放试卷，使得剩余的最大值为 min，最小值为 max，最小的代价。
经过离散化，状态数是 n^4 级别。

考虑转移。
或许有一些难以转移。设置另一个状态：
G[l][r] 表示把 [l, r] 区间完全分发的最小代价。

G[l][r] 可以很容易通过 F 中的信息得到。枚举可能的 min 和 max，于是有：
G[l][r] <- F[l][r][min][max] + a + b * (max - min)**2。

那么它能不能反过来帮助 F 的转移？
假设要向 [l, r] 的右面再增加一点东西。
我们可以向右面添加并保留单个元素。
F[l][r + 1][newmin][newmax] <- F[l][r][min][max] (a)

可以向右面添加一个完全分发的区间。
F[l][k][min][max] <- F[l][r][min][max] + G[r + 1][k] (b)

然后考虑转移顺序。首先按照 [l, r] 区间从小到大，然后 [min, max] 从小到大。
每个 [l, r] 处理完 F 就计算 G。

初始状态：F[l][l][score[l]][score[l]] = 0。
答案：G[1][N]。

转移顺序错了。
(b) 转移稍微变换一下。
F[l][r][min][max] <- F[l][k][min][max] + G[k + 1][r] (b)
*/
namespace Solution_5739888844543107 {
    auto solve() -> void {
        i32 n, a, b;
        std::cin >> n >> a >> b;

        using Score = i32;
        std::vector<Score> scores(n + 1);
        for (auto &x: scores | views::drop(1)) std::cin >> x;
        auto values = scores;
        {  // 离散化
            values.erase(values.begin());
            ranges::sort(values);
            auto tmp = ranges::unique(values);
            values.erase(tmp.begin(), tmp.end());

            for (auto &x: scores | views::drop(1)) {
                auto it = ranges::lower_bound(values, x);
                assert(it != values.end() and *it == x);
                x = std::distance(values.begin(), it);
            }
        }
        auto v = static_cast<i32>(values.size());

        using Cost = i32;
        std::vector F(n + 1, std::vector(n + 1, std::vector(v, std::vector(v, (Cost)inf))));
        std::vector G(n + 1, std::vector(n + 1, (Cost)inf));

        for (i32 l = 1; l <= n; l++) {
            F[l][l][scores[l]][scores[l]] = 0;
            G[l][l] = a;
        }

        for (i32 len = 1; len <= n; len++) {
            for (i32 l = 1; l <= n; l++) {
                auto r = l + len - 1;
                if (r > n) break;
                for (i32 len2 = 1; len2 < v; len2++) {
                    for (i32 min = 0; min < v; min++) {
                        auto max = min + len2 - 1;
                        if (max >= v) break;
                        
                        for (i32 k = l; k < r; k++) {
                            chkMin(F[l][r][min][max], F[l][k][min][max] + G[k + 1][r]);
                        }
                        if (r != n) {
                            auto newmin = std::min(min, scores[r + 1]);
                            auto newmax = std::max(max, scores[r + 1]);
                            chkMin(F[l][r + 1][newmin][newmax], F[l][r][min][max]);
                        }

                    }
                }

                for (i32 min = 0; min < v; min++) {
                    for (i32 max = min; max < v; max++) {
                        chkMin(G[l][r], F[l][r][min][max] + a + b * (values[max] - values[min]) * (values[max] - values[min]));
                    }
                }
            }
        }

        debug for (i32 i = 0; i <= n; i++) {
            for (i32 j = 0; j <= n; j++) {
                for (i32 k = 0; k < v; k++) {
                    for (i32 l = 0; l < v; l++) {
                        if (F[i][j][k][l] != inf) {
                            std::cout << "F[" << i << "][" << j << "][" << k << "][" << l << "] = " << F[i][j][k][l] << std::endl;
                        }
                    }
                }
            }
        }

        debug for (i32 i = 0; i <= n; i++) {
            for (i32 j = 0; j <= n; j++) {
                if (G[i][j] != inf) {
                    std::cout << "G[" << i << "][" << j << "] = " << G[i][j] << std::endl;
                }
            }
        }

        auto ans = G[1][n];
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_5739888844543107::solve();
    return 0;
}
