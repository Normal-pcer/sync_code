/**
 * @link https://www.luogu.com.cn/problem/P3803
 */
#include <bits/stdc++.h>
#include <iostream>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define _lambda_1(expr) [&]() { return expr; }
#define _lambda_2(a, expr) [&](auto a) { return expr; }
#define _lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define _lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define _lambda_overload(a, b, c, d, e, ...) _lambda_##e
#define lambda(...) _lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
    namespace views = std::views;
#endif
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_1106704229505844 {
    using f64 = double;

    struct Poly {
        using Num = std::complex<f64>;

        std::vector<Num> coeff;  // 系数表示

        Poly() = default;

        template <typename T>
        Poly(std::vector<T> const &range) {
            coeff.resize(range.size());
            for (size_t i = 0; i < range.size(); ++i) {
                coeff[i] = Num(range[i]);
            }
        }

        auto calc(Num x) const -> Num {
            Num res{0};
            for (i32 i = 0; i < static_cast<i32>(coeff.size()); ++i) {
                res += coeff[i] * std::pow(x, i);
            }
            return res;
        }

        auto padding(uz least = 0) -> void {  // 补零
            least = std::max(least, coeff.size());
            auto size = std::bit_ceil(least);
            coeff.resize(size);
        }

        // 令 w 为复数单位根
        // 获取 w^0, ..., w^N 上的所有点值
        // 暴力实现
        auto getPointsExprTest() const -> std::vector<Num> {
            auto n = static_cast<i32>(coeff.size());
            std::vector<Num> res(n);

            auto rad = std::numbers::pi * 2 / n;
            auto w = std::polar(1.0, rad);

            Num x{1};
            for (i32 i = 0; i < n; ++i) {
                res[i] = calc(x);
                x *= w;
            }
            return res;
        }

        // FFT
        auto getPointsExprRecursion(std::vector<Num> const &a, Num w) const -> std::vector<Num> {
            auto n = static_cast<i32>(a.size());

            // 递归边界，仅剩下常数项
            if (n == 1) return {a[0]};

            // 分为奇偶系数
            assert(n % 2 == 0);
            auto half_n = n / 2;
            std::vector<Num> even, odd;

            even.reserve(half_n), odd.reserve(half_n);

            for (i32 i = 0; i < n; i += 2) {
                even.push_back(a[i]);
                odd.push_back(a[i + 1]);
            }

            // 分别计算，然后合并
            auto e = getPointsExprRecursion(even, w * w);
            auto o = getPointsExprRecursion(odd, w * w);

            // 以上的操作从本质上讲，是分离奇偶项，然后计算值：
            // x^2 = w0, w2, w4, ...
            // x = w0, w1, w2, ... 及其相反数
            std::vector<Num> res(n);
            Num x{1};
            for (i32 i = 0; i < half_n; ++i) {
                // 原数：even + odd
                // 相反数：even - odd（由于奇偶性）
                res[i] = e[i] + x * o[i];  // odd 刚刚少乘一个 x，现在乘回来
                res[i + half_n] = e[i] - x * o[i];
                x *= w;
            }

            return res;
        }

        auto getPointsExpr() const -> std::vector<Num> {
            auto rad = std::numbers::pi * 2 / static_cast<f64>(coeff.size());
            auto w = std::polar(1.0, rad);
            return getPointsExprRecursion(coeff, w);
        }

        auto fromPointsExpr(const std::vector<Num>& points) -> void {
            auto n = static_cast<f64>(points.size());
            auto inv_n = 1 / n;
            auto rad = std::numbers::pi * 2 * inv_n;
            auto w = std::polar(1.0, -rad);
            coeff = getPointsExprRecursion(points, w);
            for (auto &x: coeff) x *= inv_n;
        }

        auto mulDigits(Poly const &other) const -> i32 {
            return static_cast<i32>(coeff.size() + other.coeff.size() - 1);
        }

        auto multiply(Poly &other) -> Poly {
            assert(this != &other);
            auto n = mulDigits(other);
            padding(n);
            other.padding(n);

            auto pts1 = getPointsExpr();
            auto pts2 = other.getPointsExpr();

            auto N = static_cast<i32>(pts1.size());

            std::vector<Num> pts(N);
            for (i32 i = 0; i < N; i++) pts[i] = pts1[i] * pts2[i];

            Poly res{};
            res.fromPointsExpr(pts);
            return res;
        }
    };
    auto solve() -> void {
        i32 n, m; std::cin >> n >> m;
        ++n, ++m;
        std::vector<i32> a(n), b(m);
        for (auto &x: a) std::cin >> x;
        for (auto &x: b) std::cin >> x;

        Poly pl1(a), pl2(b);
        auto digits = pl1.mulDigits(pl2);
        auto res = pl1.multiply(pl2);

        for (auto x: res.coeff | views::take(digits)) {
            std::cout << static_cast<i32>(x.real() + 0.5) << ' ';
        }
        std::cout << '\n';
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_1106704229505844::solve();
    return 0;
}
