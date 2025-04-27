/**
 * @link https://www.luogu.com.cn/problem/P3550
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_3364267078600393 {
    auto solve() -> void {
        i64 m, d; i32 n;
        std::cin >> m >> d >> n;

        std::vector<i64> cars(n);
        for (auto &x: cars) std::cin >> x;

        ranges::sort(cars, std::greater{});

        // 选出一个车负责送到终点
        if (auto takeIt = ranges::upper_bound(cars, m - d, std::greater{}); takeIt == cars.begin()) {
            std::cout << 0 << endl;
            return;
        } else {
            auto selected = *--takeIt;
            cars.erase(takeIt);

            i64 pos = 0;
            i64 ans = 0;
            for (auto car: cars) {
                if (pos >= d or m - pos + d - pos <= selected) break;
                if (d - pos >= car) {
                    std::cout << 0;
                    return;
                }
                ++ans;
                pos += car - (d - pos);
                if (pos >= m) {
                    --ans;
                    break;
                }
            }
            ++ans;
            std::cout << ans << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_3364267078600393::solve();
    return 0;
}
