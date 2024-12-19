/**
 * @link https://www.luogu.com.cn/problem/P2605
 */

#include "lib"
#include "libs/range.hpp"
using namespace lib;

namespace Solution_6006738937570677 {
    class SegTree {
        struct Node {
            int begin, end;
            int min = inf, add_tag;
        };
        std::vector<Node> tr;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].min = std::min(tr[ls].min, tr[rs].min);
        }
        void addNode(int p, int val) {
            tr[p].min += val;
            tr[p].add_tag += val;
        }
        void pushDown(int p) {
            if (tr[p].add_tag) {
                for (auto s: {ls, rs}) {
                    addNode(s, tr[p].add_tag);
                }
                tr[p].add_tag = 0;
            }
        }
        void build(int begin, int end, int p) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 2) {
            build(begin, end, 1);
        }
        int min(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].min;
            }
            auto res = inf;
            pushDown(p);
            if (tr[ls].end > begin)  chkMin(res, min(begin, end, ls));
            if (tr[rs].begin < end)  chkMin(res, min(begin, end, rs));
            return res;
        }
        void add(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[ls].end > begin)  add(begin, end, val, ls);
            if (tr[rs].begin < end)  add(begin, end, val, rs);
            pushUp(p);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        struct Village {
            int pos, cost, range, indemnity;
        };
        std::vector<Village> villages(N);
        for (auto &[d, c, r, w]: villages)  std::cin >> d >> c >> r >> w;

        struct VillagesStatistics {
            int sum;
            std::vector<Village> items;
        };
        std::vector<VillagesStatistics> ends(villages.size()+1);  // 接受范围向右在 i 结束
        for (auto [pos, c, r, indemnity]: villages) {
            auto max_pos = pos + r;
            auto it = ranges::upper_bound(villages, max_pos, std::less{}, lam(x, x.pos));
            auto index = std::distance(villages.begin(), it);
            ends.at(index).items.push_back({pos, c, r, indemnity});
            ends.at(index).sum += indemnity;
        }

        std::vector F(K+1, std::vector<int>(N));
        for (auto j: range(1, K+1)) {
            for (auto i: range(N)) {
                
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6006738937570677::solve();
    return 0;
}
