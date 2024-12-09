/**
 * @link https://www.luogu.com.cn/problem/P6492
 */

#include "./lib_v2.hpp"

using namespace lib;

namespace Solution_5940484066082754 {

    class SegTree {
        struct Node {
            int l, r, len;
            int ans, lmax, rmax;
            bool lc, rc;  // 0 = 'L', 1 = 'R'
        };
        std::vector<Node> tr;
    #define ls (p << 1)
    #define rs (p << 1 | 1)
    public:
        SegTree(int N): tr(N << 2) {
            build(1, 1, N);
        }

        void merge(Node &root, Node const &left, Node const &right) const {
            root.ans = std::max(left.ans, right.ans);
            root.lc = left.lc, root.rc = right.rc;
            root.lmax = left.lmax, root.rmax = right.rmax;
            if (left.rc != right.lc) {
                chkMax(root.ans, left.rmax + right.lmax);
                if (left.lmax == left.len)  root.lmax += right.lmax;
                if (right.rmax == right.len)  root.rmax += left.rmax;
            }
        }

        void push_up(int p) {
            merge(tr[p], tr[ls], tr[rs]);
        }

        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r, tr[p].len = r - l + 1, tr[p].ans = tr[p].lmax = tr[p].rmax = 1;
            tr[p].lc = tr[p].rc = false;
            
            if (l == r)  return;
            auto mid = (l + r) >> 1;
            build(ls, l, mid);
            build(rs, mid+1, r);
        }

        void update(int p, int pos) {
            if (tr[p].l == pos and pos == tr[p].r) {
                tr[p].lc = tr[p].rc = !tr[p].lc;
                return;
            }
            if (tr[ls].r >= pos)  update(ls, pos);
            else  update(rs, pos);
            push_up(p);
        }

        Node query(int, int, int) const {
            return tr[1];
        }

    #undef ls
    #undef rs
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, Q;
        std::cin >> N >> Q;
        SegTree seg(N);

        for (auto i = 0; i < Q; i++) {
            int x;  std::cin >> x;
            seg.update(1, x);
            std::cout << seg.query(1, 1, N).ans << endl;
        }
    }
}
int main(int argc, char const *argv[]) {
    initDebug;
    Solution_5940484066082754::solve();
    return 0;
}
