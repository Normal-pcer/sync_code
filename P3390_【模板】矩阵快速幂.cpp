/**
 * @link https://www.luogu.com.cn/problem/P3390
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

// 矩阵快速幂 模板
namespace Solution_1089857532959810 {
    i32 constexpr mod = 1e9 + 7;
    auto mul(i32 a, i32 b) -> i32 {
        return static_cast<i64>(a) * b % mod;
    }
    class Matrix {
        i32 height = 0, width = 0;
        std::vector<std::vector<i32>> data;

    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}

        auto operator* (const Matrix &other) const -> Matrix {
            assert(other.height == width);
            Matrix res(height, other.width);

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < other.height; k++) {
                        res.data[i][j] = (res.data[i][j] + mul(data[i][k], other.data[k][j])) % mod;
                    }
                }
            }
            return res;
        }
        auto operator*= (const Matrix &other) -> Matrix & {
            return *this = *this * other;
        }
        auto static identity(i32 N) -> Matrix {
            Matrix res(N, N);
            for (i32 i = 0; i < N; i++) {
                res.data[i][i] = 1;
            }
            return res;
        }

        auto friend qpow(Matrix a, i64 b) -> Matrix {
            assert(a.height == a.width);
            Matrix res = Matrix::identity(a.height);

            for (; b; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }
        auto friend operator>> (std::istream &is, Matrix &mat) -> std::istream & {
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                   is >> mat.data[i][j];
                }
            }
            return is;
        }
        auto friend operator<< (std::ostream &os, Matrix const &mat) -> std::ostream & {
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                    os << mat.data[i][j] << " ";
                }
                os << endl;
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        i64 M;  std::cin >> M;
        Matrix mat{N, N};  std::cin >> mat;
        // std::cout << mat;
        std::cout << qpow(mat, M);
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1089857532959810::solve();
    return 0;
}
