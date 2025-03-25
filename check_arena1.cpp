/**
 * @link https://www.luogu.com.cn/problem/P3239
 */
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <ranges>
#include <vector>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
char constexpr endl = '\n';

#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
#include "./libs/fixed_int.hpp"

/*

考虑每一张牌本身造成的伤害期望。

一张牌本身能造成多少伤害，取决于它有多大的概率被使用。
第一张牌很好分析：
0.5 的概率在第一回合即可被使用。
0.5 * 0.5 的概率再第二回合被使用。

尝试分析其他的牌。第二张牌：
0.5 * 0.3 的概率在第一回合被使用（没选中第一张牌，且选中第二张牌）
在第二回合被使用呢？首先有 50% 的概率被一号牌抢了先手。另外有概率在第一回合被用过，这一次不能继续使用。
综上所述，第二回合用第二张牌的概率是 (1 - 0.5 - 0.5 * 0.3)。

尝试验证。可以用暴力代码跑一下 N = 2。
如果按照上述算法：
第一张牌贡献的期望是 1.5。
第二张牌贡献的期望是 1.5。
事实上并非如此，答案应该为 2.56 左右（随机撒点模拟的结果）。

上述分析哪里存在问题？可以发现，有概率第一张牌在第一回合被用过了，第二回合可以使用第二张牌的概率就不需要额外要求“第一张牌没有选中”。

重新分析第二张牌：
0.5 * 0.3 的概率在第一回合被使用，这个是没有疑问的。
第二回合被使用：首先有 0.15 的概率已经被用过了。
- 0.5 的概率，第一张牌还在。0.5 * 0.5 * 0.85 * 0.3。
- 0.5 的概率，第一张牌没了。0.5 * 0.85 * 0.3。
差得不多。这么算最后总期望是 2.52。
但是差得好像还是有点多。

第一张和第二张牌被用过，两个事件并不是独立的，直接乘好像没什么道理。
666，我会做 2^n，特别厉害。

题解：F[i][j] 表示所有的 r 轮中，前 i 张牌出了 j 张的概率。
好奇怪的状态，但是它应该是经过精妙设计的。

{
    F[i][j] <- F[i - 1][j]  (a)
    F[i][j] <- F[i - 1][j - 1]  (b)
}
第 i 张牌有可能被选中或者不被选中。如果被选中，执行转移 (b)，否则执行转移 (a)。

只需计算第 i 张牌被选中的概率。在哪个回合被选中不存在本质区别，所以不用体现在状态中。
看起来有道理。
那么思考如何计算第 i 张牌被选中的概率。这个东西真的能算吗？
换一个思路，从 F[i][j] 向接下来的状态外推。考虑第 i - 1 张牌。
有 j 轮操作中，第 i - 1 张牌一定不会被考虑（提前跳过），其他情况下有 (1 - p) 的概率不会被选中。
这种情况下，整张牌完全不被选中的概率 pow(1 - p, r - j)。
*/
namespace Solution_6934306267442359 {
    using f64 = double;
    struct Card {
        f64 p = 0;
        i32 d = 0;
    };
    auto solve_force() -> void {
        // 忍不了。随机撒点模拟。
        std::mt19937 rng{std::random_device{}()};
        auto rand = [&]() -> f64 { return static_cast<f64>(rng()) / rng.max(); };

        i32 N, R; std::cin >> N >> R;
        std::vector<Card> cards(N);
        for (auto &[p, d]: cards) std::cin >> p >> d;

        i32 constexpr iter_cnt = 4e6;
        f64 ans = 0;  // 总伤害的期望
        for (auto _ = iter_cnt; _ --> 0; ) {
            std::vector<char> is_used(N, false);
            for (i32 r = R; r --> 0; ) {
                for (i32 i = 0; i != N; i++) {
                    if (is_used[i]) continue;
                    if (rand() < cards[i].p) {
                        ans += cards[i].d;
                        is_used[i] = true;
                        break;
                    }
                }
            }
        }
        ans /= iter_cnt;
        std::cout << std::fixed << std::setprecision(1) << ans << endl;
    }
    namespace views = std::ranges::views;
    auto solve() -> void {
        i32 N, R; std::cin >> N >> R;
        std::vector<Card> cards(N + 1);
        for (auto &[p, d]: cards | views::drop(1)) std::cin >> p >> d;

        std::vector F(N + 1, std::vector<f64>(N + 1));
        std::vector<f64> P(N + 1);  // 第 i 个卡牌产生贡献的概率
        F[0][0] = 1.0;

        for (i32 i = 0; i < N; i++) {
            for (i32 j = 0; j <= i; j++) {
                if (j > R) break;
                auto pij = 1 - std::pow(1 - cards[i + 1].p, R - j);
                P[i + 1] += F[i][j] * pij;
                F[i + 1][j + 1] += F[i][j] * pij;
                F[i + 1][j] += F[i][j] * (1 - pij);
            }
        }

        f64 ans = 0;
        for (i32 i = 1; i <= N; i++) {
            ans += P[i] * cards[i].d;
        }
        std::cout << std::fixed << std::setprecision(3) << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T; std::cin >> T;
    while (T --> 0) Solution_6934306267442359::solve_force();
    return 0;
}
