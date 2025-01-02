/**
 * @link https://www.luogu.com.cn/problem/P5020
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/bit.hpp"

using namespace lib;

namespace Solution_1472701684758746 {
    void solve() {
        int N;  std::cin >> N;
        std::vector<int> a(N);
        for (auto &x: a)  std::cin >> x;
        ranges::sort(a);

        constexpr const int _V = 25008;
        std::bitset<_V> reachable = 1;  // 是否可以凑到 i
        auto ans = 0;
        for (auto x: a) {
            if (reachable[x])  ans++;
            for (auto i = 1; i * x < _V; i++) {
                reachable |= reachable << (i * x);
            }
        }
        std::cout << N - ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int T;  std::cin >> T;
    while (T --> 0) {
        Solution_1472701684758746::solve();
    }
    return 0;
}
