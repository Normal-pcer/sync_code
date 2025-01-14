/**
 * @link https://www.luogu.com.cn/problem/P7735
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/range.hpp"
using namespace lib;

namespace Solution_9288857495666426 {
    struct Node {
        int begin = 0, end = 0;
        int val = 0;  // 当前点的点权（叶子节点）/
        int assign_tag = -1;

        auto assign(int x) -> void {
            val = assign_tag = x;
        }
    };
    class Tree {

    };
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int T;  std::cin >> T;
    while (T --> 0) {
        Solution_9288857495666426::solve();
    }
    return 0;
}
