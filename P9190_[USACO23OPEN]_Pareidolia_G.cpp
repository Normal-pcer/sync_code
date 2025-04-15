/**
 * @link https://www.luogu.com.cn/problem/P9190
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
using namespace lib;

/*
F[i][j] 表示字母 [1, i] 中，最后一个 "bessie" 子串已经出现了 j 个字符，此时的最优解。
最优解是 bessie 最多的前提下，使得价钱尽可能少。

F[i][j] <- F[i - 1][j - 1]，如果 s[i] 恰好为 "bessie" 的第 j 项。
F[i][0] <- F[i - 1][*]
F[i][j] <- F[i - 1][j] + cost[i]
F[i][0] <- F[i][6]
*/
namespace Solution_8321436325950558 {
    struct Result {
        i32 repeat = 0;
        i32 cost = 0;

        auto friend operator<=> (Result const &lhs, Result const &rhs) -> std::strong_ordering {
            if (auto cmp = lhs.repeat <=> rhs.repeat; cmp != 0) return cmp;
            return rhs.cost <=> lhs.cost;
        }
        auto friend operator== (Result const &lhs, Result const &rhs) -> bool {
            return (lhs <=> rhs) == 0;
        }
    };
    char const constexpr *base = "_bessie";
    i32 constexpr baseLen = 6;
    auto solve() -> void {
        std::string s;
        std::cin >> s;
        s.insert(s.begin(), '_');  // 1-index

        i32 n = static_cast<i32>(s.size()) - 1;

        std::vector<i32> costOf(n + 1);
        for (auto &x: costOf | views::drop(1)) std::cin >> x;
        
        
        auto f = [&]() {
            std::array<Result, 7> inner;
            inner.fill({-inf, 0});
            return std::vector(n + 1, inner);
        }();

        f[0][0] = {0, 0};
        for (i32 i = 1; i <= n; i++) {
            for (i32 j = 1; j <= baseLen; j++) {
                if (s[i] == base[j]) {
                    chkMax(f[i][j], f[i - 1][j - 1]);
                }
                chkMax(f[i][0], f[i - 1][j]);
                chkMax(f[i][j], {f[i - 1][j].repeat, f[i - 1][j].cost + costOf[i]});
            }
            chkMax(f[i][0], f[i - 1][0]);
            chkMax(f[i][0], {f[i][6].repeat + 1, f[i][6].cost});
        }

        auto ans = ranges::max(f[n]);
        std::cout << ans.repeat << endl << ans.cost << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_8321436325950558::solve();
    return 0;
}
