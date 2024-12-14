/**
 * @link https://www.luogu.com.cn/problem/P4677
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_3277344617769969 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<int> pos(N);
        for (auto &i: pos | views::drop(1))  std::cin >> i;

        std::partial_sum(pos.begin(), pos.end(), pos.begin());

        std::vector w(N, std::vector<int>(N+1));
        for (auto i: range(N)) {
            for (auto j: range(i+1, N+1)) {
                auto mid = (i + j) >> 1;
                for (auto k: range(i, j)) {
                    w.at(i).at(j) += std::abs(pos.at(k) - pos.at(mid));
                }
            }
        }

        std::vector F(N+1, std::vector<int>(M+1, inf));
        for (auto i: range(1, N+1)) {
            F.at(i).at(1) = w.at(0).at(i);
            for (auto j: range(2, M+1)) {
                for (auto k: range(1, i)) {
                    chkMin(F.at(i).at(j), F.at(k).at(j-1) + w.at(k).at(i));
                }
            }
        }
        std::cout << F.at(N).at(M) << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3277344617769969::solve();
    return 0;
}
