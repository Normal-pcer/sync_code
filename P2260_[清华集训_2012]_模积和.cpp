/**
 * @link https://www.luogu.com.cn/problem/P2260
 */
#include "./lib_v6.hpp"
using i16 = int16_t; using i32 = int32_t; using i64 = int64_t;
using u16 = uint16_t; using u32 = uint32_t; using u64 = uint64_t; using uz = size_t;
using namespace lib;

/*
考虑如下的子问题：

*/
namespace Solution_7648567164118096 {
    __extension__ using i128 = __int128;
    auto constexpr mulMod(i32 a, i32 b, i32 const mod) -> i32 {
        return static_cast<i64>(a) * b % mod;
    }
    auto constexpr addMod(i32 a, i32 b, i32 const mod) -> i32 {
        a += b;
        if (a >= mod) a -= mod;
        return a;
    }
    // 计算 1 * 1 + 2 * 2 + 3 * 3 + ... + n * n
    auto constexpr squareIntegersSum(i32 n, i32 const mod) -> i32 {
        auto ans = static_cast<i128>(n) * (n + 1) * (n + n + 1) / 6;
        return ans % mod;
    }
    auto constexpr nearMod(i32 a, i32 const mod) {
        if (a >= mod) a -= mod;
        return a;
    }
    auto solve() -> void {
        i32 N, M;
        std::cin >> N >> M;

        i32 constexpr mod = 19'940'417;

        // 对以下式子求值：
        // sum i * floor(n / i) {1 <= i <= n}，n 为给定参数
        auto calc1 = [&](i32 n, i32 cnt) -> i32 {
            i32 ans = 0;
            for (i32 first = 1; first <= cnt; ) {
                auto val = n / first;
                auto last = val == 0? cnt + 1: n / val + 1;
                last = std::min(cnt + 1, last);
                auto cnt = last - first;
                ans = addMod(mulMod(val, static_cast<i64>(cnt) * (first + last - 1) / 2 % mod, mod), ans, mod);
                first = last;
            }
            debug std::cout << std::format("calc({}) = {}", n, ans) << std::endl;
            return ans;
        };

        // sum i * i * floor(n / i) * floor(m / i)，n 和 m 为给定参数
        auto calc2 = [&](i32 n, i32 m) -> i32 {
            if (n > m) std::swap(n, m);
            i32 ans = 0;
            for (i32 first = 1; first <= n; ) {
                auto a = n / first;
                auto b = m / first;
                auto lastA = n / a + 1;
                auto lastB = m / b + 1;
                // 下一次到达的端点
                auto last = std::min(lastA, lastB);

                auto squareISum = squareIntegersSum(last - 1, mod) - squareIntegersSum(first - 1, mod);
                if (squareISum < mod) squareISum += mod;

                ans = addMod(mulMod(mulMod(squareISum, a, mod), b, mod), ans, mod);
                first = last;
            }
            debug std::cout << std::format("calc2({}, {}) = {}", n, m, ans) << std::endl;
            return ans;
        };

        auto check = [&]() -> void {
            std::mt19937 rng{std::random_device{}()};
            for (auto t = 10000; t --> 0; ) {
                auto distribution = std::uniform_int_distribution<i32>{1, 10000};
                auto n = distribution(rng);

                auto ans1 = calc1(n, n);
                i32 ans2 = 0;
                for (i32 i = 1; i <= n; i++) {
                    ans2 = addMod(mulMod(i, n / i, mod), ans2, mod);
                }
                assert(ans1 == ans2);
                std::cout << ans1 << " " << ans2 << std::endl;
            }
        };
        // check();
        static_cast<void>(check);

        auto ans = mulMod(nearMod(mulMod(N, N, mod) - calc1(N, N) + mod, mod), nearMod(mulMod(M, M, mod) - calc1(M, M) + mod, mod), mod);
        auto delta = mulMod(mulMod(N, M, mod), std::min(N, M), mod);
        delta = addMod(delta, mod - mulMod(M, calc1(N, std::min(N, M)), mod), mod);
        delta = addMod(delta, mod - mulMod(N, calc1(M, std::min(N, M)), mod), mod);
        delta = addMod(delta, calc2(N, M), mod);
        ans = addMod(ans, mod - delta, mod);

        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_7648567164118096::solve();
    return 0;
}
