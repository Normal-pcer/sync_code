/**
 * @link https://www.luogu.com.cn/problem/P1707
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 推式子。
 * 致敬传奇 11*11 超级大矩阵。
 */
namespace Solution_2881120974217506 {
    i64 mod = 1e9 + 7;
    using i128 = __int128;
    template <typename T>  using ilist = std::initializer_list<T>;
    auto mul(i64 a, i64 b) -> i64 {
        return static_cast<i128>(a) * b % mod;
    }
    class Matrix {
    public:
        i32 height = 0, width = 0;
        std::vector<std::vector<i64>> data;

        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i64>(width)) {}
        Matrix(ilist<ilist<i64>> init) {
            height = init.size();
            width = ranges::max(init | views::transform(lam(x, x.size())));

            data = std::vector(height, std::vector<i64>(width));

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
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i64 n, m;
        i32 p, q, r, t, u, v, w, x, y, z;
        std::cin >> n >> m >> p >> q >> r >> t >> u >> v >> w >> x >> y >> z;
        mod = m;
        Matrix T {
            {p, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0}, 
            {q, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, u, 1, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, v, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 1, 0, x, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, y, 0, 0, 0, 0, 0, 0},
            {r, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {t, 0, 0, 0, 1, 0, 2, 1, 0, 0, 0},
            {0, 0, 1, 0, 0, 0, 0, 0, w, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, z, 0}, 
            {1, 0, 0, 0, 2, 0, 1, 1, 0, 0, 1}, 
        };

        Matrix A1 {
            {3, 1, 3, 1, 3, 1, 1, 1, w, z, 1}
        };

        auto An = A1 * qpow(T, n - 2);
        auto ans = std::vector{An.data[0][0], An.data[0][2], An.data[0][4]};
        std::vector names{"nodgd", "Ciocio", "Nicole"};

        for (i32 i = 0; i < 3; i++) {
            std::cout << names[i] << " " << ans[i] << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2881120974217506::solve();
    return 0;
}
