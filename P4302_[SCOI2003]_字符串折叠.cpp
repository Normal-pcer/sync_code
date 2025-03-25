/**
 * @link https://www.luogu.com.cn/problem/P4302
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
看起来很像区间 DP。
记 F[i][j] 表示闭区间 [i, j] 压缩后的最短长度。
反正 N <= 100，乱搞都可以。
预处理出一个区间可以如何分成若干个相同字符串的重复。
*/
namespace Solution_5414410984279125 {
    auto constexpr numLen(i32 x) -> i32 {
        return 1 
            + (x >= 10) 
            + (x >= 100)
            + (x >= 1000)
            + (x >= 10000) 
            + (x >= 100000) 
            + (x >= 1000000)
            + (x >= 10000000)
            + (x >= 100000000)
            + (x >= 1000000000);
    }
    auto solve() -> void {
        std::string s;
        std::cin >> s;

        auto len = static_cast<i32>(s.size());
        std::vector F(len, std::vector<i32>(len + 1, inf));  // [i, j) 压缩后最短长度

        using i8 = std::int8_t;
        // F[i][j][k] 表示 [i, j) 区间的循环节是否可以为 k；如果是，需要循环多少次。如果不是，则为 0.
        std::vector rep(len, std::vector(len + 1, std::vector<i8>(len + 1, false)));

        for (i32 i = 0; i < len; i++) {
            for (i32 j = i + 1; j <= len; j++) {
                auto const this_len = j - i;
                for (i32 k = 1; k <= this_len; k++) {
                    if (this_len % k == 0) {
                        rep[i][j][k] = [&]() -> i8 {
                            auto const first_it = s.begin() + i, last_it = s.begin() + j;
                            for (auto it = first_it; it != last_it; it += k) {
                                if (std::lexicographical_compare_three_way(
                                    first_it, first_it + k,
                                    it, it + k
                                ) != 0) {
                                    return 0;
                                }
                            }
                            return this_len / k;
                        }();
                    }
                }
            }
        }

        for (i32 l = 1; l <= len; l++) {
            for (i32 i = 0; i < len; i++) {
                auto j = i + l;
                if (j > len) break;

                chkMin(F[i][j], l);  // 完全不压缩
                // 能不能一整个压到一起
                for (i32 part_len = 1; part_len <= l; part_len++) {
                    auto part_cnt = rep[i][j][part_len];
                    if (part_cnt != 0) {
                        chkMin(F[i][j], F[i][i + part_len] + 2 + numLen(part_cnt));
                    }
                }

                // 枚举中间点，直接拼接两段
                for (i32 mid = i + 1; mid < j; mid++) {
                    chkMin(F[i][j], F[i][mid] + F[mid][j]);
                }
                std::cout << std::format("F[{}][{}] = {}", i, j, F[i][j]) << std::endl;
            }
        }
        auto ans = F[0][len];
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_5414410984279125::solve();
    return 0;
}
