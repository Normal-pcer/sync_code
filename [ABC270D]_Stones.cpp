/**
 * @link https://www.luogu.com.cn/problem/AT_abc270_d
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

using i16 = int16_t; using i32 = int32_t; using i64 = int64_t;
using u16 = uint16_t; using u32 = uint32_t; using u64 = uint64_t; using uz = size_t;

/**
 * 一个人面对 N 个石子，尝试取出 a[i] 个
 * 接下来由另一个人面对 N - a[i] 个石子
 * 
 * 一种情况下，当前玩家可以获得的最多石子数量似乎是固定的
 * 令 F[i] 表示面对 N 个石子时，当前玩家和对方能获得的石子数量
 * 如果 i 在 a[i] 中，显然应该选择直接全拿走
 * 否则，枚举这次拿走多少个取最大值
 */
namespace Solution_1574030697499224 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, K;  std::cin >> N >> K;
        std::vector<i32> a(K);
        for (auto &x: a)  std::cin >> x;

        std::vector<std::pair<i32, i32>> F(N+1);
        for (auto x: a)  F[x] = {x, 0};  // 自己全拿走，对方没有

        for (i32 i = 1; i <= N; i++) {
            for (auto x: a) {
                if (i - x >= 0) {
                    auto [enemy, self] = F[i - x];
                    std::pair cur{self + x, enemy};
                    chkMax(F[i], cur);
                }
            }
        }

        auto [ans, _] = F[N];
        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1574030697499224::solve();
    return 0;
}
