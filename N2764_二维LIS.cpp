/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=2764
 */

#include "./lib_v2.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_8886649387362907 {
    class BIT {
        struct Node {
            int val = -inf, version = 0;
        };
        std::vector<Node> c;
        int current;

        inline constexpr int lowbit(int x) const noexcept { return x & -x; }
    public:
        BIT(size_t N = 0): c(N+1), current(0) {}
        int query(int x) const {
            x++;  auto res = -inf;
            while (x) {
                if (c.at(x).version == current)  chkMax(res, c.at(x).val);
                x -= lowbit(x);
            }
            return res;
        }
        void update(int x, int val) {
            x++;
            while (x < (int)c.size()) {
                if (c.at(x).version == current)  chkMax(c.at(x).val, val);
                else  c.at(x) = {val, current};
                x += lowbit(x);
            }
        }
        void tick() {
            current++;
        }
    } tree;

    struct Element {
        int x, y;
        int index;
    };

    std::vector<Element> ele;
    std::vector<int> F;
#define format(...) flush
    void cdq(decltype(ele)::iterator begin, decltype(ele)::iterator end) {
        if (begin + 1 == end)  return;

        auto mid = begin + (std::distance(begin, end) >> 1);
        std::vector<Element> right_cp(mid, end);
        cdq(begin, mid), tree.tick();
        std::sort(begin, mid, lam(x, y, x.x < y.x));
        std::sort(mid, end, lam(x, y, x.x < y.x));

        auto i = begin, j = mid;
        debug std::cout << std::format("cdq({}, {})", begin - ele.begin(), end - ele.begin()) << std::endl;
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->x < j->x)) {
                debug std::cout << std::format("update([{}]{}, ({})F[{}])", i->index, i->y, F.at(i->index), i->index) << std::endl;
                tree.update(i->y, F.at(i->index));
                i++;
            } else {
                debug std::cout << std::format("query([{}]{}) -> {}", j->index, j->y - 1, tree.query(j->y - 1)) << std::endl;
                auto max = tree.query(j->y - 1);
                chkMax(F.at(j->index), max + 1);
                j++;
            }
        }

        std::copy(right_cp.begin(), right_cp.end(), mid);
        tree.tick(), cdq(mid, end);
    }

    void solve() {
        int N;  io >> N;
        ele.resize(N+1), F.resize(N+1, 1);
        auto max_y = -inf;
        for (auto i = 0; i < N; i++) {
            auto &[x, y, index] = ele[i];
            io >> x >> y, index = i;
            chkMax(max_y, y);
        }
        ele.back() = {inf, max_y + 2, N};

        tree = BIT(max_y + 2);
        cdq(ele.begin(), ele.end());
        io << F.at(N) - 1 << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_8886649387362907::solve();
    return 0;
}
