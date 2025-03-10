/**
 * @link https://www.luogu.com.cn/problem/P5678
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
 * 如果把矩阵乘法的乘法换成 and，加法换成 or 会怎样？
 * 合理。
 * 
 * 
 */
namespace Solution_8220893977154469 {
    class Matrix {
        i32 height, width;
        std::vector<std::vector<u64>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<u64>(width)) {}

        auto operator[] (uz idx) -> std::vector<u64> & { return data[idx]; }
        auto operator[] (uz idx) const -> std::vector<u64> const & { return data[idx]; }
        
        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res[i][j] |= data[i][k] & other.data[k][j];
                    }
                }
            }
            return res;
        }
        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res[i][i] = ~(u64)0;
            }
            return res;
        }
        auto qpow(i64 b) -> Matrix {
            assert(height == width);
            auto res = identity(height), a = *this;
            for (; b != 0; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }
        auto friend operator<< (std::ostream &os, Matrix const &mat) -> std::ostream & {
            for (i32 i = 0; i < mat.height; i++) {
                if (i != 0)  os << endl;
                for (i32 j = 0; j < mat.width; j++) {
                    if (j != 0)  os << " ";
                    os << std::bitset<8>(mat[i][j]);
                }
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, K;  std::cin >> N >> K;
        std::vector<i64> a(K), b(K);
        for (auto &x: a)  std::cin >> x;
        for (auto &x: b)  std::cin >> x;

        // [A[K-1], ..., A[0]] 为初始的答案矩阵
        // 构建转移矩阵
        Matrix T{K, K};

        // 按照题意，第一列应该是按位与 b 的对应项
        for (i32 i = 0; i < K; i++) {
            T[i][0] = b[K - i - 1];
        }
        // 其他的直接转移
        for (i32 i = 1; i < K; i++) {
            T[i - 1][i] = ~(u64)0;
        }

        Matrix M{1, K};
        for (i32 i = 0; i < K; i++) {
            M[0][K - 1 - i] = a[i];
        }

        if (N < K) {
            std::cout << a[N] << endl;
            return;
        }

        // for (auto _ = N - K + 1; _ --> 0; ) {
            // std::cout << M << endl;
            // M = M * T;
        // }
        M = M * T.qpow(N - K + 1);
        // std::cout << M << endl;
        auto ans = M[0][0];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8220893977154469::solve();
    return 0;
}
