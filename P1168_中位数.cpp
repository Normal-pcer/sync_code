/**
 * @link https://www.luogu.com.cn/problem/P1168
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 在权值线段树上二分。
 * 但是直接二分套树状数组即可。跑得快。
 */
namespace Solution_3481521766246267 {
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U val, Func &&f, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(f(mid), val))  begin = mid + 1;
            else  end = mid;
        }
        return begin;
    }
    class BIT {
        std::vector<i32> c;

        auto static constexpr lowbit(i32 x) -> i32 { return x & -x; }
    public:
        BIT(i32 N): c(N + 1) {}

        auto addAt(i32 x, i32 val) -> void {
            x++;
            while (x < static_cast<i32>(c.size())) {
                c[x] += val;
                x += lowbit(x);
            }
        }
        auto sumPrefix(i32 x) -> i32 {
            x++;  i32 res = 0;
            while (x != 0) {
                res += c[x];
                x -= lowbit(x);
            }
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> a(N);
        for (auto &x: a)  std::cin >> x;

        // 离散化
        std::vector<i32> values(a);
        {
            ranges::sort(values);
            auto tmp = ranges::unique(values);
            values.erase(tmp.begin(), tmp.end());
        }
        auto get_index = [&](i32 x) -> i32 {
            auto it = ranges::lower_bound(values, x);
            assert(*it == x);
            return std::distance(values.begin(), it);
        };
        for (auto &x: a)  x = get_index(x);

        BIT bit{static_cast<i32>(values.size())};
        for (i32 i = 0; i < N; i += 2) {
            bit.addAt(a[i], 1);
            auto ans = lowerBoundValue<i32, i32>(0, values.size(), i / 2 + 1, [&](i32 x) {
                return bit.sumPrefix(x);
            });
            std::cout << values[ans] << endl;

            if (i + 1 < N)  bit.addAt(a[i + 1], 1);
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3481521766246267::solve();
    return 0;
}
