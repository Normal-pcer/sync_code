/**
 * @link https://www.luogu.com.cn/problem/P1848
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_5926768708892818 {
    class SegTree {
        struct Node {
            int begin, end;
            ll max_h, min_ans, add_tag;
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1 | 1)
        void pushUp(int p) {
            tr[p].max_h = std::max(tr[ls].max_h, tr[rs].max_h);
            tr[p].min_ans = std::min(tr[ls].min_ans, tr[rs].min_ans);
        }
        void addNode(int p, int val) {
            tr[p].max_h += val;
            tr[p].min_ans += val;
            tr[p].add_tag += val;
        }
        void pushDown(int p) {
            if (tr[p].add_tag) {
                for (auto s: {ls, rs}) {
                    addNode(s, tr[p].add_tag);
                }
            }
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid), build(mid, end);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        ll maxH(int begin, int end, int p = 1) {
            
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, L;  std::cin >> N >> L;
        struct Book { int height, width; };
        std::vector<Book> books(N+1);
        for (auto &[h, w]: books | views::drop(1))  std::cin >> h >> w;
        std::vector<ll> h_ps(books.size()), w_ps(books.size());
        ranges::transform(books, h_ps.begin(), lam(x, x.height)), std::partial_sum(h_ps.begin(), h_ps.end(), h_ps.begin());
        ranges::transform(books, w_ps.begin(), lam(x, x.width)), std::partial_sum(w_ps.begin(), w_ps.end(), w_ps.begin());

        std::vector<ll> F(N+1, infLL);
        F.at(0) = 0;
        for (auto i: range(1, N+1)) {
            for (auto j: range(0, i)) {
                if (w_ps.at(i) - w_ps.at(j) <= L) {
                    auto max = 0;
                    for (auto k: range(j+1, i+1)) {
                        chkMax(max, books.at(k).height);
                    }
                    chkMin(F.at(i), F.at(j) + max);
                }
            }
        }
        std::cout << F.at(N) << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5926768708892818::solve();
    return 0;
}
