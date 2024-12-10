/**
 * @link https://www.luogu.com.cn/problem/CF431E
 * @link https://codeforces.com/problemset/problem/431/E
 */

#include "lib"
using namespace lib;

namespace Solution_1129948889784761 {
    class SegTree {
        using value_type = ull;
        struct Node {
            int ls = 0, rs = 0;
            value_type cnt, mercury = 0;
        };
        std::vector<Node> tr;

        value_type begin, end;
    public:
        SegTree(value_type begin, value_type end): begin(begin), end(end) {}
#if false
        value_type lowerBoundHeight(value_type target, int p, value_type begin, value_type end) const {
            if (p == 0)  return begin;
            if (begin + 1 == end)  return begin;
            auto mid = std::midpoint(begin, end);
            auto ls = tr[tr[p].ls], rs = tr[tr[p].rs];
            if (mid * ls.cnt - ls.mercury <= target) {
                return lowerBoundHeight(target, tr[p].ls, begin, mid);
            } else {
                return lowerBoundHeight(target, tr[p].rs, mid, end);
            }
        }
#else
        value_type lowerBoundHeight(value_type target, int p, value_type begin, value_type end, value_type cur_cnt, value_type cur_mercury) {}
#endif
    };
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1129948889784761::solve();
    return 0;
}
