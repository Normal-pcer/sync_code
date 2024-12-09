/**
 * @link https://www.luogu.com.cn/problem/P4145
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_7811334411471336 {
    class SegTree {
        struct Node {
            int l, r;
            ll sum = 0, max = -infLL;
        };
        std::vector<Node> tr;
        std::vector<ll> &init;

#define ls (p<<1)
#define rs (p<<1 | 1)
        void push_up(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void build(int begin, int end, int p) {
            tr[p].l = begin, tr[p].r = end;
            if (begin + 1 == end) {
                tr[p].max = tr[p].sum = init.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            push_up(p);
        }
    public:
        SegTree(std::vector<ll> init): tr(init.size() << 2), init(init) {
            build(0, init.size(), 1);
        }
        void batchSqrt(int begin, int end, int p = 1) {
            if (tr[p].l >= begin and tr[p].r <= end and tr[p].max == 1) {
                return;
            }
            if (tr[p].l + 1 == tr[p].r) {
                tr[p].max = tr[p].sum = sqrt(tr[p].sum);
                return;
            }
            if (tr[ls].r > begin)  batchSqrt(begin, end, ls);
            if (tr[rs].l < end)  batchSqrt(begin, end, rs);
            push_up(p);
        }
        ll sum(int begin, int end, int p = 1) {
            if (tr[p].l >= begin and tr[p].r <= end) {
                return tr[p].sum;
            }
            auto ans = 0LL;
            if (tr[ls].r > begin)  ans += sum(begin, end, ls);
            if (tr[rs].l < end)  ans += sum(begin, end, rs);
            return ans;
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;
        std::cin >> N;
        std::vector<ll> init(N+1);
        for (auto &i: init | views::drop(1))  std::cin >> i;
        SegTree seg(init);
        std::cin >> M;
        for (auto _: range(M)) {
            int k, l, r;  std::cin >> k >> l >> r;
            if (r < l)  std::swap(l, r);
            if (k == 0) {
                seg.batchSqrt(l, r+1);
            } else {
                std::cout << seg.sum(l, r+1) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1]);
    Solution_7811334411471336::solve();
    return 0;
}
