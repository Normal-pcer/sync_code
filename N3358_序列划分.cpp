/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3358
 */

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_1020353071346660 {
    auto solve() -> void {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> vec(N);
        for (auto &x: vec)  std::cin >> x;

        i64 cur = 0;
        i32 cnt = 0;
        for (auto i = N; i --> 0; ) {
            auto x = vec[i];
            cur += x;
            if (cur >= 0)  cnt++, cur = 0;
        }
        if (cur < 0) {
            std::cout << 0 << "\n";
        } else {
            std::cout << cnt << "\n";
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1020353071346660::solve();
    return 0;
}
