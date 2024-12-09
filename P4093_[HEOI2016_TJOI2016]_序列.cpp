/**
 * @link https://www.luogu.com.cn/problem/P4093
 */

#include "./lib_v2.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_4879628616156045 {

    class BIT {
        struct Node {
            int val, version;
        };

        std::vector<Node> c;
        int current;

        inline int constexpr lowbit(int x) const noexcept { return x & -x; }
    public:
        BIT(size_t N): c(N+1), current(0) {}

        void update(int x, int val) {
            x++;
            while (x < (int)c.size()) {
                if (c.at(x).version == current)  chkMax(c.at(x).val, val);
                else  c.at(x) = {.val = val, .version = current};
                x += lowbit(x);
            }
        }

        int query(int x) const {
            x++; int res = -inf;
            while (x) {
                if (c.at(x).version == current)  chkMax(res, c.at(x).val);
                x -= lowbit(x);
            }
            return res;
        }

        void tick() {
            current++;
        }
    } tree(0);

    std::vector<int> F;

    struct Element {
        int val = 0, min = inf, max = -inf;
        int index = 0;
    };

    std::vector<Element> ele;

    void cdq(decltype(ele)::iterator begin, decltype(ele)::iterator end) {
        debug std::cout << std::format("cdq({}, {})", begin->index, end->index) << std::endl;
        if (begin + 1 == end) {
            debug std::cout << "return" << std::endl;
            return;
        }

        auto mid = begin + (std::distance(begin, end) >> 1);
        auto i = begin, j = mid;
        
        cdq(begin, mid), tree.tick();
        std::sort(begin, mid, lam(x, y, x.max < y.max));
        std::sort(mid, end, lam(x, y, x.val < y.val));

        while (i != mid or j != end) {
            if (j == end or (i != mid and i->max <= j->val)) {
                debug std::cout << std::format("update({}, {})", i->val, F.at(i->index)) << std::endl;
                tree.update(i->val, F.at(i->index));
                i++;
            } else {
                debug std::cout << std::format("query({}) -> {}", j->min, tree.query(j->min)) << std::endl;
                chkMax(F.at(j->index), tree.query(j->min) + 1);
                j++;
            }
        }

        std::sort(mid, end, lam(x, y, x.index < y.index));
        tree.tick(), cdq(mid, end);
        debug std::cout << std::format("cdq({}, {}) out", begin->index, end->index) << std::endl;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        F.resize(N+1, 1);
        ele.resize(N+1);

        for (auto i: range(1, N+1)) {
            std::cin >> ele[i].val, ele[i].index = i;
            ele[i].min = ele[i].max = ele[i].val;
        }

        for (auto _: range(1, M+1)) {
            int x, y;  std::cin >> x >> y;
            chkMax(ele.at(x).max, y), chkMin(ele.at(x).min, y);
        }

        ele.erase(ele.begin());

        // auto max = std::max_element(ele.begin(), ele.end(), lam(x, y, x.max < y.max))->val;
        tree = BIT(N+5);

        cdq(ele.begin(), ele.end());
        debug {
            for (auto i: F)  std::cout << i << ' ';
            std::cout << std::endl;
        }

        std::cout << *std::max_element(F.begin(), F.end()) << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_4879628616156045::solve();
    return 0;
}
