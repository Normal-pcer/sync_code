/**
 * @link https://www.luogu.com.cn/problem/P9176
 */

#include "./lib_v3.hpp"

using namespace lib;

// 即每次在序列中添加 a[i] 个 v[i]，之后查询中位数
// 用权值树状数组维护，查询目标排名对应的元素即可
// v[i] < 1e9，需要离散化
// 放弃大脑，直接二分套树状数组，多个 log 无所谓，能过就行
namespace Solution_1452969755513677 {
    // 权值树状数组
    class BIT {
        // 对应地，原数组表示一个数字的出现次数
        // 使用树状数组维护原数组前缀和
        int N;
        std::vector<ll> c;

        static auto constexpr lowbit(int x) -> int { return x & -x; }
    public:
        BIT(int N): N(N), c(N+1) {}
        auto sumPrefix(int x) -> ll {
            auto res = 0LL;  x++;
            while (x != 0) {
                res += c.at(x);
                x -= lowbit(x);
            }
            return res;
        }
        auto addAt(int x, int val) -> void {
            x++;
            while (x < static_cast<int>(c.size())) {
                c.at(x) += val;
                x += lowbit(x);
            }
        }
    };
    template <typename Value, typename Target, typename Func, typename Compare = std::less<>>
    auto lower_bound_value(Value begin, Value end, Target x, Func &&get, Compare &&cmp = {}, Value eps = 1) -> Value {
        while (end - begin >= eps) {
            auto mid = begin + ((end - begin) >> 1);
            if (cmp(get(mid), x))  begin = mid + eps;
            else  end = mid;
        }
        return begin;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<std::pair<int, int>> queries(N);
        std::vector<int> values;
        for (auto &[x, y]: queries)  std::cin >> x >> y, values.push_back(x);

        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        auto getIndex = [&](int x) -> int {
            auto it = std::lower_bound(values.begin(), values.end(), x);
            assert(it != values.end() and *it == x);
            return std::distance(values.begin(), it);
        };
        debug {
            std::cout << "values: " << std::endl;
            for (auto x: values)  std::cout << x << " ";
            std::cout << std::endl;
        }
        for (auto &[x, y]: queries)  x = getIndex(x);

        BIT bit{static_cast<int>(values.size())};
        auto size = 0LL;
        for (auto [v, a]: queries) {
            // 添加 a 个 v
            bit.addAt(v, +a), size += a;

            auto rank = (size + 1) >> 1;  // 希望的排名即 (size + 1) / 2 向上取整
            // 接下来，二分查找一个 y，数组中恰好有 rank 个 i 使得 i <= y
            // 如果找不到，应该是找大于的那个
            // 只需查找最大的 y 使得 f(y) >= rank
            auto f = [&](int y) -> ll { return bit.sumPrefix(y); };
            auto ans = lower_bound_value(0, static_cast<int>(values.size()), rank, f);
            std::cout << values.at(ans) << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1452969755513677::solve();
    return 0;
}
