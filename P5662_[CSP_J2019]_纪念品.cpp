/**
 * @link https://www.luogu.com.cn/problem/P5662
 */

#include "lib"
#include "libs/range.hpp"
using namespace lib;

namespace Solution_1103073753867724 {
    const constexpr int _Coins = 1e4+1;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int T, N, M;  std::cin >> T >> N >> M;
        std::vector price(N, std::vector<int>(M));

        for (auto &line: price) {
            for (auto &item: line) {
                std::cin >> item;
            }
        }

        std::vector<std::array<int, _Coins>> F;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1103073753867724::solve();
    return 0;
}
