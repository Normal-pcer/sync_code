/**
 * @link
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6314180276493067 {
    auto solve() -> void {
        std::string s, match;
        std::cin >> s >> match;

        s.insert(s.begin(), '_');
        match.insert(match.begin(), '_');
        // s += '_', match += '_';

        auto n = static_cast<i32>(s.size()) - 1, m = static_cast<i32>(match.size()) - 1;

        std::vector<i32> next(m + 1);
#if false
        for (i32 len = 1; len <= m; len++) {
            // 暴力匹配前后缀
            i32 i = 1, j = len;
            for (auto _ = len; _ --> 0; ) {
                if (match.substr(1, i - 1) == match.substr(j + 1, len - j)) {
                    next[len] = i - 1;
                }
                i++, j--;
            }
        }
#else
        i32 curMax = 0;  // 当前最长前缀的结尾
        for (i32 i = 1; i < m; i++) {
            while (curMax > 0 and match[curMax + 1] != match[i + 1]) {
                curMax = next[curMax];
            }
            if (match[curMax + 1] == match[i + 1]) {
                curMax++;
            }
            next[i + 1] = curMax;
        }
#endif
        debug {
            std::cout << "next: " << std::endl;
            for (i32 i = 1; i <= m; i++) {
                std::cout << "[" << i << "] " << next[i] << "  ";
            }
            std::cout << std::endl;
        }

        auto find = [&]() -> void {
            i32 i = 0, j = 0;

            for (; i <= n; i++) {
                while (j > 0 and s[i + 1] != match[j + 1]) {
                    j = next[j];
                }
                if (s[i + 1] == match[j + 1]) {
                    j++;
                }
                if (j == m) {
                    std::cout << i - m + 2 << endl;
                }
            }
        };
        find();

        for (i32 i = 1; i <= m; i++) {
            std::cout << next[i] << " ";
        }
        std::cout << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6314180276493067::solve();
    return 0;
}
