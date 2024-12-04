/**
 * @link https://www.luogu.com.cn/problem/P3157
 */

#include "./lib"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1471250357606197 {
    class BIT {
        std::vector<ll> c;
        std::vector<std::pair<int, int>> log;

        inline constexpr int lowbit(int x) const noexcept { return x & -x; };
    public:
        BIT(size_t N = 0): c(N+2) {}
        ll query(int x) const {
            x++;  auto res = 0LL;
            while (x) {
                res += c.at(x);
                x -= lowbit(x);
            }
            return res;
        }

        void update(int x, int val) {
            log.emplace_back(x, val), x++;
            while (x < (int)c.size()) {
                c.at(x) += val;
                x += lowbit(x);
            }
        }

        void clear() {
            size_t size = log.size();
            for (auto i = 0; i != (int)size; i++) {
                auto [x, val] = log.at(i);
                update(x, -val);
            }
            log.clear();
        }
    } tree;

    struct Element {
        int i;  // 原始下标
        int x;  // 原始值
        int t;  // 删除次序
    };

    int N, M;
    std::vector<Element> ele;
    std::vector<int> pos;
    std::vector<int> expire;

    std::vector<ll> ans1;
    void cdq1(int begin, int end) {
        if (begin + 1 == end)  return;

        auto mid = (begin + end) >> 1;
        cdq1(begin, mid), cdq1(mid, end);

        auto i = begin, j = mid, k = 0;
        std::vector<Element> tmp(end - begin);
        tree.clear();
        while (i != mid or j != end) {
            if (j == end or (i != mid and ele.at(i).i <= ele.at(j).i)) {
                tree.update(ele.at(i).t, 1);
                tmp.at(k++) = ele.at(i++);
            } else {
                ans1.at(ele.at(j).i) += tree.query(ele.at(j).t);
                tmp.at(k++) = ele.at(j++);
            }
        }
        std::copy(tmp.begin(), tmp.end(), ele.begin() + begin);
    }

    std::vector<ll> ans2;
    void cdq2(int begin, int end) {
        if (begin + 1 == end)  return;

        auto mid = (begin + end) >> 1;
        cdq2(begin, mid), cdq2(mid, end);

        auto i = begin, j = mid, k = 0;
        std::vector<Element> tmp(end - begin);
        tree.clear();
        while (i != mid or j != end) {
            if (j == end or (i != mid and ele.at(i).i >= ele.at(j).i)) {
                tree.update(ele.at(i).t, 1);
                tmp.at(k++) = ele.at(i++);
            } else {
                ans2.at(ele.at(j).i) += tree.query(ele.at(j).t);
                tmp.at(k++) = ele.at(j++);
            }
        }
        std::copy(tmp.begin(), tmp.end(), ele.begin() + begin);
    }

    void solve() {
        io >> N >> M;
        ele.resize(N), pos.resize(N+1);
        for (auto i = 0; i != (int)ele.size(); i++) {
            ele[i].i = i;
            io >> ele[i].x;
            pos.at(ele[i].x) = i;
        }

        expire.resize(M);
        auto t_pt = N;
        for (auto i = 0; i != (int)expire.size(); i++)  io >> expire[i], ele.at(pos.at(expire[i])).t = t_pt--;
        t_pt = 0;
        for (auto i = 0; i != (int)ele.size(); i++) {
            if (ele[i].t == 0)  ele[i].t = t_pt++;
        }
        debug for (auto [i, x, t]: ele)  io << i << ' ' << x << ' ' << t << endl;

        tree = BIT(N);

        ans1.resize(N);
        std::sort(ele.begin(), ele.end(), lam(x, y, x.x > y.x));  // t_A < t_B
        cdq1(0, N);

        ans2.resize(N);
        std::sort(ele.begin(), ele.end(), lam(x, y, x.x < y.x));  // t_A > t_B
        cdq2(0, N);

        std::vector<ll> ans_total(N);
        std::transform(ans1.begin(), ans1.end(), ans2.begin(), ans_total.begin(), lam(x, y, x+y));

        debug {
            for (auto i: ans1)  io << i << ' ';
            io << endl;
            for (auto i: ans2)  io << i << ' ';
            io << endl;
            for (auto i: ans_total)  io << i << ' ';
            io << endl;
        }

        auto sum = std::accumulate(ans_total.begin(), ans_total.end(), 0LL);
        for (auto i = 0; i != M; i++) {
            io << sum << endl;
            sum -= ans_total.at(pos.at(expire.at(i)));
        }
    }
}

int main() {
    initDebug;
    Solution_1471250357606197::solve();
    return 0;
}
