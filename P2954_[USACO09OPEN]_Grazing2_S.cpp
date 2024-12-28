/**
 * @link https://www.luogu.com.cn/problem/P2954
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_2455971966479088 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, S;  std::cin >> N >> S;  // 位置 [1, S+1)
        std::vector<int> p(N);  // 牛从 0 开始编号
        for (auto &i: p)  std::cin >> i;
        ranges::sort(p);
        
        const auto d = (S - 1) / (N - 1);
        // F[i][j]：[0, i] 中的牛，j 段距离为 d，最右侧位置为 k 时的最小总位移
        // k 可以被计算：k = 1 + d * j + (d + 1) * (i - j)
        std::vector F(N, std::vector(N, inf));
        F[0][0] = std::abs(1 - p[0]);
        for (auto i: range(1, N)) {
            for (auto j: range(i+1)) {
                const auto k = 1 + d * j + (d + 1) * (i - j);
                if (j != 0) {
                    chkMin(F[i][j], F[i-1][j-1] + std::abs(k - p[i]));
                }
                chkMin(F[i][j], F[i-1][j] + std::abs(k - p[i]));
            }
        }
        debug {
            for (auto i: range(1, N)) {
                for (auto j: range(i+1)) {
                    if (F[i][j] >= inf)  continue;
                    std::cout << std::format("F[{}][{}] = {}", i, j, F[i][j]) << std::endl;
                }
            }
        }
        auto ans = inf;
        for (auto j: range(N) | views::reverse) {
            const auto i = N-1, k = 1 + d * j + (d + 1) * (i - j);
            if (k != S)  continue;
            if (F[i][j] < inf)  chkMin(ans, F[i][j]);
            if (ans < inf)  break;
            ans = inf;
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2455971966479088::solve();
    return 0;
}