/**
 * @link https://www.luogu.com.cn/problem/CF597C
 * @link https://codeforces.com/problemset/problem/597/C
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_4833578930085354 {
    class SegTree {
        struct Node {
            int begin, end;
            ll sum;
        };
        std::vector<Node> tr;

#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].sum = tr[ls].sum + tr[rs].sum;
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)   return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            pushUp(p);
        }
        
    public:
        SegTree(int N): tr((N+1)<<2) {
            build(0, N);
        }
        void add(int pos, ll val, int p = 1) {
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
                tr[p].sum += val;
                return;
            }
            if (tr[ls].end > pos)  add(pos, val, ls);
            else  add(pos, val, rs);
            pushUp(p);
        }
        auto sum(int begin, int end, int p = 1) -> ll {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].sum;
            }
            auto res = 0LL;
            if (tr[ls].end > begin)  res += sum(begin, end, ls);
            if (tr[rs].begin < end)  res += sum(begin, end, rs);
            return res;
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K, K++;
        std::vector<int> a(N);
        for (auto &i: a)  std::cin >> i;

        std::vector<SegTree> trees(K+1, SegTree(N+1));
        for (auto i: range(0, N)) {
            for (auto j: range(K, 1, -1)) {
                auto cur = trees.at(j-1).sum(0, a.at(i));
                trees.at(j).add(a.at(i), cur);
                debug  std::cout << std::format("trees.at({}).add({}, {})", j, a.at(i), cur) << std::endl;
            }
            trees.at(1).add(a.at(i), 1);
        }

        auto ans = trees.at(K).sum(0, N+1);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4833578930085354::solve();
    return 0;
}
