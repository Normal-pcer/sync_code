/**
 * @link https://www.luogu.com.cn/problem/P3758
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6409440191914101 {
    i32 constexpr mod = 2017;
    auto constexpr mul(i32 a, i32 b) -> i32 {
        return a * b % mod;
    }
    template <typename T>
    using ilist = std::initializer_list<T>;

    class Matrix {
    public:
        i32 height = 0, width = 0;
        std::vector<std::vector<i32>> data;

        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
        Matrix(ilist<ilist<i32>> init) {
            height = init.size();
            width = ranges::max(init | views::transform(lam(x, x.size())));

            data = std::vector(height, std::vector<i32>(width));

            i32 i = 0;
            for (auto it = init.begin(); it != init.end(); it++, i++) {
                i32 j = 0;
                for (auto it2 = it->begin(); it2 != it->end(); it2++, j++) {
                    data[i][j] = *it2;
                }
            }
        }

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
        
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6409440191914101::solve();
    return 0;
}
