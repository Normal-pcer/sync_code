/**
 * @link https://www.luogu.com.cn/problem/P5004
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
 * F[i] 表示跳到格子 i 的方案数
 * F[i] = F[1] + F[2] + ... + F[i - M - 1]
 * 
 * ps[i] 表示 F[i] 的前缀和
 * F[i + 1] = ps[i - M] = F[i] + F[i - M]
 * 
 * 定义 A[i] 矩阵：[F[i], F[i-1], F[i-2], F[i-3], F[i-4]]（假设 M = 4）
 * B 矩阵：
 * 1 1 0 0 0
 * 0 0 1 0 0 
 * 0 0 0 1 0
 * 0 0 0 0 1
 * 1 0 0 0 0
 * A[i] * B（矩阵乘法）即 A[i+1]
 * 
 */
namespace Solution_3912497635112256 {
    i32 constexpr mod = 1e9 + 7;
    auto constexpr mul(i32 a, i32 b) -> i32 {
        return static_cast<i64>(a) * b % mod;
    }
    struct Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
    
        auto operator* (Matrix const &other) -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};
            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res.data[i][j] = (res.data[i][j] + mul(data[i][k], other.data[k][j])) % mod;
                    }
                }
            }
            return res;
        }
        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res.data[i][i] = 1;
            }
            return res;
        }
        auto friend qpow(Matrix a, i64 b) -> Matrix {
            assert(a.width == a.height);
            auto res = Matrix::identity(a.height);

            for (; b; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }
        auto friend operator>> (std::istream &is, Matrix &mat) {
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                    is >> mat.data[i][j];
                }
            }
        }
        auto friend operator<< (std::ostream &os, Matrix const &mat) {
            for (i32 i = 0; i < mat.height; i++) {
                if (i != 0)  os << endl;
                for (i32 j = 0; j < mat.width; j++) {
                    if (j != 0)  os << " ";
                    os << mat.data[i][j];
                }
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i64 N;  i32 M;
        std::cin >> N >> M;
        if (N <= M + 1)  return std::cout << N + 1 << endl, void();
        Matrix B{M + 1, M + 1};
        B.data[0][0] = B.data[M][0] = 1;
        for (i32 i = 0; i < M; i++) {
            B.data[i][i + 1] = 1;
        }
        
        Matrix A{1, M + 1};  // A[M + 1]
        for (i32 i = 0; i < M + 1; i++) {
            A.data[0][i] = M + 2 - i;
        }

        auto exp = N - (M + 1);
        A = A * qpow(B, exp);
        auto ans = A.data[0][0];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3912497635112256::solve();
    return 0;
}
