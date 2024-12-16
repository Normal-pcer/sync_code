/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3344
 */

#include "./lib"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1201453527095275 {
    std::vector<int> values;
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
                tr[p].tag += val;
                assert(tr[p].tag >= 0);
                if (tr[p].tag == 0) {
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
    class RectCounter {
    public:
        struct Rect {
            int x1, y1, x2, y2;
        };
        std::vector<Rect> shapes;

        RectCounter(std::vector<Rect> &&shapes): shapes(std::move(shapes)) {}
        RectCounter(): shapes() {}
        auto count() -> ll;
    };
    auto _forceCount(RectCounter::Rect rect, int flag) -> ll {
        bool x_flag = flag & 1, y_flag = flag >> 1;
        auto [x1, y1, x2, y2] = rect;

        auto ans = 0LL;
        for (auto x = x1; x != x2; x++) {
            for (auto y = y1; y != y2; y++) {
                if ((x&1) == x_flag and (y&1) == y_flag)  ans++;
            }
        }
        return ans;
    }
    auto _forceAreaUnion(std::vector<RectCounter::Rect> const &shapes) -> ll {
        std::vector<std::vector<int8_t>> flat(24, std::vector<int8_t>(24));
        for (auto [x1, y1, x2, y2]: shapes) {
            for (auto i = x1; i != x2; i++) {
                for (auto j = y1; j != y2; j++) {
                    flat.at(i).at(j) = true;
                }
            }
        }
        auto ans = 0LL;
        for (auto const &line: flat) {
            for (auto item: line) {
                ans += !!item;
            }
        }
        return ans;
    }
    auto RectCounter::count() -> ll {
        if (shapes.empty())  return 0;
        auto _force_ans = DEBUG_MODE? _forceAreaUnion(shapes): 0LL;
        struct Query {
            int pos;
            int begin, end;
            int val;
        };
        std::vector<Query> queries;

        auto min_x = inf, max_x = -inf;
        values.clear();
        for (auto [x1, y1, x2, y2]: shapes) {
            for (auto i: {x1, y1, x2, y2})  values.push_back(i);
        }
        std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());
        for (auto [x1, y1, x2, y2]: shapes) {
            for (auto i: {&x1, &y1, &x2, &y2})  *i = getIndex(*i);
            queries.push_back({y1, x1, x2, +1});
            queries.push_back({y2, x1, x2, -1});
            chkMax(max_x, x2), chkMin(min_x, x1);
        }

        std::sort(queries.begin(), queries.end(), lam(x, y, x.pos == y.pos? x.val > y.val: x.pos < y.pos));
        auto prev_pos = 0;
        auto ans = 0LL;
        SegTree sgt(min_x, max_x+1);
        for (auto [pos, begin, end, val]: queries) {
            auto dy = values.at(pos) - values.at(prev_pos);
            auto x = sgt.count();
            auto cur = (ll)dy * x;
            ans += cur, prev_pos = pos;

            sgt.add(begin, end, val);
        }
        debug  assert(_force_ans == ans);
        return ans;
    }
    // x % 2 å’Œ y % 2
    auto mapping(RectCounter::Rect origin, int flag) -> RectCounter::Rect {
        bool x_flag = flag & 1, y_flag = flag >> 1;
        auto [x1, y1, x2, y2] = origin;
        x1 = ((x1 - !x_flag) >> 1) + !x_flag;
        x2 = ((x2 - !x_flag) >> 1) + !x_flag;
        y1 = ((y1 - !y_flag) >> 1) + !y_flag;
        y2 = ((y2 - !y_flag) >> 1) + !y_flag;

        // debug  std::cout << std::format("{} {} {} {}: {}  {} <=> {}", x1, y1, x2, y2, flag, _forceCount(origin, flag), _forceCount({x1 << 1, y1 << 1, x2 << 1, y2 << 1}, flag)) << std::endl;
        debug  assert(_forceCount(origin, flag) == _forceCount({x1 << 1, y1 << 1, x2 << 1, y2 << 1}, flag));
        return {x1, y1, x2, y2};
    }
    void solve() {
        int N;  io >> N;
        std::vector<std::vector<RectCounter::Rect>> rect(4);

        for (auto i = 0; i < N; i++) {
            int x1, y1, x2, y2, op;
            io >> x1 >> y1 >> x2 >> y2 >> op;

            auto x_flag = x1 & 1, y_flag = y1 & 1, flag = x_flag ^ y_flag;

            RectCounter::Rect origin{x1, y1, x2, y2};
            
            for (auto i: [&]() -> std::vector<int> {
                switch (op) {
                case 1:
                    return {0, 1, 2, 3};
                case 2:
                    return {0b11 ^ (y_flag << 1), 0b10 ^ (y_flag << 1)};
                case 3:
                    return {0b11 ^ (x_flag), 0b01 ^ (x_flag)};
                default:
                    return {0b00 ^ (flag), 0b11 ^ (flag)};
                }
            }()) {
                rect.at(i).push_back(mapping(origin, i));
            }
        }
        
        auto ans = 0LL;
        for (auto i: {0, 1, 2, 3}) {
            RectCounter counter(std::move(rect.at(i)));
            ans += counter.count();
            debug  io << ans << endl;
        }

        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1201453527095275::solve();
    return 0;
}
