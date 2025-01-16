/**
 * @link https://www.luogu.com.cn/problem/P6870
 */

#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/range.hpp"
using namespace lib;

// F[i][j] 表示分配前 i 个人、前 j 个题
namespace Solution_1152672094401331 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1152672094401331::solve();
    return 0;
}
