#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

#define FILENAME "T2"

/*
两个数只有在一定的时间范围内才会构成逆序对。
例如现在有一个顺序对 3, 5 （M = 7）
右面那个数等于 5，6 的时候会是顺序对，除此以外都是逆序对。
一个逆序对 4, 1
左面等于 4 5 6 的时候都是逆序对。

发现一个对何时是逆序对，只与较大的数有关。
先正常求一次逆序对，例如遇到一个 3 就可以对于时间为 0, 1, 2, 3 统计贡献。
然后求一次顺序对，例如遇到一个 5 可以对于时间 2, 3, 4, ... 统计贡献。
需要区间操作，其实使用差分数组就行。

有转机。还有一段时间可以让逆序对还是逆序对。即小的那个数回绕。
可能需要跑四次。
*/
namespace Solution_1003255212752722 {
    class BIT {
        std::vector<i64> c;

        auto static constexpr lowbit(i32 x) -> i32 { return x & -x; }
    public:
        BIT(i32 n): c(n + 1) {}
        auto addAt(i32 x, i64 val) -> void {
            x++;
            while (x < static_cast<i32>(c.size())) {
                c[x] += val;
                x += lowbit(x);
            }
        }
        auto sumPrefix(i32 x) -> i64 {
            i64 res = 0;
            x++;
            while (x != 0) {
                res += c[x];
                x -= lowbit(x);
            }
            return res;
        }
    };
    class ADArray {
        std::vector<i64> ad;
    public:
        ADArray(i32 n): ad(n) {}

        auto addRange(i32 begin, i32 end, i64 val) -> void {
            if (begin == end) return;
            ad[begin] += val;
            if (end != static_cast<i32>(ad.size())) {
                ad[end] -= val;
            }
        }
        auto query() -> std::vector<i64> {
            std::vector<i64> res(ad.size());
            std::partial_sum(ad.begin(), ad.end(), res.begin());
            return res;
        };
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<i32> nums(n);
        for (auto &x: nums) {
            std::cin >> x;
        }

        ADArray res(m);
        {
            // 数逆序对
            BIT bit{m};
            std::for_each(nums.rbegin(), nums.rend(), [&](int x) {
                auto lessCount = x == 0? 0: bit.sumPrefix(x - 1);
                // 直到 x 回绕到 0
                auto last = m - x;
                auto first = 0;
                res.addRange(first, last, lessCount);
                
                bit.addAt(x, 1);
            });
        }
        {
            BIT bit{m};
            std::for_each(nums.begin(), nums.end(), [&](int x) {
                auto lessEqualCount = bit.sumPrefix(x);
                auto greaterCount = bit.sumPrefix(m - 1) - lessEqualCount;

                // x 回绕到 0，(y, x) 自然再次成为逆序对
                auto first = m - x;
                auto last = m;
                res.addRange(first, last, greaterCount);
                bit.addAt(x, 1);
            });
        }

        {
            BIT bit{m};
            std::for_each(nums.begin(), nums.end(), [&](int x) {
                auto lessCount = x == 0? 0: bit.sumPrefix(x - 1);
                auto last = m;
                auto first = m - x;
                res.addRange(first, last, lessCount);
                bit.addAt(x, 1);
            });
        }

        {
            BIT bit{m};
            std::for_each(nums.rbegin(), nums.rend(), [&](int x) {
                auto lessEqualCount = bit.sumPrefix(x);
                auto greaterCount = bit.sumPrefix(m - 1) - lessEqualCount;

                // x 回绕到 0，不再是逆序对
                auto first = m - x;
                auto last = m;
                res.addRange(first, last, -greaterCount);
                bit.addAt(x, 1);
            });
        }

        for (auto x: res.query()) {
            std::cout << x << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_1003255212752722::solve();
    return 0;
}
