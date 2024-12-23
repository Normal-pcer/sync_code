/**
 * @link https://www.luogu.com.cn/problem/P1856
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_8190006491226031 {
    class SegTree {
        struct Node {
            int begin, end;
            int cnt, tag;
        };
        std::vector<Node> tr;
#define ls (p << 1)
#define rs (p << 1 | 1)
        void pushUp(int p) {
            if (tr[p].tag == 0) {
                tr[p].cnt = tr[ls].cnt + tr[rs].cnt;
            }
        }
        void build(int begin, int end, int p) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, ls), build(mid, end, rs);
            pushUp(p);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 3) {
            build(begin, end, 1);
        }
        int count() const { return tr[1].cnt; }
        // 返回：在此次操作中变成 0 的元素个数
        int add(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].tag += val, assert(tr[p].tag >= 0);
                if (tr[p].tag == 0) {
                    tr[p].cnt = tr[ls].cnt + tr[rs].cnt;
                    return tr[p].end - tr[p].begin - tr[p].cnt;
                } else {
                    tr[p].cnt = tr[p].end - tr[p].begin;
                    return 0;
                }
            }
            auto res = 0;
            if (tr[ls].end > begin)  res += add(begin, end, val, ls);
            if (tr[rs].begin < end)  res += add(begin, end, val, rs);
            pushUp(p);
            if (tr[p].tag != 0)  res = 0;
            return res;
        }
#undef ls
#undef rs
    };
    struct Rect {
        int x1, y1, x2, y2;
    };
    auto perimeter(std::vector<Rect> const &shapes) -> ll {
        if (shapes.empty())  return 0LL;
        struct Query {
            int pos;
            int begin, end;
            int val;

            auto operator<=>(const Query &other) const noexcept -> std::weak_ordering {
                if (auto cmp = pos <=> other.pos; cmp != 0)  return cmp;
                return other.val <=> val;
            }
        };
        std::vector<Query> queries;

        auto min_x = inf, max_x = -inf;
        for (auto [x1, y1, x2, y2]: shapes) {
            queries.push_back({y1, x1, x2, +1});
            queries.push_back({y2, x1, x2, -1});
            chkMin(min_x, x1), chkMax(max_x, x2);
        }
        std::sort(queries.begin(), queries.end());

        auto ans = 0LL;
        SegTree sgt(min_x, max_x);
        for (auto [pos, begin, end, val]: queries) {
            ans += sgt.add(begin, end, val);
        }
        return ans * 2;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<Rect> shapes(N);
        for (auto &[x1, y1, x2, y2]: shapes) {
            std::cin >> x1 >> y1 >> x2 >> y2;
        }
        auto ans = perimeter(shapes);  // 横向
        for (auto &[x1, y1, x2, y2]: shapes) {
            std::swap(x1, y1), std::swap(x2, y2);
        }
        ans += perimeter(shapes);  // 纵向
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8190006491226031::solve();
    return 0;
}
