/**
 * @link https://www.luogu.com.cn/problem/P7057
 */

#include "lib"
using namespace lib;

namespace Solution_1133362793174219 {
    class SegTree {
        struct Node {
            int begin, end;
            int min;
        };
        std::vector<Node> tr;

    };
    struct Card {
        int r, p;
    };
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1133362793174219::solve();
    return 0;
}
