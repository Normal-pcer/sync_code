/**
 * @link https://www.luogu.com.cn/problem/P3147
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_6828279621444478 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> a(N);
        for (auto &i: a)  std::cin >> i;

        auto const max_j = ranges::max(a) + std::__lg(N) + 2;
        std::vector F(N, std::vector<int>(max_j));
        for (auto i: range(a.size())) {
            F[i][a[i]] = i + 1;
        }
        for (auto j: range(max_j - 1)) {
            for (auto i: range(a.size())) {
                auto x = F[i][j];
                if (x != 0 and x < (int)a.size()) {
                    F[i][j+1] = F[x][j];
                }
            }
        }
        for (auto j: range(max_j) | views::reverse) {
            for (auto i: range(a.size())) {
                if (F[i][j] != 0) {
                    std::cout << j << std::endl;
                    return;
                }
            }
        }
        std::cout << "I AK IOI!" << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6828279621444478::solve();
    return 0;
}