/**
 * @link https://www.luogu.com.cn/problem/P2736
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/rolling_container.hpp"

using namespace lib;

namespace Solution_1092020689218958 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, T, M;  std::cin >> N >> T >> M;
        // F[j][k]：前 i 首音乐放到 j 个光盘中，最后一个光盘存放了 k 分钟
        std::vector F(M+1, std::vector<int>(T+1, -inf));

        std::vector<int> musics(N);
        for (auto &i: musics)  std::cin >> i;
        std::erase_if(musics, lam(x, x > T));
        auto ans = 0;
        F[0][0] = 0;
        for (auto i: range(musics.size())) {
            for (auto j: range(1, M+1) | views::reverse) {
                for (auto k: range(std::max(0, T+1-musics.at(i))) | views::reverse) {
                    chkMax(F[j][k + musics.at(i)], F[j][k] + 1);
                }
                for (auto x: range(T+1))  chkMax(F[j][musics.at(i)], F[j-1][x] + 1);
                for (auto k: range(T+1))  chkMax(ans, F[j][k]);
            }
        }

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1092020689218958::solve();
    return 0;
}
