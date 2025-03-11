/**
 * @link https://www.luogu.com.cn/problem/CF920F
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_2544484027294876 {
    i32 constexpr maxValue = 1e6 + 5;
    struct FactorCollection {
        std::vector<i32> cnt;
        
        FactorCollection(i32 N): cnt(N) {
            for (i32 i = 1; i < N; i++) {
                for (i32 j = i; j < N; j += i)  cnt[j]++;
            }
        }
    } fc{maxValue};
    class SegTree {
        struct Node {
            i32 begin = 0, end = 0;
            i32 max = 0;
            i64 sum = 0;
        };
        std::vector<Node> tree;

        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }

        auto pushUp(i32 p) {
            tree[p].max = std::max(tree[lson(p)].max, tree[rson(p)].max);
            tree[p].sum = tree[lson(p)].sum + tree[rson(p)].sum;
        }
        auto build(i32 begin, i32 end, i32 p, std::vector<i32> const &init) {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end) {
                tree[p].max = tree[p].sum = init.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p), init);
            build(mid, end, rson(p), init);
            pushUp(p);
        }
    public:
        SegTree(i32 N, std::vector<i32> const &init): tree((N + 1) << 2) {
            build(0, N, 1, init);
        }

        auto sumRange(i32 begin, i32 end, i32 p = 1) -> i64 {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p].sum;
            }
            i64 res = 0;
            if (tree[lson(p)].end > begin)  res += sumRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  res += sumRange(begin, end, rson(p));
            return res;
        }

        auto transformRange(i32 begin, i32 end, i32 p = 1) -> void {
            if (tree[p].max <= 2)  return;
            if (tree[p].begin + 1 == tree[p].end) {
                tree[p].max = tree[p].sum = fc.cnt[tree[p].sum];
                return;
            }
            if (tree[lson(p)].end > begin)  transformRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  transformRange(begin, end, rson(p));
            pushUp(p);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<i32> init(N);
        for (auto &x: init)  std::cin >> x;

        SegTree sgt{N, init};
        for (auto m = M; m --> 0; ) {
            i32 op, x, y;  std::cin >> op >> x >> y;
            if (op == 1) {
                sgt.transformRange(x - 1, y);
            } else {
                auto ans = sgt.sumRange(x - 1, y);
                std::cout << ans << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2544484027294876::solve();
    return 0;
}
