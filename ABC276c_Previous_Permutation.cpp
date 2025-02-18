/**
 * @link https://www.luogu.com.cn/problem/AT_abc276_c
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_1115709672668968 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> vec(N);
        for (auto &x: vec)  std::cin >> x;

        std::prev_permutation(vec.begin(), vec.end());
        for (auto x: vec)  std::cout << x << " ";
        std::cout << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1115709672668968::solve();
    return 0;
}
