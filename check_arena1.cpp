/**
 * @link
 */
#include <algorithm>
#include <bits/stdc++.h>
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
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6314180276493067 {
    i32 constexpr inf = 0x3f3f3f3f;
    struct Matrix {
        i32 height{}, width{};
        std::vector<std::vector<i32>> data;

        Matrix() = default;
        Matrix(i32 h, i32 w): height(h), width(w), data(h, std::vector<i32>(w, -inf)) {}

        auto operator[] (std::size_t index) -> auto & {
            return data[index];
        }

        // 广义矩阵乘法
        // 两个操作替换为加法和 max
        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; ++i) {
                for (i32 k = 0; k < width; ++k) {
                    for (i32 j = 0; j < other.width; ++j) {
                        if (data[i][k] == -inf or other.data[k][j] == -inf) continue;
                        chkMax(res[i][j], data[i][k] + other.data[k][j]);
                    }
                }
            }
            return res;
        }

        auto static identifier(i32 n) -> Matrix {
            Matrix res{n, n};
            for (i32 i = 0; i < n; ++i) {
                res[i][i] = 0;
            }
            return res;
        }

        // 矩阵快速幂
        auto pow(i32 b) -> Matrix {
            assert(height == width);
            auto a = *this;
            auto res = identifier(height);
            for (; b != 0; b >>= 1, a = a * a) {
                if ((b & 1) == 1) res = res * a;
            }
            return res;
        }
    };
    auto solve() -> void {
        i32 n{}, t{};
        std::cin >> n >> t;

        std::vector<i32> a(n);  // 循环节
        for (auto &x: a) std::cin >> x;

        // 离散化
        auto sorted = a;
        {
            std::sort(sorted.begin(), sorted.end());
            sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

            auto getIndex = [&](i32 x) -> i32 {
                auto it = std::lower_bound(sorted.begin(), sorted.end(), x);
                assert(it != sorted.end() and *it == x);
                return static_cast<i32>(it - sorted.begin());
            };
            for (auto &x: a) x = getIndex(x);
        }
        auto vals = static_cast<i32>(sorted.size());

        // F[p][i][j] 表示，单个循环节中，以数字 i 开头、数字 j 结尾的 LIS 长度，当前考虑到第 p 个数
        std::vector<std::vector<std::vector<i32>>> F(n, std::vector<std::vector<i32>>(vals, 
            std::vector<i32>(vals, -inf)));
        for (i32 i = 0; i < vals; ++i) F[0][i][i] = 0;
        F[0][a[0]][a[0]] = 1;

        for (i32 p = 1; p < n; ++p) {
            for (i32 i = 0; i <= a[p]; ++i) {
                for (i32 j = i; j <= a[p]; ++j) {
                    chkMax(F[p][i][a[p]], F[p - 1][i][j] + 1);
                }
            }
            for (i32 i = 0; i < vals; ++i) {
                for (i32 j = i; j < vals; ++j) {
                    chkMax(F[p][i][j], F[p - 1][i][j]);
                }
            }
        }

        // 调试：输出 F[n-1]
        debug {
            std::cout << "F[n-1]: \n";
            for (i32 i = 0; i < vals; ++i) {
                for (i32 j = 0; j < vals; ++j) {
                    std::cout << F[n - 1][i][j] << ' ';
                }
                std::cout << endl;
            }
        }

        // G[i][j] 表示，单个循环节中，开头大于等于数字 i、数字 j 结尾的 LIS 长度
        std::vector<std::vector<i32>> G(vals, std::vector<i32>(vals, -inf));
        for (i32 i = 0; i < vals; ++i) {
            for (i32 k = i; k < vals; ++k) {
                for (i32 j = 0; j < vals; ++j) {
                    chkMax(G[i][j], F[n - 1][k][j]);
                }
            }
        }

        Matrix mat{vals, vals};
        mat.data = G;

        // 当前的 mat 是一个循环节的结果，转移方式类似邻接矩阵，所以可以矩阵快速幂
        mat = mat.pow(t);
        // auto tmp = mat;
        // for (auto _ = t - 1; _ --> 0; ) {
        //     mat = mat * tmp;
        //     debug {
        //         std::cout << "---\n";
        //         for (i32 i = 0; i < vals; ++i) {
        //             for (i32 j = 0; j < vals; ++j) {
        //                 if (mat[i][j] < 0) std::cout << "- ";
        //                 else std::cout << mat[i][j] << ' ';
        //             }
        //             std::cout << endl;
        //         }
        //     }
        // }

        auto ans = *std::max_element(mat[0].begin(), mat[0].end());
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6314180276493067::solve();
    return 0;
}
