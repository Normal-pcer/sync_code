/**
 * @link https://www.luogu.com.cn/problem/P9982
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

using f80 = long double;
namespace Solution_4476985040799282 {
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<i32> vec(n + 1);
        for (auto &x: vec | views::drop(1)) std::cin >> x;
        ranges::sort(vec.begin() + 1, vec.end());

        std::vector<i64> ps(vec.begin(), vec.end());
        std::partial_sum(ps.begin() + 1, ps.end(), ps.begin() + 1);

        i32 q;
        std::cin >> q;
        for (i32 _ = q; _ --> 0; ) {
            i32 a, b;
            std::cin >> a >> b;

            auto u = static_cast<i32>(
                std::ceil(static_cast<f80>(b) * n / (a+b))
            );
            
            auto ans = (static_cast<i64>(vec[u]) * u - ps[u]) * a;
            ans += (ps[n] - ps[u] - static_cast<i64>(vec[u]) * (n - u)) * b;

            std::cout << ans << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4476985040799282::solve();
    return 0;
}
