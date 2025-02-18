/**
 * @link https://www.luogu.com.cn/problem/AT_abc276_g
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
#include "./libs/debug_log.hpp"

using namespace lib;

/**
 * 观察这个数组的差分：
 * - a[i] 单调不减，即差分数组非负。
 * - a[N] <= M，差分数组之和加上 a[1] 不大于 M
 * - a[i] 与 a[i+1] 模 3 不同余，即差分数组中不存在 3 的倍数
 * 
 * 可以先往差分数组里放置若干个 1 和若干个 2。一共有 N - 1 个数。
 * 假设这一步放了 x 个 1，就需要 N - 1 - x 个 2。
 * 接下来，可以选择一些数 +3。这里也可以加在 A[1] 上。
 *  
 * 例如样例，N = 3，M = 4，
 * A[1] 及差分数组的可能性如下：
 * 0 1 1
 * 0 1 2
 * 0 2 1
 * 0 2 2
 * 1 1 1
 * 1 1 2
 * 1 2 1
 * 2 1 1
 * 
 * 不妨把 A[1] 也看成差分数组的一部分，那么 diff[1] 就比较特别，可以为 0。
 * 先选一个 diff[1]，剩下的填充 1 和 2，方案数为 C(N - 1, x)
 * 接下来，计算有多少次 +3 机会。
 * T 次 +3 机会分给 N 个元素，可以留空。
 * 组合笑传之插插板。
 * T 次 +3 机会，N - 1 个隔板。sum C(N+t-1, N-1) 种可能性。（t <= T）
 */
namespace Solution_1031583703409751 {
    auto constexpr qpow(i64 a, i64 b, i32 const mod) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % mod) {
            if (b & 1)  res = res * a % mod;
        }
        return res;
    }
    i32 constexpr mod = 998244353;
    class Factorials {
        std::vector<i32> fac;
        std::vector<i32> fac_inv;
    public:
        Factorials(i32 N): fac(N+1), fac_inv(N+1) {
            fac[0] = 1;
            for (i32 i = 1; i <= N; i++) {
                fac[i] = static_cast<i64>(fac[i-1]) * i % mod;
            }
            fac_inv[N] = qpow(fac[N], mod - 2, mod);
            for (i32 i = N; i --> 0; ) {
                fac_inv[i] = static_cast<i64>(fac_inv[i+1]) * (i+1) % mod;
            }
        }

        auto constexpr A(i32 a, i32 b) const -> i32 {
            return static_cast<i64>(fac[a]) * fac_inv[a - b] % mod;
        }
        auto constexpr C(i32 a, i32 b) const -> i32 {
            return static_cast<i64>(A(a, b)) * fac_inv[b] % mod;
        }
    };
    i32 constexpr facsMax = 20'000'005;
    Factorials facs{facsMax};
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        i32 ans = 0;

        std::vector<int> ps(M+1);  // facs.C(N + t - 1, N - 1) 的前缀和
        ps[0] = 1;
        for (i32 t = 1; t <= M; t++) {
            ps[t] = (ps[t - 1] + facs.C(N + t - 1, N - 1)) % mod;
        }

        // 选择 diff1
        for (i32 first = 0; first <= 2; first++) {
            for (i32 x = N; x --> 0; ) {
                i32 placed = x + (N - 1 - x) * 2;
                if (placed + first > M)  break;

                auto T = (M - placed - first) / 3;
                auto cur_ans = static_cast<i64>(facs.C(N - 1, x)) * ps[T] % mod;
                ans = (ans + cur_ans) % mod;

                debug  printValues(first, x, cur_ans);
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1031583703409751::solve();
    return 0;
}
