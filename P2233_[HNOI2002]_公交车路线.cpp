/**
 * @link https://www.luogu.com.cn/problem/P2233
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 设 F[i][j] 表示走 i 步后位于 j 点的方案数
 * F[i][j] <- F[i - 1][(j - 1) mod 8] + F[i - 1][(j + 1) mod 8]
 */
namespace Solution_9408623968424637 {
    i32 constexpr mod = 1000;
    template <typename T>
    using ilist = std::initializer_list<T>;
    class Matrix {
        i32 height, width;
        std::vector<std::vector<i32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<i32>(width)) {}
        Matrix(ilist<ilist<i32>> &il) {
            height = il.size();
            width = ranges::max(il | views::transform(lam(x, x.size())));

            data = std::vector(height, std::vector<i32>(width));
            for (auto [i, it] = std::pair{0, il.begin()}; it != il.end(); it++, i++) {
                for (auto [j, it2] = std::pair{0, it->begin()}; it2 != it->end(); it2++, j++) {
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
                for (i32 k = 0; k < width; k++) {
                    for (i32 j = 0; j < other.width; j++) {
                        res[i][j] = (res[i][j] + data[i][k] * other.data[k][j] % mod) % mod;
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

            for (; b != 0; b >>= 1, a = a * a) {
                if (b & 1)  res = res * a;
            }
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        // 用 G[i][0][j] 表示 F[i][j]
        Matrix G0{1, 8};
        G0[0][0] = 1;

        Matrix T{8, 8};
        for (i32 i = 0; i < 8; i++) {
            T[(i + 1) % 8][i] = 1;
            T[(i + 7) % 8][i] = 1;
        }
        ranges::fill(T[4], 0);  // 从 4 不能再向外走

        auto GN = G0 * T.qpow(N);
        auto ans = GN[0][4];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9408623968424637::solve();
    return 0;
}
