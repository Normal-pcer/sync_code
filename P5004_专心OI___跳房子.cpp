/**
 * @link https://www.luogu.com.cn/problem/P5004
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

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
