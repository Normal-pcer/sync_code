/**
 * @link https://www.luogu.com.cn/problem/P11655
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * g(S) 即：S[i-1] != S[i] 的位置数量
 * 如果选定两个相邻的数，这个位置上的数是 01 或者 10，无论其他的位置是什么，都会产生一点贡献
 * 
 * 其他位置随意排列：C(N+M-2, N-1)
 * 有 N+M-1 个可能位置，所以为 C(N+M-2, N-1) * (N+M-1) * 2
 * 所有的都需要额外 +1，总计 C(N+M, N)
 */
namespace Solution_1834729110940105 {
    i32 const constexpr maxFactorial = 400'005;
    i32 const constexpr mod = 1e9+7;
    auto constexpr qpow(i64 a, i64 b, i32 const p) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    struct Factorials {
        std::vector<i32> fac;
        std::vector<i32> fac_inv;

        Factorials(i32 N): fac(N+1), fac_inv(N+1) {
            fac[0] = fac_inv[0] = 1;
            for (i32 i = 1; i <= N; i++) {
                auto inv_i = qpow(i, mod - 2, mod);
                fac[i] = static_cast<i64>(fac[i-1]) * i % mod;
                fac_inv[i] = static_cast<i64>(fac_inv[i-1]) * inv_i % mod;
            }
        }
        auto constexpr A(i32 a, i32 b) -> i32 {
            return static_cast<i64>(fac[a]) * fac_inv[a - b] % mod;
        }
        auto constexpr C(i32 a, i32 b) -> i32 {
            if (a >= 0 && b >= 0 && b <= a)  return static_cast<i64>(A(a, b)) * qpow(A(b, b), mod - 2, mod) % mod;
            else  return 0;
        }
    } fac{maxFactorial};
    void solve() {
        i32 N, M;  std::cin >> N >> M;
        auto ans = static_cast<i64>(fac.C(N + M - 2, N - 1)) * (N + M - 1) % mod * 2 % mod;
        ans = (ans + fac.C(N + M, N)) % mod;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);

    int T;  std::cin >> T;
    while (T --> 0) {
        Solution_1834729110940105::solve();
    }
    return 0;
}
