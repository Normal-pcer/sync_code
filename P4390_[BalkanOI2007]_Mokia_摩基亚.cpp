/**
 * @link https://www.luogu.com.cn/problem/P4390
 */

#include "./lib"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1151073895552120 {

    class BIT {
        struct Node {
            int val, version;
        };
        std::vector<Node> c;
        int current;
        constexpr int lowbit(const int x) const noexcept { return x & -x; }
    public:
        BIT(size_t N = 0): c(N+2), current(0) {}

        int query(int x) const {
            x++;  auto res = 0;
            while (x) {
                if (c.at(x).version == current) {
                    res += c.at(x).val;
                }
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
        enum Type { Query, Update } type;
        int t, x, y, cnt = 0;

        operator std::string() const {
            return std::format("Element({}, {}, {}, {}, {})", type == Query? " Query": "Update", t, x, y, cnt);
        }
    };

    std::vector<Element> ele;
    std::vector<int> ans;

    void cdq(int begin, int end) {
        if (begin + 1 == end)  return;

        auto mid = (begin + end) >> 1;
        cdq(begin, mid), tree.tick();
        cdq(mid, end), tree.tick();

        auto i = begin, j = mid, k = 0;
        std::vector<Element> tmp(end - begin);

        while (i != mid or j != end) {
            if (j == end or (i != mid and ele.at(i).x <= ele.at(j).x)) {
                if (ele.at(i).type == Element::Update) {
                    tree.update(ele.at(i).y, ele.at(i).cnt);
                }
                tmp.at(k++) = ele.at(i++);
            } else {
                if (ele.at(j).type == Element::Query) {
                    ans.at(ele.at(j).t) += tree.query(ele.at(j).y);
                }
                tmp.at(k++) = ele.at(j++);
            }
        }

        std::copy(tmp.begin(), tmp.end(), ele.begin() + begin);
    }

    void solve() {
        auto i = 0;
        std::vector<int> queries;
        while (true) {
            int op;  io >> op;
            if (op == 0) {
                int w;  io >> w;
                tree = BIT(w);
            } else if (op == 1) {
                int x, y, a;  io >> x >> y >> a;
                ele.emplace_back(Element::Update, i++, x, y, a);
            } else if (op == 2) {
                int x0, y0, x1, y1;  io >> x0 >> y0 >> x1 >> y1;
                queries.push_back(i);
                ele.emplace_back(Element::Query, i++, x1, y1);
                ele.emplace_back(Element::Query, i++, x0-1, y1);
                ele.emplace_back(Element::Query, i++, x1, y0-1);
                ele.emplace_back(Element::Query, i++, x0-1, y0-1);
            } else if (op == 3) {
                goto egg;
            }
        }
    egg:
        debug for (auto i: ele)  io << (std::string)i << endl;

        ans.resize(ele.size());
        // std::sort(ele.begin(), ele.end(), lam(x, y, x.t < y.t));
        cdq(0, ele.size());

        debug for (auto i: ans)  io << i << ' ';
        debug io << endl;

        auto query_seg = [&](int x) {
            auto a = ans.at(x), b = ans.at(x+1), c = ans.at(x+2), d = ans.at(x+3);
            return a - b - c + d;
        };

        for (auto x: queries) {
            io << query_seg(x) << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1151073895552120::solve();
    return 0;
}
