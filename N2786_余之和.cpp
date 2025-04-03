/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=2786
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_4275873758316556 {
    auto nearMod(i64 a, i64 const mod) -> i64 {
        if (a >= mod) a -= mod;
        return a;
    }
    auto mulMod(i64 a, i64 b, i64 const mod) -> i64 {
        i64 res = 0;
        a %= mod;
        for (; b != 0; b >>= 1, a = nearMod(a + a, mod)) {
            if (b & 1) res = nearMod(res + a, mod);
        }
        return res;
    }
    auto solve() -> void {
        i64 N; std::cin >> N;

        i64 constexpr mod = 23'333'333'333'333'333;
        i64 ans = 0;
        // 遍历 floor(n / k) 的值域
        for (i64 first_i = 1; first_i <= N; ) {
            auto val = N / first_i;
            auto last_i = N / val + 1;

            auto cnt = last_i - first_i;
            auto mul_div2_mod = [](i64 a, i64 b, i64 const mod) -> i64 {
                if (a % 2 == 0) a /= 2;
                else b /= 2;
                return mulMod(a, b, mod);
            };
            auto scale = mul_div2_mod(cnt, first_i + first_i + cnt - 1, mod);
            ans += mulMod(scale, val, mod);
            if (ans >= mod) ans -= mod;

            first_i = last_i;
        }
        auto squareN = mulMod(N, N, mod);
        ans = squareN - ans;
        if (ans >= mod) ans -= mod;
        if (ans < 0) ans += mod;
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_4275873758316556::solve();
    return 0;
}
