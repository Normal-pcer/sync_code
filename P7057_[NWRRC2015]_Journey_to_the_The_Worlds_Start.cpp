/**
 * @link https://www.luogu.com.cn/problem/P7057
 */

#include "./lib"

#include "./libs/binary.hpp"

using namespace lib;

namespace Solution_1133362793174219 {
    class SegTree {
        struct Node {
            int begin, end;
            int min;
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1|1)
        void pushUp(int p) {
            tr[p].min = std::min(tr[ls].min, tr[rs].min);
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 3) {
            build(begin, end, 1);
        }
        int min(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].min;
            }
            auto res = inf;
            if (tr[ls].end > begin)  chkMin(res, min(begin, end, ls));
            if (tr[rs].begin < end)  chkMin(res, min(begin, end, rs));
            return res;
        }
        void update(int pos, int val, int p = 1) {
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
                chkMin(tr[p].min, val);
                return;
            }
            if (tr[rs].begin <= pos)  update(pos, val, rs);
            else  update(pos, val, ls);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, T;  std::cin >> N >> T;
        std::vector<int> p(N+1);
        for (auto &i: p | views::drop(1))  std::cin >> i;
        std::vector<int> d(N+1);
        for (auto &i: d | views::take(N) | views::drop(2))  std::cin >> i;

        auto mapping = [&](int r) {
            std::vector<int> F(N+1);
            SegTree sgt(0, N+5);
            for (auto i = 2; i <= N; i++) {
                F.at(i) = sgt.min(std::max(1, i-r), i) + d.at(i);
                sgt.update(i, F.at(i));
            }
            return F.at(N);
        };
        auto ans = *std::min_element(p.begin() + binary::lower_bound_mapping(1, N, T, mapping), p.end());
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1133362793174219::solve();
    return 0;
}
