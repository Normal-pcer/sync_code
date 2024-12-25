/**
 * @link https://www.luogu.com.cn/problem/P1433
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/bit.hpp"

#include "./libs/debug_log.hpp"

#include "./libs/rolling_container.hpp"

using namespace lib;

namespace Solution_8294724057770579 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<std::pair<double, double>> cheese(N);
        for (auto &[x, y]: cheese)  std::cin >> x >> y;
        cheese.insert(cheese.begin(), {0, 0});

        auto inner = std::vector(N+1, std::vector(1 << (N+1), 1e300));
        RollingContainer<std::vector<decltype(inner)>, 2> F(inner);
        F[0][0][1] = 0;
        for (auto i: range(1, N+1)) {
            for (auto j: range(1 << (N+1))) {
                bit::traverse1(j, [&](int x) {
                    auto pos_x = std::__lg(x);
                    bit::traverse1(j, [&](int y) {
                        if (x == y)  return;
                        auto pos_y = std::__lg(y);
                        chkMin(
                            F[i][pos_x][j], 
                            F[i-1][pos_y][j - x] + std::hypot(
                                cheese[pos_x].first - cheese[pos_y].first, 
                                cheese[pos_x].second - cheese[pos_y].second
                            )
                        );
                    });
                });
            }
        }

        never for (auto i: range(N+1)) {
            for (auto j: range(N+1)) {
                for (auto k: range(1 << (N+1))) {
                    printValues(i, j, std::bitset<8>(k), F[i][j][k]);
                }
            }
        }

        auto ans = 1e300;
        for (auto i: range(N+1)) {
            chkMin(ans, F[N][i][(1<<(N+1))-1]);
        }
        std::cout << std::format("{:.2f}", ans) << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8294724057770579::solve();
    return 0;
}
