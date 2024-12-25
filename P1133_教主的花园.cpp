/**
 * @link https://www.luogu.com.cn/problem/P1133
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_4975579123371222 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector value(N, std::vector<int>(3));
        for (auto &item: value) {
            for (auto i: {0, 1, 2})  std::cin >> item[i];
        }

        std::vector<std::array<std::array<std::array<int, 2>, 3>, 3>> F(N);  // F[N][3][3][2]
        for (auto j: {0, 1, 2}) {
            for (auto k: {0, 1}) {
                F[0][j][j][k] = value[0][j];
            }
        }
        for (auto i: range(1, N)) {
            for (auto j: {0, 1, 2}) {
                for (auto z: {0, 1, 2}) {
                    for (auto x: range(0, j)) {
                        debug  std::cout << std::format("F[{}][{}][{}][{}] <- F[{}][{}][{}][{}]", i, j, z, 1, i-1, x, z, 0) << std::endl;
                        chkMax(F[i][j][z][1], F[i-1][x][z][0] + value[i][j]);
                    }
                    for (auto x: range(j+1, 3)) {
                        debug  std::cout << std::format("F[{}][{}][{}][{}] <- F[{}][{}][{}][{}]", i, j, z, 0, i-1, x, z, 0) << std::endl;
                        chkMax(F[i][j][z][0], F[i-1][x][z][1] + value[i][j]);
                    }
                }
            }
        }
        debug  for (auto i: range(N)) {
            for (auto j: {0, 1, 2}) {
                for (auto z: {0, 1, 2}) {
                    for (auto k: {0, 1}) {
                        printValues(i, j, z, k, F[i][j][z][k]);
                    }
                }
            }
        }
        auto ans = -inf;
        for (auto j: {0, 1, 2}) {
            for (auto z: {0, 1, 2}) {
                for (auto k: {0, 1}) {
                    if ((k and z < j) or (not k and z > j)) {
                        chkMax(ans, F[N-1][j][z][k]);
                    }
                }
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4975579123371222::solve();
    return 0;
}
