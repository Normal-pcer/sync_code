/**
 * @link https://www.luogu.com.cn/problem/AT_abc269_b
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1705022884169357 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::vector<std::string> mat(10);
        for (auto &s: mat)  std::cin >> s;

        auto all_dot = [&](std::string const &row) -> bool {
            return std::all_of(row.begin(), row.end(), lam(x, x == '.'));
        };
        auto first_row = ranges::find_if_not(mat.begin(), mat.end(), all_dot);
        auto last_row = ranges::find_last_if_not(mat.begin(), mat.end(), all_dot);

        auto x1 = std::distance(mat.begin(), first_row) + 1;
        auto x2 = std::distance(mat.begin(), last_row.begin()) + 1;
        auto y1 = first_row->find('#') + 1;
        auto y2 = first_row->find("#.") + 1;
        if (y2 == 0LL)  y2 += 10;

        std::cout << y1 << " " << y2 << std::endl << x1 << " " << x2 << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1705022884169357::solve();
    return 0;
}
