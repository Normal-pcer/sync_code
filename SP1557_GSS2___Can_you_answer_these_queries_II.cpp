/**
 * @link https://www.luogu.com.cn/problem/SP1557
 */

#include "lib"
using namespace lib;

namespace Solution_1396255543610532 {
    class SegTree1 {
        struct Node {
            int begin, end;
            int cnt, sum;
        };
        std::vector<Node> tr;
        int offset;

        inline int index(int val) const { return offset + val; }

#define ls (p << 1)
#define rs (p << 1 | 1)
        void push_up(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
        }

        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid), build(mid, end);
            push_up(p);
        }

        void insert(int val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                assert(tr[p].begin == index(val));
                tr[p].cnt++;
                if (tr[p].cnt >= 2)  tr[p].sum += val;
                return;
            }
            if (tr[ls].end > index(val))  insert(val, ls);
            else  insert(val, rs);
            push_up(p);
        }

        void erase(int val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                assert(tr[p].begin == index(val));
                assert(tr[p].cnt >= 1);
                tr[p].cnt--;
                if (tr[p].cnt >= 1)  tr[p].sum -= val;
                return;
            }
            if (tr[ls].end > index(val))  erase(val, ls);
            else  erase(val, rs);
            push_up(p);
        }

        int queryAll(int p = 1) {
            return tr[p].sum;
        }
#undef ls
#undef rs
    };
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1396255543610532::solve();
    return 0;
}
