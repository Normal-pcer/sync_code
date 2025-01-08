/**
 * @link https://www.luogu.com.cn/problem/AT_abc264_f
 * @link https://atcoder.jp/contests/abc264/tasks/abc264_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_9681356274053257 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::array<std::array<ll, 2>, 2> base;
        std::memset(base.begin(), 0x3f, sizeof(base));
        std::vector F(N+1, std::vector(M+1, base));  // (i, j) 处，行列有无反转，最小代价
        std::vector map(N+1, std::vector<char>(M+1));

        std::vector<int> r_cost(N+1), c_cost(M+1);
        for (auto &x: r_cost | views::take(N))  std::cin >> x;
        for (auto &x: c_cost | views::take(M))  std::cin >> x;
        for (auto &line: map | views::take(N)) {
            std::string s;  std::cin >> s;
            ranges::transform(s, line.begin(), lam(x, x == '1'));
        }
        F[0][0][0][0] = 0;
        F[0][0][0][1] = c_cost[0];
        F[0][0][1][0] = r_cost[0];
        F[0][0][1][1] = c_cost[0] + r_cost[0];
        for (auto i: range(N)) {
            const int j = 0;
            for (auto k: {0, 1}) {
                for (auto l: {0, 1}) {
                    for (auto m: {0LL, 1LL}) {
                        const auto n = l;
                        if ((map[i+1][j]^k^l) != (map[i][j]^m^n))  continue;
                        chkMin(F[i+1][j][m][n], F[i][j][k][l] + (r_cost[i+1] & -m));
                    }
                }
            }
        }
        for (auto j: range(M)) {
            const int i = 0;
            for (auto k: {0, 1}) {
                for (auto l: {0, 1}) {
                    const auto m = k;
                    for (auto n: {0LL, 1LL}) {
                        if ((map[i][j]^k^l) != (map[i][j+1]^m^n))  continue;
                        chkMin(F[i][j+1][m][n], F[i][j][k][l] + (c_cost[j+1] & -n));
                    }
                }
            }
        }
        for (auto i: range(N)) {
            for (auto j: range(M)) {
                // 向右转移
                for (auto k: {0, 1}) {
                    for (auto l: {0, 1}) {
                        const auto m = k;
                        for (auto n: {0LL, 1LL}) {
                            if ((map[i][j]^k^l) != (map[i][j+1]^m^n))  continue;
                            chkMin(F[i][j+1][m][n], F[i][j][k][l] + (c_cost[j+1] & -n));
                        }
                    }
                }
                // 向下转移
                for (auto k: {0, 1}) {
                    for (auto l: {0, 1}) {
                        for (auto m: {0LL, 1LL}) {
                            const auto n = l;
                            if ((map[i+1][j]^k^l) != (map[i][j]^m^n))  continue;
                            chkMin(F[i+1][j][m][n], F[i][j][k][l] + (r_cost[i+1] & -m));
                        }
                    }
                }
            }
        }
        debug for (auto i: range(N)) {
            for (auto j: range(M)) {
                for (auto k: range(2)) {
                    for (auto l: range(2)) {
                        printValues(i, j, k, l, F[i][j][k][l]);
                    }
                }
            }
        }
        auto ans = ranges::min({F[N-1][M-1][0][0], F[N-1][M-1][0][1], F[N-1][M-1][1][0], F[N-1][M-1][1][1]});
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9681356274053257::solve();
    return 0;
}
