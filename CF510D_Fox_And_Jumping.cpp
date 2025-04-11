/**
 * @link https://www.luogu.com.cn/problem/CF510D
 * @link https://codeforces.com/problemset/problem/510/D
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
只需取出若干个数，它们的最大公约数为 1。
由裴蜀定理，这种情况下一定可以保证能够凑出 1。
而所有数的约数个数一定是比较有限的，可能的 gcd 也会很少。
使用 map 进行 dp 即可。
*/
namespace Solution_2231663168481729 {
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<i32> lens(n), costs(n);
        for (auto &x: lens) std::cin >> x;
        for (auto &x: costs) std::cin >> x;

        // f[i] 表示选几个 gcd 为 i 的数，最小代价为多少
        std::map<i32, i32> f;

        for (i32 p = 0; p < n; p++) {
            auto copy = f;
            if (auto it = f.find(lens[p]); it != f.end()) {
                chkMin(it->second, costs[p]);
            } else {
                f.insert({lens[p], costs[p]});
            }
            for (auto [key, val]: copy) {
                auto newGcd = std::gcd(key, lens[p]);
                if (auto it = f.find(newGcd); it != f.end()) {
                    chkMin(it->second, val + costs[p]);
                } else {
                    f.insert({newGcd, val + costs[p]});
                }
            }
        }

        if (auto it = f.find(1); it != f.end()) {
            std::cout << it->second << endl;
        } else {
            std::cout << -1 << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_2231663168481729::solve();
    return 0;
}
