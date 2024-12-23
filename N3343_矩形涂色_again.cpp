/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3343
 * @link https://www.luogu.com.cn/problem/P5490
 */

#include "./lib_v3.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_5124327814186457 {

    std::vector<int> values;

    class SegTree {
        struct Node {
            int begin, end;
            int cnt;  // 被覆盖的长度
            int tag;  // 最小的覆盖次数
        };

        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1|1)
        void pushUp(int p) {
            if (tr[p].tag == 0) {  // 没有完全覆盖
                tr[p].cnt = tr[ls].cnt + tr[rs].cnt;
            }
        }
        void build(int begin, int end, int p) { 
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = begin + ((end - begin) >> 1);
            build(begin, mid, ls), build(mid, end, rs);
        }
    public:
        SegTree(int begin, int end): tr((end - begin) << 3) {
            build(begin, end, 1);
        }
        int count() const { return tr[1].cnt; }
        void add(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                tr[p].tag += val, assert(tr[p].tag >= 0);
                if (tr[p].tag == 0) {  // 现在有一些未覆盖的
                    tr[p].cnt = tr[ls].cnt + tr[rs].cnt;
                } else {
                    tr[p].cnt = values.at(tr[p].end) - values.at(tr[p].begin);
                }
                return;
            }
            if (tr[ls].end > begin)  add(begin, end, val, ls);
            if (tr[rs].begin < end)  add(begin, end, val, rs);
            pushUp(p);
        }
#undef ls
#undef rs
    };

    int getIndex(int x) {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x);
        return std::distance(values.begin(), it);
    }

    void solve() {
        int N;  io >> N;
        struct Rect {
            int x1, x2;
            int y1, y2;
        };
        std::vector<Rect> rectangles(N);
        for (auto &[a, b, c, d]: rectangles) {
            io >> a >> c >> b >> d;
            values.push_back(a), values.push_back(b);
            values.push_back(c), values.push_back(d);
        }

        std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());
        debug {
            std::cout << "values: " << std::endl;
            for (auto i: values)  std::cout << i << ' ';
            std::cout << std::endl;
        }

        struct Query {
            int pos;
            int begin, end;
            int val;
        };
        std::vector<Query> queries;
        queries.reserve(N);
        for (auto [x1, x2, y1, y2]: rectangles) {
            x1 = getIndex(x1), x2 = getIndex(x2);
            y1 = getIndex(y1), y2 = getIndex(y2);
            queries.push_back({y1, x1, x2, +1});
            queries.push_back({y2, x1, x2, -1});
        }

        std::sort(queries.begin(), queries.end(), lam(x, y, x.pos < y.pos));
        SegTree sgt(0, values.size());
        auto prev_pos = 0;
        auto ans = 0LL;
        for (auto [pos, begin, end, val]: queries) {
            auto len = sgt.count();
            auto dy = values.at(pos) - values.at(prev_pos);
            auto cur = (ll)dy * len;
            // debug  std::cout << std::format("{{{}, {}, {}, {}}}: ", pos, begin, end, val) << std::endl; 
            // debug  std::cout << std::format("{} += {} * {}", ans, dy, len) << std::endl;
            ans += cur, prev_pos = pos;

            sgt.add(begin, end, val);
        }

        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5124327814186457::solve();
    return 0;
}
