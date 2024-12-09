/**
 * @link https://www.luogu.com.cn/problem/P4169
 */

#include "./lib_v2.hpp"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_8035759900442287 {
    class BIT {
        struct Node {
            int val;
            int version;
        };
        std::vector<Node> c;
        int current;

        constexpr inline int lowbit(int x) const noexcept { return x & -x; }
    public:
        BIT(size_t N = 0): c(N+1), current(0) {}
        int query(int x) const {
            x++;  auto res = -inf;
            while (x) {
                if (c.at(x).version == current) {
                    chkMax(res, c.at(x).val);
                }
                x -= lowbit(x);
            }
            return res;
        }
        void update(int x, int val) {
            x++;
            while (x < (int)c.size()) {
                if (c.at(x).version != current)  c.at(x) = {val, current};
                else  chkMax(c.at(x).val, val);
                x += lowbit(x);
            }
        }
        void tick() { current++; }
    } tree;

    struct Node {
        enum Type { Query, Update } type;
        int t, x, y;

        Node(): type(Query), t(0), x(0), y(0) {}
        Node(Type type, int t, int x, int y): type(type), t(t), x(x), y(y) {}

        operator std::string() const {
            return std::format("Node(type={}, t={}, {}, {})", type==Query? " Query": "Update", t, x, y);
        }
    };

    std::vector<Node> a;
    std::vector<int> ans;  // t 为 i 时的询问的答案

    // 计算左下角范围的最近点
    void cdq(int begin, int end) {
        if (begin + 1 == end)  return;
        debug io << std::format("cdq({}, {})", begin, end) << endl;

        auto mid = (begin + end) >> 1;
        tree.tick(), cdq(begin, mid);
        tree.tick(), cdq(mid, end);
        tree.tick();

        auto i = begin, j = mid, k = 0;
        std::vector<Node> tmp(end - begin);
        while (i != mid or j != end) {
            if (j == end or (i != mid and a.at(i).x <= a.at(j).x)) {
                if (a.at(i).type == Node::Update) {
                    tree.update(a.at(i).y, a.at(i).y + a.at(i).x);
                }
                tmp.at(k++) = a.at(i++);
            } else {
                if (a.at(j).type == Node::Query) {
                    auto [_, t, x, y] = a.at(j);
                    auto max_pos = tree.query(y);
                    debug io << "update ans " << t << endl;
                    chkMin(ans.at(t), x + y - max_pos);
                }
                tmp.at(k++) = a.at(j++);
            }
        }

        std::copy(tmp.begin(), tmp.end(), a.begin() + begin);
    }

    void solve() {
        int N, M;  io >> N >> M;

        a.resize(N+M), ans.resize(N+M, inf);
        auto max_x = -1, max_y = -1;

        for (auto i: range(N)) {
            int x, y;  io >> x >> y;
            a.at(i) = Node(Node::Update, i, x, y);
            chkMax(max_x, x), chkMax(max_y, y);
        }

        std::vector<int> queies;
        for (auto i: range(N, N+M)) {
            int op, x, y;  io >> op >> x >> y;
            if (op == 1)  a.at(i) = Node(Node::Update, i, x, y);
            else  a.at(i) = Node(Node::Query, i, x, y), queies.push_back(i);
            chkMax(max_x, x), chkMax(max_y, y);
        }

        auto max = std::max(max_x, max_y) + 1;
        tree = BIT(max);

        for (auto _: range(4)) {
            std::sort(a.begin(), a.end(), lam(a, b, a.t < b.t));
            debug for (auto i: a)  io << (std::string)i << '\n'; 
            cdq(0, a.size());

            for (auto &[type, t, x, y]: a) {
                auto new_x = max - y, new_y = x;
                x = new_x, y = new_y;
            }
        }

        for (auto i: queies)  io << ans.at(i) << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
#ifndef ONLINE_JUDGE
    std::freopen("1.in", "r", stdin);
    std::freopen("1.out", "w", stdout);
#endif
    Solution_8035759900442287::solve();
    return 0;
}
