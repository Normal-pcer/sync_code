/**
 * @link https://www.luogu.com.cn/problem/CF438D
 * @link https://codeforces.com/problemset/problem/438/D
 */

#include "./lib"

using namespace lib;

namespace Solution_1629225122955841 {
    class SegTree {
        struct Node {
            int begin, end;
            int max, tag/*assign*/;
            ll sum;
        };
        std::vector<Node> tr;
        std::vector<int> &init;

#define ls (p<<1)
#define rs (p<<1|1)
        void push_up(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void push_down(int p) {
            if (tr[p].tag) {
                tr[ls].sum = (ll)(tr[ls].end - tr[ls].begin) * tr[p].tag;
                tr[rs].sum = (ll)(tr[rs].end - tr[rs].begin) * tr[p].tag;
                tr[ls].max = tr[rs].max = tr[p].tag;
                tr[ls].tag = tr[rs].tag = tr[p].tag;
                tr[p].tag = 0;
            }
        }
        void build(int begin, int end, int p) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].max = tr[p].sum = init.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            push_up(p);
        }
    public:
        SegTree(std::vector<int> &init): tr(init.size() << 2), init(init) {
            build(0, init.size(), 1);
        }
        void assign(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].tag = val;
                tr[p].sum = (ll)(tr[p].end - tr[p].begin) * tr[p].tag;
                tr[p].max = tr[p].tag;
                return;
            }
            push_down(p);
            if (tr[ls].end > begin)  assign(begin, end, val, ls);
            if (tr[rs].begin < end)  assign(begin, end, val, rs);
            push_up(p);
        }
        void modulo(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                if (tr[p].max < val)  return;  // ignore
            }
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].sum = tr[p].max = tr[p].sum % val;
                return;
            }
            push_down(p);
            if (tr[ls].end > begin)  modulo(begin, end, val, ls);
            if (tr[rs].begin < end)  modulo(begin, end, val, rs);
            push_up(p);
        }
        ll sum(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].sum;
            }
            push_down(p);
            auto ans = 0LL;
            if (tr[ls].end > begin)  ans += sum(begin, end, ls);
            if (tr[rs].begin < end)  ans += sum(begin, end, rs);
            return ans;
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<int> init(N+1);
        for (auto &i: init | views::drop(1))  std::cin >> i;
        SegTree seg(init);

        for (auto _: views::iota(0, M)) {
            int op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                std::cout << seg.sum(x, y+1) << endl;
            } else if (op == 2) {
                int z;  std::cin >> z;
                seg.modulo(x, y+1, z);
            } else {
                seg.assign(x, x+1, y);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1629225122955841::solve();
    return 0;
}
