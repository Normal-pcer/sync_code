/**
 * @link https://www.luogu.com.cn/problem/P9759
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 一条路径上的乘积是 k 的倍数，要求这条路径能集齐 k 的所有质因子
 * 考虑 F[i][j][...] 表示走到点 (i, j) 时质因子数量满足条件的方案数
 * 考虑如何描述每个质因子的数量
 * k <= 1e6，那么它最多有多少种质因子
 * 2 * 3 * 5 * 7 * 11 * 13 * 17 = 5e5，所以最多是七个质因子
 * 那直接用 map<char[7], int> 好像就可以
 * 先处理出来 k 的所有质因子，从小到大编号
 * 内层数组的每个数表示对应质因子的出现次数
 * 从左边或者右边转移，所有素数加上当前点的
 * 超时了难绷 https://www.luogu.com.cn/record/201735515
 * 没必要用 char[7]，把它们乘起来其实就好了
 * 这时候 key 的意义是这个数与 K 的 gcd
 * map<int, int> 即可
 */
namespace Solution_1404362619582699 {
    constexpr const int mod = 998244353;

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, K;  std::cin >> N >> K;
        std::vector grid(N, std::vector<int>(N));
        for (auto &row: grid) {
            for (auto &item: row) {
                std::cin >> item;
            }
        }

        std::vector F(N, std::vector<std::unordered_map<int, int>>(N));
        F[0][0][std::gcd(grid[0][0], K)] = 1;  // 起点
        for (auto i: range(N)) {
            for (auto j: range(N)) {
                if (i == 0 and j == 0)  continue;
                if (grid[i][j] == -1)  continue;
                auto transform_from = [&](int x, int y) -> void {
                    if (x < 0 or y < 0 or x >= N or y >= N)  return;
                    for (auto [stat, cnt]: F[x][y]) {
                        auto new_stat = std::gcd<ll>(static_cast<ll>(stat) * grid[i][j], K);
                        F[i][j][new_stat] = (F[i][j][new_stat] + cnt) % mod;
                    }
                };
                transform_from(i-1, j), transform_from(i, j-1);
            }
        }

        debug {
            for (auto i: range(N)) {
                for (auto j: range(N)) {
                    for (auto [k, cnt]: F[i][j]) {
                        std::cout << std::format("F[{}][{}][{}] = {}", i, j, k, cnt) << std::endl;
                    }
                }
            }
        }

        std::cout << F[N-1][N-1][K] << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1404362619582699::solve();
    return 0;
}
