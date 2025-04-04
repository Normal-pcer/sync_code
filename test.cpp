/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6314180276493067 {
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        using Stat = u32;
        auto blocks = n * m;

        auto statCount = (Stat)1 << blocks;
        i32 ans = 0x3f3f3f3f;
        for (Stat s = 0; s != statCount; s++) {
            auto walk = [&]() -> bool {
                std::vector mat(n, std::vector<char>(m));
                for (i32 i = 0; i < n; i++) {
                    for (i32 j = 0; j < m; j++) {
                        mat[i][j] = static_cast<bool>(s & ((Stat)1 << (i * m + j)));
                    }
                }
                std::array<i32, 4> constexpr dxs{ +1, -1,  0,  0 };
                std::array<i32, 4> constexpr dys{  0,  0, +1, -1 };
                for (auto _ = blocks; _ --> 0; ) {
                    auto getAt = [&](i32 i, i32 j) -> bool {
                        if (i < 0 or i >= n) return false;
                        if (j < 0 or j >= m) return false;
                        return mat[i][j];
                    };

                    for (i32 i = 0; i < n; i++) {
                        for (i32 j = 0; j < m; j++) {
                            if (mat[i][j]) continue;
                            auto cnt = 0;
                            for (i32 dir = 0; dir != 4; dir++) {
                                cnt += getAt(i + dxs[dir], j + dys[dir]);
                            }
                            if (cnt >= 2) mat[i][j] = true;
                        }
                    }
                }

                for (i32 i = 0; i < n; i++) {
                    for (i32 j = 0; j < m; j++) {
                        if (not mat[i][j]) return false;
                    }
                }
                return true;
            };
            if (walk()) {
                chkMin(ans, std::popcount(s));
            }
        }
        std::cout << ans << endl;
        assert(ans == n / 2 + m / 2 + 1 - (n % 2 == 0 and m % 2 == 0));
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_6314180276493067::solve();
    return 0;
}
