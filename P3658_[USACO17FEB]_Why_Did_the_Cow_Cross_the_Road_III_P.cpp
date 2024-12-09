/**
 * @link https://www.luogu.com.cn/problem/P3658
 */

#include "./lib_v2.hpp"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1260153240037018 {

    class BIT {
        struct Node {
            ll val;
            int version;
        };
        std::vector<Node> c;
        int current = 0;

        constexpr inline int lowbit(int x) const noexcept { return x & -x; }
    public:
        BIT(size_t N = 0): c(N+1) {}
        ll query(int x) const {
            x++; auto res = 0LL;
            while (x) {
                if (c.at(x).version == current)  res += c.at(x).val;
                x -= lowbit(x);
            }
            return res;
        }

        void update(int x, int val) {
            x++;
            while (x < (int)c.size()) {
                if (c.at(x).version == current)  c.at(x).val += val;
                else  c.at(x) = {.val = val, .version = current};
                x += lowbit(x);
            }
        }

        void tick() { current++; }
    } tree;

    struct Element {
        int p, q, val;

        operator std::string() const {
            return std::format("Element({}, {}, {})", p, q, val);
        }
    };
    std::vector<Element> ele;

    int N, M;

    auto ans = 0LL;
    void cdq(int begin, int end) {
        if (begin + 1 == end)  return;
        auto mid = (begin + end) >> 1;
        cdq(begin, mid), tree.tick();
        cdq(mid, end), tree.tick();

        auto i = begin, j = mid, k = 0;
        std::vector<Element> tmp(end - begin);

        debug io << std::format("cdq({}, {})\n", begin, end);
        debug for (auto i: ele)  io << (std::string)i << "   ";
        debug io << endl;

        while (i != mid or j != end) {
            if (j == end or (i != mid and ele.at(i).p < ele.at(j).p)) {
                tree.update(ele.at(i).val, 1);
                debug io << "update " << ele.at(i).val << endl;
                tmp.at(k++) = ele.at(i++);
            } else {
                auto cur = tree.query(N);
                cur -= tree.query(std::min(ele.at(j).val + M, N));
                cur += tree.query(std::max(0, ele.at(j).val - M - 1));
                ans += cur;
                debug io << std::format("[0, N] - [0, {}] + [0, {}] -> {}", ele.at(j).val + M, ele.at(j).val - M - 1, cur) << endl;
                tmp.at(k++) = ele.at(j++);
            }
        }

        std::copy(tmp.begin(), tmp.end(), ele.begin() + begin);
    }

    void solve() {
        io >> N >> M;
        ele.resize(N);
        for (auto i: range(N)) {
            int x;  io >> x;
            ele.at(x-1) = {i, 0, x};
        }

        for (auto i: range(N)) {
            int x;  io >> x;
            ele.at(x-1).q = i;
        }

        tree = BIT(N+1);

        debug for (auto i: ele)  io << (std::string)i << endl;
        std::sort(ele.begin(), ele.end(), lam(x, y, x.q > y.q));
        cdq(0, N);

        io << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1260153240037018::solve();
    return 0;
}
