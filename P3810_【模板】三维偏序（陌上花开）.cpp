/**
 * @link https://www.luogu.com.cn/problem/P3810
 */

#include "./lib_v2.hpp"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1114822546759732 {

    struct Element {
        int a, b, c;

        std::strong_ordering operator<=> (const Element &other) const noexcept = default;
    };

    int current = 0;

    struct BIT {
        struct Node {
            int val, version;
        };
        std::vector<Node> c;

        BIT(size_t N): c(N+1) {}

        inline int constexpr lowbit(const int x) const noexcept { return x & -x; }
        int query(int x) const {
            x++;
            auto res = 0;
            while (x) {
                if (c.at(x).version == current)  res += c[x].val;
                x -= lowbit(x);
            }
            return res;
        }

        void update(int x, int val) {
            x++;
            while ((size_t)x < c.size()) {
                if (c.at(x).version != current)  c[x].version = current, c[x].val = 0;
                c.at(x).val += val;
                x += lowbit(x);
            }
        }
    };

    int N, K;
    std::vector<Element> ele;
    std::map<Element, int> ans;
    std::map<Element, int> count;

    void cdq(const int begin, const int end, BIT &tree) {
        if (begin + 1 == end) {
            ans[ele.at(begin)] += count.at(ele.at(begin));
            return;
        }

        auto mid = (begin + end) >> 1;
        // 计算左侧的答案
        cdq(begin, mid, tree), current++;
        cdq(mid, end, tree), current++;

        auto i = begin, j = mid, k = 0;
        std::vector<Element> tmp(end - begin);
        debug io << std::format("cdq({}, {})", begin, end) << endl;
        debug for (auto [x, y, z]: ele)  io << std::format("{{{}, {}, {}}} ", x, y, z) << ' ';
        debug io << endl;
        while (i != mid or j != end) {
            if (j == end or (i != mid and ele.at(i).b <= ele.at(j).b)) {
                tree.update(ele.at(i).c, count.at(ele.at(i)));
                tmp.at(k++) = ele.at(i++);
            } else {
                ans[ele.at(j)] += tree.query(ele.at(j).c);
                debug io << std::format("ans[({{{}, {}, {}}})ele[{}]] += {}", ele.at(j).a, ele.at(j).b, ele.at(j).c, j, tree.query(ele.at(j).c)) << endl;
                tmp.at(k++) = ele.at(j++);
            }
        }
        std::copy(tmp.begin(), tmp.end(), ele.begin() + begin);
    }

    void solve() {
        io >> N >> K;  ele.resize(N);
        for (auto &cur: ele) {
            auto &[a, b, c] = cur;
            io >> a >> b >> c;
            count[cur]++;
        }

        std::vector cpy(ele.begin(), ele.end());

        std::sort(ele.begin(), ele.end());
        ele.erase(std::unique(ele.begin(), ele.end()), ele.end());

        BIT tree(K+5);
        cdq(0, ele.size(), tree);

        std::vector<int> buc(N);

        for (auto e: cpy) {
            buc.at(ans[e]-1)++;
            debug io << ans[e] << endl;
        }
        debug io << endl;

        for (auto i: buc) {
            io << i << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1114822546759732::solve();
    return 0;
}
