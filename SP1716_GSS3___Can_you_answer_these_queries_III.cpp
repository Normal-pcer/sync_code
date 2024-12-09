
/**
 * @link https://www.luogu.com.cn/problem/SP1716
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1730635379514186 {
    class SegTree {
        struct Node {
            int begin, end;
            int max = -inf, lmax = -inf, rmax = -inf;
            int tot = 0;
        };
        std::vector<Node> tr;
        std::vector<int> initializer;
#define ls (p<<1)
#define rs (p<<1 | 1)
        static Node &merge(Node &root, Node const &left, Node const &right) {
            root.max = std::max({left.max, right.max, left.rmax + right.lmax});
            root.lmax = left.lmax, root.rmax = right.rmax;
            root.tot = left.tot + right.tot;
            chkMax(root.lmax, left.tot + right.lmax);
            chkMax(root.rmax, right.tot + left.rmax);
            return root;
        }
        void push_up(int p) {
            merge(tr[p], tr[ls], tr[rs]);
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].lmax = tr[p].rmax = tr[p].max = tr[p].tot = initializer.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            push_up(p);
        }
    public:
        SegTree(std::vector<int> &&init): tr(init.size() << 2), initializer(std::move(init)) {
            build(0, initializer.size());
            std::vector<int>().swap(initializer);
        }
        void modify(int pos, int val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].lmax = tr[p].rmax = tr[p].max = tr[p].tot = val;
                return;
            }
            if (tr[rs].begin <= pos)  modify(pos, val, rs);
            else  modify(pos, val, ls);
            push_up(p);
        }
        Node query(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p];
            }
            Node ans = {begin, end};
            Node ans_l = ans, ans_r = ans;
            if (tr[ls].end > begin)  ans_l = query(begin, end, ls);
            if (tr[rs].begin < end)  ans_r = query(begin, end, rs);
            return merge(ans, ans_l, ans_r);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N;  std::cin >> N;
        std::vector<int> init(N+1);
        for (auto &i: init | views::drop(1))  std::cin >> i;
        SegTree seg(std::move(init));

        int M;  std::cin >> M;
        // int M = 1;
        for (auto _: range(M)) {
            int op, x, y;  std::cin >> op >> x >> y;
            // int l = 1, r = N;
            if (op == 0) {
                seg.modify(x, y);
            } else {
                auto ans = seg.query(x, y+1);
                std::cout << ans.max << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1730635379514186::solve();
    return 0;
}
