/**
 * @link https://www.luogu.com.cn/problem/CF431E
 * @link https://codeforces.com/problemset/problem/431/E
 */

#include "./lib"

using namespace lib;

namespace Solution_1129948889784761 {
    class SegTree {
        using value_type = ull;
        struct Node {
            int ls = 0, rs = 0;
            value_type cnt = 0, mercury = 0;
        };
        std::deque<Node> tr;

        value_type begin, end;
    public:
        SegTree(value_type begin, value_type end): tr(2), begin(begin), end(end) {}
#if false
        value_type lowerBoundHeight(value_type target, int p, value_type begin, value_type end) const {
            if (p == 0)  return begin;
            if (begin + 1 == end)  return begin;
            auto mid = std::midpoint(begin, end);
            auto ls = tr[tr[p].ls], rs = tr[tr[p].rs];
            if (mid * ls.cnt - ls.mercury <= target) {
                return lowerBoundHeight(target, tr[p].ls, begin, mid);
            } else {
                return lowerBoundHeight(target, tr[p].rs, mid, end);
            }
        }
#else
        value_type lowerBoundHeight(value_type target, int &p, value_type begin, value_type end, value_type cur_cnt, value_type cur_mercury) {
            debug std::cout << std::format("lowerBoundHeight({}, {}, {}, {}, {}, {})", target, p, begin, end, cur_cnt, cur_mercury) << std::endl;
            if (p == 0)  p = tr.size(), tr.push_back({});
            if (begin + 1 == end)  return begin;
            auto mid = std::midpoint(begin, end);
            auto ls = tr[tr[p].ls];
            bool flag = (double)mid * (double)(ls.cnt + cur_cnt) - (double)(ls.mercury + cur_mercury) > 1.8e19;
            debug std::cout
                << std::format ( "mid = {}, (ls.cnt + cur_cnt) = {}, mid * (ls.cnt + cur_cnt) = {}, ls.mercury + cur_mercury = {}, diff = {}, target = {}, flag = {}", 
                        mid, (ls.cnt + cur_cnt), mid * (ls.cnt + cur_cnt), ls.mercury + cur_mercury, 
                        mid * (ls.cnt + cur_cnt) - (ls.mercury + cur_mercury), target, flag
                    )
                << std::endl;
            if (flag or mid * (ls.cnt + cur_cnt) - (ls.mercury + cur_mercury) > target) {
                return lowerBoundHeight(target, tr[p].ls, begin, mid, cur_cnt, cur_mercury);
            } else {
                return lowerBoundHeight(target, tr[p].rs, mid, end, cur_cnt + ls.cnt, cur_mercury + ls.mercury);
            }
        }

        value_type lowerBoundHeight(value_type target) {
            int tmp = 1;
            return lowerBoundHeight(target, tmp, begin, end, 0, 0);
        }
#endif
        void erase(value_type val, int p, value_type begin, value_type end) {
            if (p == 0)  assert(false);
            tr[p].cnt -= 1, tr[p].mercury -= val;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            if (val < mid)  erase(val, tr[p].ls, begin, mid);
            else  erase(val, tr[p].rs, mid, end);
        }
        void erase(value_type val) { erase(val, 1, begin, end); }

        void insert(value_type val, int &p, value_type begin, value_type end) {
            if (p == 0)  p = tr.size(), tr.push_back({});
            tr[p].cnt += 1, tr[p].mercury += val;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            if (val < mid)  insert(val, tr[p].ls, begin, mid);
            else  insert(val, tr[p].rs, mid, end);
        }
        void insert(value_type val) {
            int tmp = 1;
            insert(val, tmp, begin, end);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<ull> init(N+1);
        const auto mul = 10000;
        SegTree seg(1, 18446744073709551615ULL);
        for (auto &i: init | views::drop(1))  std::cin >> i, i *= mul, seg.insert(i);
        init[0] = -inf;

        std::cout << std::fixed << std::setprecision(5);
        for (auto _: views::iota(0, M)) {
            int op;  std::cin >> op;
            if (op == 1) {
                ull p, x;
                std::cin >> p >> x, x *= mul;
                seg.erase(init.at(p)), seg.insert(x);
                init.at(p) = x;
            } else {
                ull v;  std::cin >> v;
                auto ans = seg.lowerBoundHeight(v * mul);
                std::cout << (double)ans / mul << endl;
            }
            debug std::cout << std::flush;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1129948889784761::solve();
    return 0;
}
