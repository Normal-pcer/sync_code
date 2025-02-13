/**
 * @link https://www.luogu.com.cn/problem/AT_abc275_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

/**
 * 令 F[i][j] 表示前 i 个数，希望最终和为 j，最少删除多少个连续段。
 * 转移：考虑最后删除一个长为 k 的连续段。
 * F[i][j] <- F[i-k][j] + 1
 * F[i][j] <- F[i-1][j-val]
 * 
 * 直接转移是 O(N * M * N)，不行。
 * 想要取 F[i-k][j] 的最小值，i-k 的范围是 [0, i)。
 * 随着 i 的增长，更新对于每个 j 的最小值即可。
 */
namespace Solution_5559037506637451 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<int> a(N+1);
        for (auto &x: a | views::drop(1))  std::cin >> x;  // 从 1 开始编号

        std::vector F(N+1, std::vector(M+1, inf));
        std::vector min(M+1, inf);  // min[j]：截至当前枚举的 i，F[k][j] 的最小值
        F[0][0] = min[0] = 0;
        for (auto i = 1; i <= N; i++) {
            for (auto j = 0; j <= M; j++) {
                chkMin(F[i][j], min[j] + 1);
                if (j - a[i] >= 0) {
                    chkMin(F[i][j], F[i-1][j - a[i]]);
                }
                chkMin(min[j], F[i][j]);
            }
        }

        for (auto q = 1; q <= M; q++) {
            debug  std::cout << "q = " << q << ": ";
            auto ans = F[N][q];
            if (ans == inf)  ans = -1;
            std::cout << ans << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5559037506637451::solve();
    return 0;
}
