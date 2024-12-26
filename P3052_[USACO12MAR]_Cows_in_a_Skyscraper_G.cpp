/**
 * @link https://www.luogu.com.cn/problem/P3052
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_7839012423031640 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, W;  std::cin >> N >> W;
        std::vector<int> size(N);
        for (auto &item: size)  std::cin >> item;

        std::vector F(N, std::vector(1 << N, inf));  // F[i][j]: 分为 i + 1 个区域，状态为 j
        F[0][0] = 0;

        for (auto i: range(N)) {
            for (auto j: range(1 << N)) {  // 当前状态
                for (auto k: range(N)) {  // 添加一个
                    if (j & (1 << k))  continue;
                    auto new_st = j | (1 << k);
                    auto new_w = F[i][j] + size[k];
                    if (new_w <= W) {
                        chkMin(F[i][new_st], new_w);
                    } else if (F[i][j] <= W and i + 1 < N) {
                        chkMin(F[i+1][new_st], size[k]);
                    }
                }
            }
        }
        

        auto ans = N;
        for (auto i: range(N)) {
            if (F[i][(1 << N) - 1] <= W) {
                ans = i;
                break;
            }
        }
        std::cout << ans + 1 << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7839012423031640::solve();
    return 0;
}
