/**
 * @link https://www.luogu.com.cn/problem/CF474E
 */

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_5875700393430805 {
    std::vector<ll> values;
    auto getIndex(ll x) -> int {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x);
        return std::distance(values.begin(), it);
    };
    struct MaxItem {
        int val, source;
        std::weak_ordering operator<=> (const MaxItem &other) const noexcept {
            return val <=> other.val;
        }
        operator std::string () const {
            return std::format("MaxItem({}, {})", val, source);
        }
    };
}

using Solution_5875700393430805::MaxItem;

template <typename _CharT>
struct std::formatter<MaxItem, _CharT>: std::formatter<std::string, _CharT> {
    using base_t = std::formatter<std::string, _CharT>;

    auto format(auto const &v, auto &context) const {
        return base_t::format((std::string)v, context);
    }
};

namespace Solution_5875700393430805 {
    class SegTree {
        struct Node {
            int begin, end;
            MaxItem max;
        };
        std::vector<Node> tr;
#define ls (p<<1)
#define rs (p<<1|1)
        void pushUp(int p) {
            tr[p].max = std::max(tr[ls].max, tr[rs].max);
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].max = {0, begin};
                return;
            }
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
            auto res = MaxItem{-inf, -1};
            if (tr[ls].end > begin)  chkMax(res, max(begin, end, ls));
            if (tr[rs].begin < end)  chkMax(res, max(begin, end, rs));
            return res;
        }
        void update(int pos, MaxItem val, int p = 1) {
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

        int N, D;  std::cin >> N >> D;
        std::vector<ll> a(N);
        for (auto &i: a)  std::cin >> i, values.push_back(i);
        values.push_back(infLL);
        std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());
        debug {
            std::cout << "Values:" << endl;
            for (auto i: range(values.size()))  std::cout << std::format("[{}]{}", i, values[i]) << ' ';
            std::cout << std::endl;
        }
        for (auto &i: a)  i = getIndex(i);

        std::vector<MaxItem> F(a.size());
        SegTree sgt(0, values.size());

        auto ans = MaxItem{0, 0};
        for (auto i: range(a.size())) {
            auto it1 = std::lower_bound(values.begin(), values.end(), values.at(a[i]) - D + 1);
            auto it2 = std::upper_bound(values.begin(), values.end(), values.at(a[i]) + D - 1);
            auto cur = MaxItem{0, -1};
            chkMax(cur, sgt.max(0, std::distance(values.begin(), it1)));
            chkMax(cur, sgt.max(std::distance(values.begin(), it2), values.size()));
            // debug  std::cout << std::format("sgt.max({}, {}) -> {}", std::distance(values.begin(), min_it), std::distance(values.begin(), max_it), cur) << std::endl;
            cur.val += 1;
            F[i] = cur, chkMax(ans, {cur.val, (int)i});
            sgt.update(a[i], {cur.val, (int)i});
            debug  std::cout << std::format("sgt.update(pos={}, val={})", a[i], MaxItem{cur.val, (int)i}) << std::endl;
        }

        debug {
            std::cout << "F:" << std::endl;
            for (auto i: range(F.size()))  std::cout << std::format("[{}]{}", i, F[i]) << ' ';
            std::cout << std::endl;
        }

        std::vector<int> steps {ans.source};
        while (F.at(steps.back()).source != -1) {
            auto item = F.at(steps.back());
            steps.push_back(item.source);
        }
        std::reverse(steps.begin(), steps.end());

        std::cout << steps.size() << endl;
        for (auto i: steps)  std::cout << i+1 << ' ';
        std::cout << endl;

        auto prev = steps.front();
        for (auto i: steps | views::drop(1)) {
            assert(abs(values.at(a.at(i)) - values.at(a.at(prev))) >= D);
            prev = i;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5875700393430805::solve();
    return 0;
}
