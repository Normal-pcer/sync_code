/**
 * @link https://www.luogu.com.cn/problem/P3431
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_3801916692981172 {
    class SegTree {
        struct Node {
            int begin, end;
            int max;
        };
        std::vector<Node> tr;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
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
        int max(int begin, int end, int p = 1) const {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            auto res = -inf;
            if (tr[ls].end > begin)  chkMax(res, max(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, max(begin, end, rs));
            return res;
        }
        void update(int pos, int val, int p = 1) {
            if (tr[p].begin == pos and pos + 1 == tr[p].end) {
                chkMax(tr[p].max, val);
                return;
            }
            if (tr[ls].end > pos)  update(pos, val, ls);
            else  update(pos, val, rs);
            pushUp(p);
        }
#undef ls
#undef rs
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        struct Point {
            int x, y;
            int cnt;
            auto operator<=> (const Point &) const = default;
        };

        std::vector<int> values;
        auto get_index = [&](int x) -> int {
            auto it = std::lower_bound(values.begin(), values.end(), x);
            assert(*it == x);
            return std::distance(values.begin(), it);
        };

        int N, M, K;  std::cin >> N >> M >> K;
        std::vector<Point> points(K);
        for (auto &[x, y, cnt]: points)  std::cin >> x >> y >> cnt, values.push_back(x), values.push_back(y);
        std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());

        for (auto &[x, y, cnt]: points) {
            for (auto i: {&x, &y})  *i = get_index(*i);
        }
        std::sort(points.begin(), points.end());
        SegTree sgt(0, (int)values.size());
        for (auto [x, y, cnt]: points) {
            auto cur = cnt;
            cur += sgt.max(0, y+1);
            sgt.update(y, cur);

            debug  std::cout << std::format("{} {} {}: {}", x, y, cnt, cur) << std::endl;
        }
        auto ans = sgt.max(0, values.size());
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3801916692981172::solve();
    return 0;
}
