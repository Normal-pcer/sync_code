/**
 * @link https://www.luogu.com.cn/problem/P7867
 */

#include "./lib"


#include "./libs/range.hpp"


using namespace lib;

namespace Solution_7735545344698334 {
    struct Segment {
        int l, r, val;
    };

    class SegTree {
        struct Node {
            int begin, end;
            ll max = 0, add_tag = 0;  // 查询：最大值；修改：累加
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1|1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void pushDown(int p) {
            if (tr[p].add_tag) {
                tr[ls].max += tr[p].add_tag, tr[rs].max += tr[p].add_tag;
                tr[ls].add_tag += tr[p].add_tag, tr[rs].add_tag += tr[p].add_tag;
                tr[p].add_tag = 0;
            }
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int N): tr((N+1)<<2) {
            build(0, N+1);
        }
        ll max(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            pushDown(p);
            auto res = -infLL;
            if (tr[ls].end > begin)  chkMax(res, max(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, max(begin, end, rs));
            return res;
        }
        void add(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].max += val, tr[p].add_tag += val;
                return;
            }
            pushDown(p);
            if (tr[ls].end > begin)  add(begin, end, val, ls);
            if (tr[rs].begin < end)  add(begin, end, val, rs);
            pushUp(p);
        }
        void assign(int pos, ll val, int p = 1) {
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
                tr[p].max = val;
                return;
            }
            pushDown(p);
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

        int N, M;  std::cin >> N >> M;
        std::vector<int> cost(N+2);
        for (auto &c: cost | views::drop(1) | views::take(N))  std::cin >> c;

        std::vector<Segment> segs(M);
        std::vector<std::vector<Segment>> right(N+1);
        for (auto &[l, r, val]: segs) {
            std::cin >> l >> r >> val;
        }
        std::sort(segs.begin(), segs.end(), lam(x, y, x.r==y.r? x.l>y.l: x.r>y.r));
        for (auto &[l, r, val]: segs) {
            right.at(r).push_back({l, r, val});
        }

        SegTree sgt(N+1);
        // auto ans = 0LL;
        std::vector<ll> F(N+1);
        for (auto i: range(1, N+1)) {
            for (auto [l, r, val]: right.at(i)) {
                assert(r == i);
                sgt.add(0, l, +val);
            }
            sgt.add(0, i, -cost.at(i));
            F.at(i) = std::max(F.at(i-1), sgt.max(0, i));
            debug  std::cout << F.at(i) << std::endl;
            sgt.assign(i, F.at(i));
        }
        std::cout << F.at(N) << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7735545344698334::solve();
    return 0;
}
