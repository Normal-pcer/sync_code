#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
预处理出所有可能的 a*b + c 以及 (e+f) * d。
放在 map 里。
*/
namespace Solution_1585286303122099 {
    auto solveForce() -> void {
        i32 n;
        std::cin >> n;

        std::vector<i64> nums(n);
        for (auto &x: nums) std::cin >> x;

        i64 ans = 0;
        for (auto a: nums) {
            for (auto b: nums) {
                for (auto c: nums) {
                    for (auto d: nums) {
                        for (auto e: nums) {
                            for (auto f: nums) {
                                if (a * b + c == d * (e + f)) {
                                    ans++;
                                }
                            }
                        }
                    }
                }
            }
        }
        std::cout << ans << endl;
    }
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<i32> nums(n);
        for (auto &x: nums) std::cin >> x;

        std::map<i32, i32> map;  // (a + b) * c
        for (auto a: nums) {
            for (auto b: nums) {
                for (auto c: nums) {
                    map[(a + b) * c]++;
                }
            }
        }

        i64 ans = 0;
        for (auto a: nums) {
            for (auto b: nums) {
                for (auto c: nums) {
                    auto it = map.find(a * b + c);
                    if (it != map.end()) {
                        auto cnt = it->second;
                        ans += cnt;
                    }
                }
            }
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_1585286303122099::solve();
    return 0;
}
