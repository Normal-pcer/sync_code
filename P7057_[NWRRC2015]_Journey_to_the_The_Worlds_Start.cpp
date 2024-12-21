/**
 * @link https://www.luogu.com.cn/problem/P7057
 */

#include "./lib"

#include "./libs/binary.hpp"

using namespace lib;

namespace Solution_1251875619187741 {
    class SegTree {
        struct Node {
            int begin, end;
            ll min = infLL;
        };
        std::vector<Node> tr;
#define ls (p << 1)
#define rs (p << 1 | 1)
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
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        ll min(int begin, int end, int p = 1) const {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].min;
            }
            auto res = infLL;
            if (tr[ls].end > begin)  chkMin(res, min(begin, end, ls));
            if (tr[rs].begin < end)  chkMin(res, min(begin, end, rs));
            return res;
        }
        void assign(int pos, ll val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].min = val;
                return;
            }
            if (tr[ls].end > pos)  assign(pos, val, ls);
            else  assign(pos, val, rs);
            pushUp(p);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, T;  std::cin >> N >> T;
        T -= N - 1;

        struct Ticket {
            int range, price;
        };
        std::vector<Ticket> tickets{{0, 0}};
        for (auto i = 1; i <= N - 1; i++) {
            int p;  std::cin >> p;
            tickets.emplace_back(i, p);
        } 

        std::vector<int> d(N);
        for (auto &i: d | views::take(N-1) | views::drop(1)) {
            std::cin >> i;
        }

        // range -> min time
        auto mapping = [&](int x) {
            SegTree sgt(0, N);
            sgt.assign(0, 0);
            for (auto i = 1; i < N; i++) {
                auto begin = std::max(0, i - x), end = i;
                auto min = sgt.min(begin, end);
                auto cur = min + d.at(i);
                sgt.assign(i, cur);
            }
            debug  std::cout << std::format("{} -> {}", x, -sgt.min(N-1, N)) << std::endl;
            return -sgt.min(N-1, N);
        };
        auto min_range = binary::lower_bound_mapping(1, N, -T, mapping);
        auto ans = inf;
        for (auto i = min_range; i < N; i++)  chkMin(ans, tickets.at(i).price);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1251875619187741::solve();
    return 0;
}
