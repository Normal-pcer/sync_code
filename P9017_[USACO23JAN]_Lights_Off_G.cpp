/**
 * @link https://www.luogu.com.cn/problem/P9017
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
题意：
有 n 个灯泡，对应了 n 个开关，每个开关都有是否可用之分。

首先，不妨假设让所有的开关都在初始时不可用。
令 g[i][k] 表示一个在 k 处开启的开关，在接下来的 i 步之内可以影响多少个灯泡。

f[i][st] 表示 i 步操作之后，状态为 j 的灯是否可能全被关闭。

*/
namespace Solution_3407243190822596 {
    auto solve() -> void {
        i32 t, n;
        std::cin >> t >> n;

        using Bits = u32;
        auto readBits = [&]() -> Bits {
            char ch; std::cin.get(ch);
            while (ch != '0' and ch != '1') std::cin.get(ch);

            Bits res = 0;
            for (auto _ = n; _ --> 0; ) {
                res = res << 1 | (ch == '1');
                std::cin.get(ch);
            }
            return res;
        };

        std::vector g(n * 2 + 1, std::vector<Bits>(n, 0));
        for (i32 k = 0; k < n; k++) {
            for (i32 i = 1; i <= 2 * n; i++) {
                for (i32 j = 0; j < i; j++) {
                    g[i][k] ^= (Bits)1 << ((k - j + n + n) % n);
                }
            }
        }

        std::vector f(n * 2 + 1, std::vector<char>(1 << n, false));
        f[0][0] = true;
        for (i32 i = 1; i <= n * 2; i++) {
            for (Bits j = 0; j < ((Bits)1 << n); j++) {
                if (not f[i - 1][j]) continue;
                for (i32 k = 0; k < n; k++) {
                    auto vis = j ^ g[i][k];
                    f[i][vis] = true;
                }
            }
        }

        for (auto _ = t; _ --> 0; ) {
            Bits a = readBits(), b = readBits();

            auto calc = [&](Bits s, Bits t) -> i32 {
                if (s == 0) return 0;
                for (i32 i = 1; i <= n * 2; i++) {
                    auto vis = s;
                    // 对于每一个已经开启的开关
                    for (i32 j = 0; j < n; j++) {
                        if (t & ((Bits)1 << j)) vis ^= g[i][j];
                    }
                    if (f[i][vis]) return i;
                }
                assert(false);
                return -1;
            };
            std::cout << calc(a, b) << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_3407243190822596::solve();
    return 0;
}
