/**
 * @link https://www.luogu.com.cn/problem/P3239
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6934306267442359 {
    void solve() {
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T; std::cin >> T;
    while (T --> 0) Solution_6934306267442359::solve();
    return 0;
}
