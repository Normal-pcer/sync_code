/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "palindorme"

/*
F[i][j] 表示把区间 [i, j] 删成回文串，可能的答案数量。
*/
namespace Solution_6138187558929015 {
    auto solve() -> void {
        std::string s;
        std::cin >> s;

        auto len = static_cast<i32>(s.size());
        
        using Count = i64;
        std::vector<std::vector<Count>> F(len, std::vector<Count>(len));
        for (i32 i = 0; i < len - 1; i++) {
            F[i + 1][i] = 1;
        }
        for (i32 i = 0; i < len; i++) {
            F[i][i] = 2;
        }
        for (i32 segLen = 2; segLen <= len; segLen++) {
            for (i32 i = 0; i < len; i++) {
                auto j = i + segLen - 1;
                if (j >= len) break;
                F[i][j] += F[i][j - 1];
                F[i][j] += F[i + 1][j];
                F[i][j] -= F[i + 1][j - 1];  // 去重
                if (s[i] == s[j]) {
                    F[i][j] += F[i + 1][j - 1];
                }
            }
        }

        debug for (i32 i = 0; i < len; i++) {
            for (i32 j = 0; j < len; j++) {
                std::cout << std::format("F[{}][{}] = {}", i, j, F[i][j]) << std::endl;
            }
        }

        auto ans = F[0][len - 1];
        std::cout << ans - 1 << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 t;
    std::cin >> t;
    while (t --> 0) {
        Solution_6138187558929015::solve();
    }
    return 0;
}
