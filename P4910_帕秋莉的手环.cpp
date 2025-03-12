/**
 * @link https://www.luogu.com.cn/problem/P4910
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 如果两个珠子中有一个 1，那么之间的连线就为 1。
 * 求有多少种方案使得 N 个珠子所成的环之间的所有连线都为 1。
 * 
 * 令 F[i][j][k] 表示 i 个珠子，开头颜色为 j，结尾颜色为 k 的方案数。（j, k = 0/1）
 * 转移：F[i][j][1] <- F[i - 1][j][0] + F[i - 1][j][1]
 * F[i][j][0] <- F[i - 1][j][1]
 * 
 * 显然可以使用矩阵乘法优化。另外，本题的答案矩阵可以为二维的，类似：
 * {
 * {F[i][0][0], F[i][0][1]},
 * {F[i][1][0], F[i][1][1]}
 * }
 * 
 * 这个矩阵乘以转移矩阵也是相当于共同转移一次。
 */
namespace Solution_2762622120976528 {
    i32 constexpr mod = 1'000'000'007;
    template <typename T>
    using ilist = std::initializer_list<T>;
    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
        Matrix(ilist<ilist<i32>> const &il) {
            height = il.size();
            width = ranges::max(il | views::transform(lam(x, x.size())));
            data = std::vector(height, std::vector<i32>(width));

            for (auto [i, it0] = std::pair{0, il.begin()}; it0 != il.end(); it0++, i++) {
                for (auto [j, it1] = std::pair{0, it0->begin()}; it1 != it0->end(); it1++, j++) {
                    data[i][j] = *it1;
                }
            }
        }
        auto operator[] (uz idx) -> auto & { return data[idx]; }
        auto operator[] (uz idx) const -> auto const & { return data[idx]; }

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
            assert(b >= 0);
            auto a = *this;
            auto res = identity(height);

            for (; b != 0; b >>= 1, a = a * a){
                if (b & 1)  res = res * a;
            }
            return res;
        }

        auto friend operator<< (std::ostream &os, Matrix const &mat) -> auto & {
            uz max_len = 0;
            for (i32 i = 0; i < mat.height; i++) {
                for (i32 j = 0; j < mat.width; j++) {
                    chkMax(max_len, std::format("{}", mat[i][j]).length());
                }
            }

            for (i32 i = 0; i < mat.height; i++) {
                if (i != 0)  os << endl;
                for (i32 j = 0; j < mat.width; j++) {
                    os << std::setw(max_len + 1) << mat[i][j];
                }
            }
            return os;
        }
    };
    void solve() {
        i64 N;  std::cin >> N;
        
        Matrix T {
            { 0,  1},
            { 1,  1},
        };

        Matrix A1 {
            { 1,  0},
            { 0,  1},
        };

        auto AN = A1 * T.qpow(N - 1);
        // auto AN = A1;
        // for (i32 i = 1; i < N; i++) {
            // std::cout << "A[" << i << "] = \n" << AN << endl;
            // AN = AN * T;
        // }
        // std::cout << "A[" << N << "] = \n" << AN << endl;
        i32 ans = 0;
        for (auto x: {AN[0][1], AN[1][0], AN[1][1]}) {
            ans = (ans + x) % mod;
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T;  std::cin >> T;
    while (T --> 0) {
        Solution_2762622120976528::solve();
    }
    return 0;
}
