/**
 * @link https://www.luogu.com.cn/problem/P4290
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_2121952539319613 {
    auto const fail = "The name is wrong!";
    using str2 = std::string;
    auto const letters = "WING";
    constexpr int indexOf(char ch) {
        auto it = letters;
        for (; *it; it++) {
            if (*it == ch)  return std::distance(letters, it);
        }
        return std::distance(letters, it);
    } 
    void solve() {
        struct Replacements {
            int count;
            std::vector<str2> items;
        };
        std::vector<Replacements> replace(4);
        for (auto &i: replace)  std::cin >> i.count;
        for (auto &i: replace) {
            i.items.resize(i.count);
            for (auto &j: i.items) {
                std::cin >> j;
            }
        }

        std::string origin;
        std::cin >> origin;
        auto const N = (int)origin.size();
        std::vector F(N, std::vector(N+1, std::vector<char>(4)));

        for (auto const i: range(N)) {
            F[i][i+1][indexOf(origin[i])] = true;
        }
        for (auto const len: range(1, N+1)) {
            for (auto const i: range(N)) {
                auto const j = len + i;
                if (j > N)  break;
                for (auto const x: range(i, j)) {
                    for (auto const k: range(4)) {
                        for (auto str: replace[k].items) {
                            auto const first = str[0], second = str[1];
                            debug  printValues(len, i, j, x, k, first, second);
                            F[i][j][k] |= F[i][x][indexOf(first)] and F[x][j][indexOf(second)];
                        }
                    }
                }
            }
        }

        auto flag = false;
        for (auto k: range(4)) {
            if (F[0][N][k]) {
                std::cout << letters[k];
                flag = true;
            }
        }
        if (not flag) {
            std::cout << "The name is wrong!";
        }
        std::cout << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2121952539319613::solve();
    return 0;
}
