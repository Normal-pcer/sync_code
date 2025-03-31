/**
 * @link https://www.luogu.com.cn/problem/P1654
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/rolling_container.hpp"
using namespace lib;

/*
先考虑比较暴力的方式。
F[i][j] 表示经历了前 i 次操作，且第 i 次操作时连击恰好为 j，这种情况的期望。
G[i][j] 表示这种情况的发生概率。

转移：G[i][j] 有 p 的概率转移到 G[i + 1][j + 1]，1 - p 的概率转移到 G[i + 1][0]。
F[i][j] 同理。

并非正确。
考虑枚举最后一段连续的 1。

G[i][j] 表示以 i 结尾的连续 1 串，长度为 j 的概率。这个东西很好求。
那么答案 F[i] = sum G[i][j] * pow(j, 3)，这样似乎是有道理的。

---

假如把一段连续 1 的贡献设为 len 而非  pow(len, 3)，应该如何做？
题解写得有些抽象，还好 DeepSeek 大夫妙手回春。

x1[i]：处理完前 i 次操作后，以第 i 个位置结尾的 极长连续 1 的长度的期望值。
只需要统计 x1[i] * (1 - p[i]) 的求和。因为需要在每一次遇到 0 的时候统计贡献。

x1[i] 的转移：x1[i] = (x1[i - 1] + i) * p[i] + 0 * (1 - p[i])。

原题怎么做？
同理，x2[i] 表示长度平方的期望值。
x2[i] 的转移：x2[i] = (x2[i - 1] + 2 * x1[i - 1] + 1) * pi + 0 * (1 - p[i])
这个式子的前半部分是一个完全平方公式。

x3 表示长度立方的期望。
x3[i] = (x3[i - 1] + 3 * x2[i - 1] + 3 * x1[i - 1] + 1) * p[i]

最后统计答案，ans = sum x3 * (1 - p[i])

然而并不正确。
换一个角度，ans[i] 表示此时的答案。
ans[i] 每次的增量即为 x2[i - 1] * 3 + x1[i - 1] * 3 + 1;
*/
namespace Solution_8887285103618128 {
    using f64 = double;
    auto solveForce() -> void {
        i32 N; std::cin >> N;

        using Prob = f64;
        std::vector<Prob> ps(N);
        for (auto &x: ps) {
            std::cin >> x;
        }

        std::mt19937 rng{std::random_device{}()};

        i32 constexpr iterCount = 1'000'000;
        f64 ans = 0;
        for (auto t = iterCount; t --> 0; ) {
            std::vector<char> results(N);  // 所有判定结果
            for (i32 i = 0; i < N; i++) {
                if (static_cast<f64>(rng()) / rng.max() <= ps[i]) {
                    results[i] = true;
                } else {
                    results[i] = false;
                }
            }

            i32 prevLen = 0;
            for (i32 i = 0; i <= N; i++) {
                if (i != N and results[i]) {
                    prevLen++;
                } else {
                    ans += prevLen * prevLen * prevLen;
                    prevLen = 0;
                }
            }
        }

        ans /= iterCount;
        std::cout << std::fixed << std::setprecision(1) << ans << endl;
    }
    auto solve() -> void {
        i32 N; std::cin >> N;

        using Prob = f64;
        std::vector<Prob> probs(N + 1);  // 1-index
        for (auto &x: probs | views::drop(1)) {
            std::cin >> x;
        }

        std::vector<Prob> x1(N + 1), x2(N + 1), ans(N + 1);

        for (i32 i = 1; i <= N; i++) {
            auto p = probs[i];
            // 计算下一个点的长度期望
            x1[i] = (x1[i - 1] + 1) * p;
            x2[i] = (x2[i - 1] + 2 * x1[i - 1] + 1) * p;
            ans[i] = ans[i - 1] + (3 * x2[i - 1] + 3 * x1[i - 1] + 1) * p;
        }
        std::cout << std::fixed << std::setprecision(1) << ans[N] << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_8887285103618128::solve();
    return 0;
}
