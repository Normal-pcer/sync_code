#include <bits/stdc++.h>
#define FILENAME "T1"

using i32 = std::int32_t; using i64 = std::int64_t;
using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
char constexpr endl = '\n';

/**
 * 考虑一个贪心的分配方案。
 * 只需保证每个类型的城堡尽可能被分给不同的人。
 * 每个城堡，优先分配给一个没有分配过该类型、且拥有城堡数量最少的人。
 * 似乎每个城堡类型产生的贡献均为 max(0, 城堡个数 - 人数)
 * 
 * 难绷，这个贪心策略似乎是正确的。
 * 考虑优化。
 * 令 f(x) 表示有多少种城堡数量为 x。
 * 例如当前查询一个 4，答案即为 1*f(4) + 2*f(5) + 3*f(6)...
 * 可以使用线段树维护 4*f(4) + 5*f(5) + 6*f(6) + ...
 * 二者的差值是 f(4) + f(5) + f(6) + ... 乘以一个常数
 * 使用线段树维护 f(x) 的加和，以及 g(x) = x * f(x) 的加和。
 * 好像可以写树状数组
 * */
namespace Solution {
    class BIT {
        std::vector<i64> c;

        auto constexpr static lowbit(i32 x) -> i32 { return x & -x; }
    public:
        BIT(i32 N): c(N + 1) {}
        auto addAt(i32 x, i64 val) -> void {
            x++;
            while (x < static_cast<i32>(c.size())) {
                c[x] += val;
                x += lowbit(x);
            }
        }
        auto sumPrefix(i32 x) -> i64 {
            x++;  i64 res = 0;
            while (x != 0) {
                res += c[x];
                x -= lowbit(x);
            }
            return res;
        }
    };
    auto solve() -> void {
        i32 N, Q;  std::cin >> N >> Q;

        std::vector<i32> init(N);
        std::vector<i32> cnt_of_key(N + 1);
        BIT bit{N + Q + 5};     // 维护 f(x)
        BIT bit_g{N + Q + 5};   // 维护 g(x) 的树状数组
        for (auto &x: init)  {
            std::cin >> x;
            cnt_of_key[x]++;
        }
        for (i32 i = 1; i <= N; i++) {
            bit.addAt(cnt_of_key[i], 1);
            bit_g.addAt(cnt_of_key[i], cnt_of_key[i]);
        }

        // std::cout << "bit: " << endl;
        // for (i32 i = 0; i < N + Q; i++) {
        //     std::cout << bit.sumPrefix(i) - (i == 0? 0: bit.sumPrefix(i - 1)) << " ";
        // } std::cout << endl;
        // std::cout << "bit_g: " << endl;
        // for (i32 i = 0; i < N + Q; i++) {
        //     std::cout << bit_g.sumPrefix(i) - (i == 0? 0: bit_g.sumPrefix(i - 1)) << " ";
        // } std::cout << endl;

        for (auto _ = Q; _ --> 0; ) {
            i32 op, x;  std::cin >> op >> x;
            if (op == 1) {
                auto &cnt = cnt_of_key[x];
                bit.addAt(cnt, -1);
                bit.addAt(cnt + 1 , 1);
                bit_g.addAt(cnt, -cnt);
                bit_g.addAt(cnt + 1, cnt + 1);
                cnt++;
            } else if (op == 2) {
                auto &cnt = cnt_of_key[x];
                bit.addAt(cnt, -1);
                bit.addAt(cnt - 1 , 1);
                bit_g.addAt(cnt, -cnt);
                bit_g.addAt(cnt - 1, cnt - 1);
                cnt--;
            } else {
                auto ans = bit_g.sumPrefix(N + Q + 5) - bit_g.sumPrefix(x);
                ans -= x * (bit.sumPrefix(N + Q + 5) - bit.sumPrefix(x));
                std::cout << ans << endl;
            }
        }
    }
}

int main() {
    std::freopen(FILENAME ".in", "r", stdin);
    std::freopen(FILENAME ".out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();
    return 0;
}
