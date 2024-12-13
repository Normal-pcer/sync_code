/**
 * @link https://www.luogu.com.cn/problem/P4677
 */

#include "lib"
#include "libs/range.hpp"
using namespace lib;

namespace Solution_3277344617769969 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<int> pos(N);
        for (auto &i: pos)  std::cin >> i;

        std::partial_sum(pos.begin(), pos.end(), pos.begin());
        std::vector F(N+1, std::vector<int>(M+1));

        std::vector<int> pos_ps(N);
        std::partial_sum(pos.begin(), pos.end(), pos_ps.begin());
        std::vector w(N+1, std::vector<int>(N+1));

        for (auto x: range(0, N)) {
            for (auto y: range(x, N)) {
                for (auto z: range(x, y)) {
                    auto cur = pos.at(z) * (z*2 - x + y) - (pos_ps.at(z-1) - pos_ps.at(x-1)) + (pos_ps.at(y-1) - pos_ps.at(z-1));
                    w.at(x).at(y) = cur;
                }
            }
        }

        for (auto i: range(1, N+1)) {
            for (auto j: range(1, M+1)) {
                for (auto k: range(0, j)) {
                    // F.at(i).at(j)
                }
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3277344617769969::solve();
    return 0;
}
