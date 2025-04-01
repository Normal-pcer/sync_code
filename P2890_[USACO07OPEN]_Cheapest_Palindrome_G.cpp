/**
 * @link https://www.luogu.com.cn/problem/P2890
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
using namespace lib;

/*
用 F[i][j] 表示 [0, i], [j, N - 1] 两个区间可以互相匹配上，此时的最小代价。
如同使用两个指针，希望指针向内侧移动。
转移：可以在任意一侧：
- 增加和另一侧一样的字符
- 删除一个字符
无论如何，都可以保证新的 [i, j] 区间减小。
例如：
a b c b
^     ^
右侧删除：
a b c
^   ^

右侧添加：
a b c b a
  ^   ^

左侧同理。

转移方程：
令 a 为 s[i - 1]，b 为 s[j + 1]。
F[i][j] <- min({
    F[i][j + 1] + cost(delete b)
    F[i - 1][j] + cost(add a)
    F[i - 1][j] + cost(delete a)
    F[i][j + 1] + cost(add b)
})
F[i][j] <- F[i - 1][j + 1] 仅当 a = b.

初始：F[-1][N] = 0，其他为无穷大。
改用 1-index。
答案：所有空区间的较小值，即 j = i + 1。
*/
namespace Solution_2344395429132209 {
    auto solve() -> void {
        i32 letterCount, len;
        std::cin >> letterCount >> len;

        std::string s;
        std::cin >> s;
        s.insert(s.begin(), '_');

        using Cost = i32;
        struct Info {
            char letter{};
            Cost addCost{};
            Cost delCost{};
        };
        std::map<char, Info> letters;
        for (auto _ = letterCount; _ --> 0; ) {
            char letter;
            Cost add, del;
            std::cin >> letter >> add >> del;
            letters[letter] = {letter, add, del};
        }
        char constexpr nullLetter = 0;
        letters[nullLetter] = {nullLetter, (Cost)inf, (Cost)inf};

        std::vector F(len + 2, std::vector(len + 2, (Cost)inf));
        F[0][len + 1] = 0;
        for (i32 segLen = len + 1; segLen > 0; segLen--) {
            for (i32 i = 0; i <= len; i++) {
                auto j = segLen + i - 1;
                if (j > len + 1) break;

                auto hasA = i != 0;
                auto hasB = j != len + 1;
                auto a = hasA? letters[s[i]]: letters[nullLetter];
                auto b = hasB? letters[s[j]]: letters[nullLetter];

                if (hasB) chkMin(F[i][j], F[i][j + 1] + b.delCost);
                if (hasA) chkMin(F[i][j], F[i - 1][j] + a.addCost);
                if (hasA) chkMin(F[i][j], F[i - 1][j] + a.delCost);
                if (hasB) chkMin(F[i][j], F[i][j + 1] + b.addCost);

                if (hasA and hasB and a.letter == b.letter and a.letter != nullLetter) {
                    chkMin(F[i][j], F[i - 1][j + 1]);
                }
            }
        }

        debug {
            for (i32 i = 0; i <= len + 1; i++) {
                for (i32 j = 0; j <= len + 1; j++) {
                    std::cout << std::format("F[{}][{}] = {}", i, j, F[i][j]) << std::endl;
                }
            }
        }

        auto ans = (Cost)inf;
        for (i32 i = 0; i < len; i++) {
            chkMin(ans, F[i][i + 1]);
        }
        for (i32 i = 0; i < len - 1; i++) {
            chkMin(ans, F[i][i + 2]);
        }
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_2344395429132209::solve();
    return 0;
}
