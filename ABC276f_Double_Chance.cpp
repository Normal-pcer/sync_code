/**
 * @link https://www.luogu.com.cn/problem/AT_abc276_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 例如如下的有序序列
 * 1 2 3 4 5
 * 5 作为权值的可能：15 25 35 45 55 54 53 52 51
 * 4: 14 24 34 44 43 42 41
 * 3: 13 23 33 32 31
 * 2: 12 22 31
 * 1: 11
 * 
 * 总可能性为 25 种，分别为 9/25 7/25 5/25 3/25 1/25
 * 一个数产生的贡献是 ((2*index-1) / (N*N)) * value，index 为 1 开始的排名（如果有重复，重复的可以随意排列）
 * 验证一下这个式子？比如长度为 5 的序列 [1, 2, 3, 5, 8]，通过上述式子计算的期望为 5.16。通过随机模拟计算的期望为 5.160698，在误差允许范围内。
 * 
 * 然后可以观察插入一个数的变化。先不考虑 N*N 的分母。插入一个数会使得一些数的 index 增加 1，具体地，所有大于新值的 index 都会 +1
 * 那么总贡献的增量就是：所有大于新值的 value 之和乘以 2，加上新值的贡献。
 * 最后考虑一下 N 的变化即可。
 * 使用权值树状数组查询上述增量。
 */
namespace Solution_7145331350905880 {
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
        i32 sum = 0;  // (2*index-1) * value 之和
        for (i32 i = 0; i < N; i++) {
            auto append = val[i];
            auto greater = bit_sum.sumPre(max) - bit_sum.sumPre(append);
            greater = (greater + mod) % mod;
            sum = ((sum + greater) % mod + greater) % mod;  // 其他数贡献的增量
            // 然后计算当前数的贡献
            // 需要求出当前数的排名
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
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7145331350905880::solve();
    return 0;
}
