/**
 * @link https://www.luogu.com.cn/problem/T586391?contestId=231658
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_4110319084178899 {
    char const constexpr *keyboardLayout[] = {
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm",
    };
    std::size_t constexpr keyboardSizeEachLine[] = {
        10,
        9,
        7,
    };
    double constexpr offsetEachRow[] = {
        0.0,
        0.5,
        1.0
    };

    auto solve() -> void {
        using std::operator""sv;
        std::string policy;
        std::getline(std::cin, policy);

        std::vector<std::string> paths;
        auto it = policy.begin();
        for (; it != policy.end(); ) {
            paths.push_back("");
            for (; it != policy.end() and *it != ' '; it++) {
                paths.back() += *it;
            }
            if (it != policy.end() and *it == ' ') {
                it++;
            }
        }

        double x = 0;
        i32 y = 0;
        for (auto const &path: paths) {
            for (auto ch: path) {
                switch (ch) {
                case 'W':
                    x -= 0.5, y -= 1;
                    break;
                case 'E':
                    x += 0.5, y -= 1;
                    break;
                case 'Z':
                    x -= 0.5, y += 1;
                    break;
                case 'X':
                    x += 0.5, y += 1;
                    break;
                case 'D':
                    x += 1;
                    break;
                case 'A':
                    x -= 1;
                    break;
                default:
                    assert(false), __builtin_unreachable();
                }
            }
            if (y > 2) { assert(false); continue; }
            if (y < 0) { assert(false); continue; }
            auto index = static_cast<i32>(x - offsetEachRow[y]);
            if (index < 0) { assert(false); continue; }
            if (index >= static_cast<i32>(keyboardSizeEachLine[y])) { assert(false); continue; }
            auto letter = keyboardLayout[y][index];
            std::cout << letter;
        }
        std::cout << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_4110319084178899::solve();
    return 0;
}
