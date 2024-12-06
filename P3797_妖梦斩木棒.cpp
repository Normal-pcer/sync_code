/**
 * @link https://www.luogu.com.cn/problem/P3797
 */

#include "./lib"

using namespace lib;

namespace Solution_1454236553461775 {
    struct SegTree {
        struct Node {
            int l, r;
            int cnt = 0, lc = 0, rc = 0;  // 0 = 无括号，1 = '(', 2 = ')'
        };

        std::vector<Node> tr;
    public:
        SegTree(int N): tr((N+1) << 2) {
            build(1, 1, N, N);
        }
#define ls (p << 1)
#define rs (p << 1 | 1)
        void push_up(Node &p, Node const &lson, Node const &rson) const {
            p.cnt = lson.cnt + rson.cnt;
            if (lson.rc == 1 and rson.lc == 2)  p.cnt++;
            p.lc = (lson.lc == 0? rson.lc: lson.lc);
            p.rc = (rson.rc == 0? lson.rc: rson.rc);
        }

        void build(int p, int l, int r, int N) {
            tr[p].l = l, tr[p].r = r;
            if (l == r) {
                if (l == 1)  tr[p].lc = tr[p].rc = 1;
                else if (l == N)  tr[p].lc = tr[p].rc = 2;
                else  tr[p].lc = tr[p].rc = 0;
                return;
            }
            auto mid = (l + r) >> 1;
            build(ls, l, mid, N);
            build(rs, mid+1, r, N);
            push_up(tr[p], tr[ls], tr[rs]);
        }

        void update(int p, int pos, int val) {
            if (tr[p].l == tr[p].r) {
                tr[p].lc = tr[p].rc = val;
                return;
            }
            if (tr[ls].r >= pos)  update(ls, pos, val);
            else  update(rs, pos, val);
            push_up(tr[p], tr[ls], tr[rs]);
        }

        Node query(int p, int l, int r) {
            if (tr[p].l >= l and tr[p].r <= r) {
                return tr[p];
            }
            
            Node res0 = {tr[p].l, tr[p].r, 0, 0, 0}, res1 = {tr[ls].l, tr[ls].r, 0, 0, 0}, res2 = {tr[rs].l, tr[rs].r, 0, 0, 0};
            if (tr[ls].r >= l)  res1 = query(ls, l, r);
            if (tr[rs].l <= r)  res2 = query(rs, l, r);
            push_up(res0, res1, res2);
            return res0;
        }
#undef ls
#undef rs
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, M;  std::cin >> N >> M;
        SegTree seg(N);
        for (auto i = 0; i < M; i++) {
            int x, y;  std::cin >> x >> y;
            if (x == 2) {
                int z;  std::cin >> z;
                std::cout << seg.query(1, y, z).cnt << std::endl;
            } else {
                char ch;  std::cin >> ch;
                if (ch == '(')  seg.update(1, y, 1);
                else if (ch == ')')  seg.update(1, y, 2);
                else  seg.update(1, y, 0);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1454236553461775::solve();
    return 0;
}
