/**
 * @link https://www.luogu.com.cn/problem/P3977
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
#include "./libs/debug_log.hpp"

using namespace lib;

/**
 * 先考虑实现一个普通的状压 dp。
 * 一个棋子只有可能打到一行，所以记录一行的状态即可。
 * 
 * 接下来，考虑优化。
 * 每次转移的方式都是较为固定的。一个状态只会由若干个确定的状态转移而来。
 * 可以使用矩阵乘法进行一次转移。
 * 
 * 坏了怎么 TLE 了？
 * 难绷，算转移矩阵的时候跑了 N 次。
 */
namespace Solution_6649813464241949 {
    class Matrix {
        i32 height, width;
        std::vector<std::vector<u32>> data;
    public:
        Matrix(i32 height, i32 width): height(height), width(width), data(height, std::vector<u32>(width)) {}

        auto operator[] (uz idx) const -> std::vector<u32> const & { return data[idx]; }
        auto operator[] (uz idx) -> std::vector<u32> & { return data[idx]; }

        auto operator* (const Matrix &other) const -> Matrix {
            assert(width == other.height);
            Matrix res{height, other.width};

            for (i32 i = 0; i < height; i++) {
                for (i32 k = 0; k < width; k++) {
                    for (i32 j = 0; j < other.width; j++) {
                        res[i][j] += data[i][k] * other.data[k][j];
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
            assert(b >= 0);
            assert(height == width);
            auto a = *this;
            auto res = Matrix::identity(height);
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

        i32 board_height, board_width;  std::cin >> board_height >> board_width;
        i32 constexpr attack_height = 3;
        i32 attack_width;  std::cin >> attack_width;    // 攻击范围的宽度
        i32 attack_origin;  std::cin >> attack_origin;  // 攻击范围的中心位置

        using Stat = std::uint8_t;
        Stat const stat_last = (Stat)1 << board_width;

        std::array<Stat, attack_height> attack_mat;  // 攻击矩阵，可以直接状压表示
        attack_mat.fill(0);
        for (i32 i = 0; i < 3; i++) {
            for (i32 j = 0; j < attack_width; j++) {
                i32 tmp;  std::cin >> tmp;
                attack_mat[i] |= static_cast<Stat>(tmp) << j;
            }
        }

        // F[i][j] 表示前 i 行，最后一行状态为 j 的方案数
        // 把它扔到矩阵里。G[i][0][j] = F[i][j]
        // std::vector G(board_height + 1, Matrix{1, stat_last});

        // 获取列号在 x 位置的攻击矩阵的第 i 行
        auto get_attack_mat_at = [&](i32 x, i32 i) -> Stat {
            auto shift = x - attack_origin;
            auto res = attack_mat[i];
            if (shift < 0) {
                res >>= -shift;
            } else {
                res <<= shift;
            }
            return res & (stat_last - 1);
        };

        Matrix T{stat_last, stat_last};  // 转移矩阵

        // F[0][0] = 1;
        for (Stat j = 0; j < stat_last; j++) {
            // j 能互相打到
            auto j_reach = [&]() -> bool {
                for (i32 k = 0; k < board_width; k++) {
                    if (j & ((Stat)1 << k)) {
                        auto mask = get_attack_mat_at(k, 1);
                        if (mask & (j & ~((Stat)1 << k))) {
                            debug  printValues("j -> j", std::bitset<5>(j));
                            return true;
                        }
                    }
                }
                return false;
            }();
            if (j_reach)  continue;
            for (Stat prev = 0; prev < stat_last; prev++) {
                // 检查 prev 中的元素和 j 中的元素能否互相打到
                auto available = [&]() -> bool {
                    // prev 能打到 j
                    for (i32 k = 0; k < board_width; k++) {
                        if (prev & ((Stat)1 << k)) {
                            auto mask = get_attack_mat_at(k, 2);
                            if (mask & j) {
                                return false;
                            }
                        }
                    }
                    // j 能打到 prev
                    for (i32 k = 0; k < board_width; k++) {
                        if (j & ((Stat)1 << k)) {
                            auto mask = get_attack_mat_at(k, 0);
                            if (mask & prev) {
                                return false;
                            }
                        }
                    }
                    
                    return true;
                }();
                if (available) {
                    T[prev][j] = 1;
                }
            }
        }

        // G[0][0][0] = 1;
        // for (i32 i = 1; i <= board_height; i++) {
        //     G[i] = G[i - 1] * T;
        //     std::cout << "i = " << i << ", mat = \n" << G[i] << std::endl;
        // }

        Matrix G0{1, stat_last};
        G0[0][0] = 1;
        auto GN = G0 * T.qpow(board_height);

        auto ans = std::accumulate(GN[0].begin(), GN[0].end(), (u32)0);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6649813464241949::solve();
    return 0;
}
