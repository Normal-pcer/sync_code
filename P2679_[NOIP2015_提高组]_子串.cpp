/**
 * 
 */
#include "./libs/debug_macros.hpp"


#include "./lib_v3.hpp"


#include "./libs/range.hpp"


#include "./libs/debug_log.hpp"


#include "./libs/rolling_container.hpp"


using namespace lib;

namespace Solution_1361299745306647 {
    const constexpr int mod = 1e9+7;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        using inner_type = std::vector<std::vector<std::array<int, 2>>>;
        RollingContainer<std::vector<inner_type>, 2> F(std::vector(K+1, std::vector<std::array<int, 2>>(M+1)));
        F[0][0][0][0] = 1;
        std::string a, b;
        std::cin >> a >> b;

        for (auto h: range(1, N+1)) {
            for (auto i: range(0, K+1)) {
                for (auto k: range(0, M+1)) {
                    F[h][i][k][0] = (F[h-1][i][k][1] + F[h-1][i][k][0]) % mod;
                    if (k != 0 and a[h-1] == b[k-1])  F[h][i][k][1] = (F[h-1][i][k-1][1] + (i? (F[h-1][i-1][k-1][0] + F[h-1][i-1][k-1][1]) % mod: 0)) % mod;
                }
            }
        }
        never {
            for (auto h: range(1, N+1)) {
                for (auto i: range(1, K+1)) {
                    for (auto k: range(0, M+1)) {
                        for (auto x: {0, 1}) {
                            printValues(h, i, k, x, F[h][i][k][x]);
                        }
                    }
                }
            }
        }
        auto ans = (F[N][K][M][0] + F[N][K][M][1]) % mod;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1361299745306647::solve();
    return 0;
}
