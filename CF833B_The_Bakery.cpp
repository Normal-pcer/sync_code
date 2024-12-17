/**
 * @link https://www.luogu.com.cn/problem/CF833B
 * @link https://codeforces.com/problemset/problem/833/B
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_2494025223377382 {
    class SegTree {
        struct Node {
            int begin, end;
            int max, add_tag;
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1 | 1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void pushDown(int p) {
            if (tr[p].add_tag) {
                for (auto s: {ls, rs}) {
                    tr[s].max += tr[p].add_tag;
                    tr[s].add_tag += tr[p].add_tag;
                }
                tr[p].add_tag = 0;
            }
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            pushUp(p);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        auto max(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            auto res = -inf;
            pushDown(p);
            if (tr[ls].end > begin)  chkMax(res, max(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, max(begin, end, rs));
            return res;
        }
        void add(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].max += val;
                tr[p].add_tag += val;
                return;
            }
            pushDown(p);
            if (tr[ls].end > begin)  add(begin, end, val, ls);
            if (tr[rs].begin < end)  add(begin, end, val, rs);
            pushUp(p);
        }
        void assign(int pos, int val, int p = 1) {
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

        int N, K;  std::cin >> N >> K;
        std::vector<int> a(N+1);
        for (auto &i: a | views::take(N))  std::cin >> i;

        std::vector sgt(K+1, SegTree(0, N+1));
        for (auto j: range(1, K+1)) {
            std::vector<int> last(N+1);  // [last_x, i) 范围内均没有 x
            for (auto i: range(N+1)) {
                auto cur = i? sgt.at(j-1).max(0, i): 0;  // 最大的 F[k][j-1] + kind[k][i] -> F[i][j]
                debug  std::cout << std::format("sgt[{}].max({}, {}) = {}", j-1, 0, i, cur) << std::endl;
                sgt.at(j).assign(i, cur);
                debug  std::cout << std::format("sgt[{}].assign({}, {})", j, i, cur) << std::endl;

                // 取点 i
                // [last_x, i] 范围都会产生贡献
                sgt.at(j-1).add(last.at(a[i]), i+1, +1);
                debug  std::cout << std::format("sgt[{}].add({}, {}, {})", j-1, last.at(a[i]), i+1, +1) << std::endl;
                last.at(a[i]) = i+1;
            }
        }

        // 答案即 F[N][K]
        auto ans = sgt.at(K).max(N, N+1);
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2494025223377382::solve();
    return 0;
}
