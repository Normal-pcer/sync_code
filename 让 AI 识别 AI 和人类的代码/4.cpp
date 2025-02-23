// 4.cpp
#include <bits/stdc++.h>
namespace ranges = std::ranges;
using i32 = int32_t; using i64 = int64_t;
char constexpr endl = '\n';
namespace Solution {
    int constexpr mod = 998244353;
    class BIT {
        std::vector<i32> c;
        auto static constexpr lowbit(i32 x) -> i32 { return x & -x; }
    public:
        BIT(i32 N): c(N+1) {}
        auto sumPre(i32 x) -> i32 {
            x++;  i32 res = 0;
            while (x != 0) {
                res = (res + c[x]) % mod;
                x -= lowbit(x);
            }
            return res;
        }
        auto addAt(i32 x, i32 val) -> void {
            x++;
            while (x < static_cast<i32>(c.size())) {
                c[x] = (c[x] + val) % mod;
                x += lowbit(x);
            }
        }
    };
    auto constexpr qpow(i64 a, i64 b, i32 const mod) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % mod) {
            if (b & 1)  res = res * a % mod;
        }
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        i32 N;  std::cin >> N;
        std::vector<i32> val(N);
        for (auto &x: val)  std::cin >> x;
        auto max = ranges::max(val) + 2;
        BIT bit_sum{max + 1}, bit_cnt{max + 1};
        i32 sum = 0;  
        for (i32 i = 0; i < N; i++) {
            auto append = val[i];
            auto greater = bit_sum.sumPre(max) - bit_sum.sumPre(append);
            greater = (greater + mod) % mod;
            sum = ((sum + greater) % mod + greater) % mod;  
            
            
            bit_cnt.addAt(append, +1);
            bit_sum.addAt(append, append);
            auto index = bit_cnt.sumPre(append);
            sum = (sum + static_cast<i64>(2 * index - 1) * append % mod) % mod;
            auto cur_ans = static_cast<i64>(sum) * qpow(i + 1, mod - 2, mod) % mod;
            cur_ans = cur_ans * qpow(i + 1, mod - 2, mod) % mod;
            std::cout << cur_ans << endl;
        }
    }
    
}
int main(int argc, char const *argv[]) {
    Solution::solve();
    return 0;
}