/**
 * @link https://www.luogu.com.cn/problem/AT_abc249_e
 */
#include "libs/debug_macros.hpp"
#include "lib"
using namespace lib;

/**
 * F[i][j] 表示将长度为 i 的字符串压到长度为 j 的方案数
 * 考虑转移
 * 在一个字符串后面加一个长度为 k 的连续段，
 * 原字符串长度 i - k -> i
 * 新字符串长度 j - 1 - numLen(k) -> j
 * F[i][j] <= 25 * F[i-k][j-1-len(k)]
 * 分别对于 len(k) = 1, 2, 3, 4 的情况，j - 1 - len(k) 可以看成定值
 * 先枚举 j，再枚举 i，用前缀和维护即可
 * 令 G[i][j] 表示 F[j][i]
 */
namespace Solution_4594102107456167 {
    auto numLen(int x) -> int {
        return 1 + (x >= 10) + (x >= 100) + (x >= 1000);
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, P;  std::cin >> N >> P;

        std::vector G(N+1, std::vector<int>(N+1));  // G[j][i]：长度为 i -> j
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4594102107456167::solve();
    return 0;
}
