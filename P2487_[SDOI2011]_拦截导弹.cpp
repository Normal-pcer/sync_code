/**
 * @link https://www.luogu.com.cn/problem/P2487
 */

#include "./lib_v2.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_7021114588160689 {

    struct MaxItem {
        int val, pos;

        std::weak_ordering operator<=> (const MaxItem &other) const {
            return val <=> other.val;
        }
    };

    struct Statistic {
        int val; double cnt = 0.0;

        Statistic &update(Statistic const &other) {
            if (val < other.val)  val = other.val, cnt = other.cnt;
            else if (val == other.val)  cnt += other.cnt;
            return *this;
        }

        Statistic operator+ (int x) const {
            return {val + x, cnt};
        }

        operator std::string () const {
            return std::format("{}({})", val, cnt);
        }

        auto operator<=> (const Statistic &other) const = default;
    };

    std::vector<int> values;
    struct Element {
        int h, v;  // 映射后的高度和速度
        int index;  // 原始下标

        operator std::string () const {
            return std::format("Element({}, {}, {})", values.at(h), values.at(v), index);
        }
    };

    class BIT {
        struct Node {
            Statistic val;
            int version;
        };
        std::vector<Node> c;
        int current = 0;

        inline int constexpr lowbit(int x) const noexcept { return x & -x; }
    public:
        BIT(size_t N = 0): c(N+1), current(0) {}
        Statistic query(int x) const {
            x++; Statistic res = {-inf};
            while (x) {
                if (c.at(x).version == current)  res.update(c.at(x).val);
                x -= lowbit(x);
            }
            return res;
        }

        void update(int x, Statistic val) {
            x++;
            while (x < (int)c.size()) {
                if (c.at(x).version == current)  c.at(x).val.update(val);
                else  c.at(x) = {.val = val, .version = current};
                x += lowbit(x);
            }
        }

        void tick() { current++; }
    } tree;

    std::vector<Element> ele;

    inline int getIndex(int x) {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x);
        return it - values.begin();
    }

    int N;
    std::vector<Statistic> F, G;

    void cdq(int begin, int end) {
        if (begin + 1 == end)  return;
        auto mid = (begin + end) >> 1;
        cdq(begin, mid), tree.tick();

        std::sort(ele.begin() + begin, ele.begin() + mid, lam(x, y, x.h < y.h));
        std::sort(ele.begin() + mid, ele.begin() + end, lam(x, y, x.h < y.h));

        auto i = begin, j = mid;
        while (i != mid or j != end) {
            if (j == end or (i != mid and ele[i].h <= ele[j].h)) {
                tree.update(ele[i].v, F.at(ele[i].index));
                i++;
            } else {
                F.at(ele[j].index).update(tree.query(ele[j].v) + 1);
                j++;
            }
        }

        std::sort(ele.begin() + mid, ele.begin() + end, lam(x, y, x.index < y.index));
        cdq(mid, end);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N;
        ele.resize(N);
        for (auto &[h, v, _]: ele) {
            std::cin >> h >> v;  h = -h, v = -v;
            values.push_back(h), values.push_back(v);
        }
        ele.push_back({0, 0, 0}), values.push_back(0);
        N++;
        for (auto i: range(N))  ele.at(i).index = i;

        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        debug std::cout << "values: " << std::endl;
        debug for (auto i: values)  std::cout << i << ' ';
        debug std::cout << std::endl;

        for (auto &[h, v, _]: ele)  h = getIndex(h), v = getIndex(v);
        debug for (auto e: ele)  std::cout << (std::string)e << std::endl;

        tree = BIT(values.size());
        F.resize(N, {1, 1.0});
        cdq(0, ele.size());

        debug std::cout << "G: " << std::endl;
        debug for (auto i: F)  std::cout << (std::string)i << ' ';
        debug std::cout << std::endl;

        auto &max = *std::max_element(F.begin(), F.end());
        auto ans = max.val - 1;
        auto tot = max.cnt;
        G = std::vector(F.begin(), F.end());  // 备份 F

        // 接下来，将序列翻转和取反，再跑一遍
        ele.pop_back();
        std::sort(ele.begin(), ele.end(), lam(x, y, x.index < y.index));
        for (auto &[h, v, i]: ele)  h = -values.at(h), v = -values.at(v), i = N-i-1;
        std::reverse(ele.begin(), ele.end());
        std::transform(values.begin(), values.end(), values.begin(), lam(x, -x));
        std::reverse(values.begin(), values.end());
        for (auto &[h, v, _]: ele)  h = getIndex(h), v = getIndex(v);

        tree.tick();
        std::fill(F.begin(), F.end(), Statistic{1, 1.0});
        cdq(0, ele.size());
        std::reverse(F.begin(), F.end());

        debug for (auto e: ele)  std::cout << (std::string)e << std::endl;
        debug std::cout << "F: " << std::endl;
        debug for (auto i: F)  std::cout << (std::string)i << ' ';
        debug std::cout << std::endl;

        
        std::cout << ans << std::endl;
        std::cout << std::fixed << std::setprecision(6);
        for (auto i: range(N - 1)) {
            assert(F.at(i).val + G.at(i).val - 1 <= ans);
            if (F.at(i).val + G.at(i).val - 1 == ans) {
                std::cout << F.at(i).cnt * G.at(i).cnt / tot << ' ';
            } else {
                std::cout << 0 << ' ';
            }
        }
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_7021114588160689::solve();
    return 0;
}
