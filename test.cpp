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
 */
namespace Solution_6649813464241949 {
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
        std::vector F(board_height + 1, std::vector<u32>(stat_last));
        
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

        F[0][0] = 1;
        for (i32 i = 1; i <= board_height; i++) {
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
                        F[i][j] += F[i - 1][prev];
                    }
                }
            }
        }

        for (i32 i = 1; i <= board_height; i++) {
            for (Stat j = 0; j < stat_last; j++) {
                std::cout << F[i][j] << " ";
            }
            std::cout << std::endl;
        }

        auto ans = std::accumulate(F[board_height].begin(), F[board_height].end(), (u32)0);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6649813464241949::solve();
    return 0;
}
