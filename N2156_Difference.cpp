/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=2156
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_7891436569830078 {
    class SegTree {
        struct Node {
            i32 begin, end;
            i32 min = inf;
        };

        std::vector<Node> tree;
        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }

        auto pushUp(i32 p) -> void {
            tree[p].min = std::min(tree[lson(p)].min, tree[rson(p)].min);
        }
        auto build(i32 begin, i32 end, i32 p) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = (begin + end) >> 1;
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(i32 N): tree((N + 1) << 2) {
            build(0, N, 1);
        }

        auto minRange(i32 begin, i32 end, i32 p = 1) -> i32 {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p].min;
            }
            i32 ans = inf;
            if (tree[lson(p)].end > begin)  chkMin(ans, minRange(begin, end, lson(p)));
            if (tree[rson(p)].begin < end)  chkMin(ans, minRange(begin, end, rson(p)));
            return ans;
        }

        auto assignAt(i32 pos, i32 val, i32 p = 1) -> void {
            if (tree[p].begin + 1 == tree[p].end) {
                tree[p].min = val;
                return;
            }
            if (tree[lson(p)].end > pos)  assignAt(pos, val, lson(p));
            else  assignAt(pos, val, rson(p));
            pushUp(p);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        i32 N, Q;  std::cin >> N >> Q;

        std::vector<i32> vec(N);
        for (auto &x: vec)  std::cin >> x;
        std::vector<i32> buc(N + 1, inf);
        std::vector<i32> last(N, inf);
        for (i32 i = N; i --> 0; ) {
            last[i] = buc[vec[i]];
            buc[vec[i]] = i;
        }

        SegTree sgt{N};
        for (i32 i = 0; i < N; i++) {
            sgt.assignAt(i, last[i]);
        }

        for (auto q = Q; q --> 0; ) {
            i32 x, y;  std::cin >> x >> y;
            auto min = sgt.minRange(x - 1, y);
            auto flag = min >= y;
            if (flag)  std::cout << "Yes" << endl;
            else  std::cout << "No" << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7891436569830078::solve();
    return 0;
}
