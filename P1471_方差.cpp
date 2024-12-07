/**
 * @link https://www.luogu.com.cn/problem/P1471
 */

#include "lib"
using namespace lib;

namespace Solution_8988634283922841 {
    class SegTree {
        struct Node {
            int l, r, len;
            int sum = 0, square_sum = 0;
            int tag = 0;
        };
        std::vector<Node> tr;
        std::vector<int> const &initializer;
#define ls (p<<1)
#define rs (p<<1 | 1)
        void build(int l, int r, int p = 1) {
            tr[p].l = l, tr[p].r = r, tr[p].len = r - l + 1;

        }
    public:
        SegTree(int N, std::vector<int> const &init): tr(N << 2), initializer(init) {
            build(1, N);
        }
#undef ls
#undef rs
    };
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_8988634283922841::solve();
    return 0;
}
