/**
 * @link https://www.luogu.com.cn/problem/P4838
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 状压记录最后的三个数
 * 
 */
namespace Solution_3005915539153714 {
    i32 constexpr mod = 19260817;
    template <typename T>
    using init_list = std::initializer_list<T>;

    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
        Matrix(init_list<init_list<i32>> const &il) {
            height = il.size();
            width = ranges::max(il | views::transform(lam(const &x, x.size())));
            data = std::vector(height, std::vector<i32>(width));

            for (auto [it, i] = std::pair{il.begin(), 0}; it != il.end(); it++, i++) {
                for (auto [it2, j] = std::pair{it->begin(), 0}; it2 != it->end(); it2++, j++) {
                    data[i][j] = *it2;
                }
            }
        }

        auto operator[] (uz idx) const -> std::vector<i32> const & { return data[idx]; }
        auto operator[] (uz idx) -> std::vector<i32> & { return data[idx]; }
        auto operator* (Matrix const &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 j = 0; j < other.width; j++) {
                    for (i32 k = 0; k < width; k++) {
                        res[i][j] = (res[i][j] + static_cast<i64>(data[i][k]) * other.data[k][j] % mod) % mod;
                    }
                }
            }
            return res;
        }

        auto static identity(i32 N) -> Matrix {
            Matrix res{N, N};
            for (i32 i = 0; i < N; i++) {
                res[i][i] = 1;
            }
            return res;
        }

        auto qpow(i64 b) const -> Matrix {
            assert(height == width);
            auto a = *this, res = identity(height);
            for (; b; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }

        auto friend operator<< (std::ostream &os, Matrix const &mat) -> std::ostream & {
            for (i32 i = 0; i < mat.height; i++) {
                if (i != 0)  os << endl;
                for (i32 j = 0; j < mat.width; j++) {
                    if (j != 0)  os << " ";
                    os << mat[i][j];
                }
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        Matrix T{8, 8};
        for (u32 i = 0; i < 8; i++) {
            auto common = i & 0b11;
            T[i][common << 1] = 1;
            if (common != 0b11)  T[i][common << 1 | 1] = 1;
        }

        Matrix A{{1, 1, 1, 1, 1, 1, 1, 0}};

        i32 M;  std::cin >> M;
        for (auto m = M; m --> 0; ) {
            i32 N;  std::cin >> N;
            if (N == 1)  std::cout << 2 << endl;
            else if (N == 2)  std::cout << 4 << endl;
            else if (N == 3)  std::cout << 7 << endl;
            else {
                auto ans_mat = A * T.qpow(N - 3);
                // auto ans_mat = A;
                // for (i32 i = 0; i < N; i++) {
                //     ans_mat = ans_mat * T;
                //     std::cout << "i = " << i << ", ans_mat = " << std::endl;
                //     std::cout << ans_mat << std::endl;
                // }
                i32 ans = 0;
                for (auto x: ans_mat[0])  ans = (ans + x) % mod;
                std::cout << ans << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3005915539153714::solve();
    return 0;
} 
