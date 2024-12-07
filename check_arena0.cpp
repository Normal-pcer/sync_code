/**
 * @link https://www.luogu.com.cn/problem/CF1045G
 */

#include "./lib"

using namespace lib;

namespace Solution_1029182544418189 {
    class BIT {
        struct Node {
            int val = 0, version = 0;
        };
        std::vector<Node> c;
        int current;
        inline int constexpr lowbit(int x) const noexcept { return x & -x; }
    public:
        BIT(size_t N=0): c(N+1), current(0) {}
        int query(int x) const {
            x++;  auto res = 0;
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
    } tree(0);
    struct Robot {
        int pos, sight, IQ;
    };
    int N, K;
    std::vector<Robot> bots;
    std::vector<int> values;
    int getIndex(int x, bool flag = false) {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x or flag);
        return std::distance(values.begin(), it);
    }
    // 按照 IQ 排序
    ll cdq(decltype(bots)::iterator begin, decltype(bots)::iterator end) {
        auto x = begin - bots.begin(), y = end - bots.begin();
        debug  std::cout << std::format("cdq({}, {})", x, y) << std::endl;
        if (begin + 1 == end)  return 0;
        auto ans = 0LL;
        auto mid = begin + (std::distance(begin, end) >> 1);
        ans += cdq(begin, mid), tree.tick();
        ans += cdq(mid, end), tree.tick();

        auto i = mid;  // 右侧的指针
        auto min_p = begin, max_p = begin;  // 左侧最小和最大能被看到的 IQ，左闭右开
        while (i != end) {
            while (max_p != mid and max_p->IQ - i->IQ <= K)  tree.update(max_p->pos, 1), max_p++;
            while (min_p != max_p and i->IQ - min_p->IQ > K)  tree.update(min_p->pos, -1), min_p++;
            auto pos_center = i->pos;
            auto min_pos = std::distance(values.begin(), std::lower_bound(values.begin(), values.end(), values.at(pos_center) - i->sight));
            auto max_pos = std::distance(values.begin(), std::upper_bound(values.begin(), values.end(), values.at(pos_center) + i->sight));
            auto cnt = tree.query(max_pos - 1) - tree.query(min_pos - 1);  // 配对个数
            ans += cnt;
            i++;
        }
        std::sort(begin, end, lam(x, y, x.IQ < y.IQ));
        return ans;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        std::cin >> N >> K;
        bots.resize(N);
        for (auto &[p, s, i]: bots)  std::cin >> p >> s >> i, values.push_back(p);
        std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());
        for (auto &[p, s, i]: bots)  p = getIndex(p);

        std::sort(bots.begin(), bots.end(), lam(x, y, x.sight > y.sight));  // 按照视野排序
        // 如果右面能看到左面，反过来也一定能看到
        tree = BIT(values.size());
        auto ans = cdq(bots.begin(), bots.end());

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1029182544418189::solve();
    return 0;
}
