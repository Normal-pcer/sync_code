/**
 * @link https://www.luogu.com.cn/problem/P4861
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * M 进制下，每次操作将这个数乘以 K。
 * 希望通过若干次操作让个位是 1。
 * 
 * 即 pow(K, x) = 1（模 M）
 * 
 * 由欧拉定理，可以保证 pow(a, phi(x)) = 1 (mod x)
 * 当 gcd(a, x) = 1，上述方程一定有解
 * 否则是否一定无解？
 * ~~感觉是这样的。~~
 * 证明：若 pow(K, x) = 1 (mod M)
 * 令 d = gcd(K, M)
 * 则有 d | K 且 d | M
 * 而必然存在 t 使得 pow(K, x) = 1 + tM
 * 由于 d | pow(K, x) 且 d | tM，所以必然有 d | 1
 * 故 gcd(K, M) = 1，这个方程有解仅当 K 和 M 互质。
 * 先判断是否有解，如果有解就使用 BSGS 求出。
 */
namespace Solution_6005643282492609 {
    char const constexpr NoSolutionTip[] = "Let's go Blue Jays!";

    auto constexpr qpow(i64 a, i64 b, const i32 mod) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % mod) {
            if (b & 1)  res = res * a % mod;
        }
        return res;
    }
    // 解关于 x 的方程：pow(a, x) = 1（mod m），无解返回 -1
    auto f(i32 a, const i32 m) -> i32 {
        // 这个方程的解必然可以保证小于 m
        // 令 t = ceil(sqrt(m))，那么解可以表示成 x0 = pt - q
        // 预处理所有的 pow(a, ti) 和 pow(a, i)（i < t）
        // 枚举 p，希望 pow(a, pt) / pow(a, q) = 1
        // 求 q 使得 pow(a, q) = pow(a, pt)
        // 在上述的 pow(a, i) 中查找即可

        auto t = static_cast<i32>(std::ceil(std::sqrt(m)));
        
        std::vector<i32> pow_a_ti(t + 1), pow_a_i(t);  // 前者 i 的范围是 [1, t] 闭区间，后者是 [0, t-1]（最后凑出来的结果是 1 至 M）
        pow_a_ti[0] = pow_a_i[0] = 1;
        auto pow_a_t = qpow(a, t, m);
        for (auto i = 1; i <= t; i++) {
            pow_a_ti[i] = static_cast<i64>(pow_a_ti[i-1]) * pow_a_t % m;
        }
        for (auto i = 1; i < t; i++) {
            pow_a_i[i] = static_cast<i64>(pow_a_i[i-1]) * a % m;
        }

        std::map<i32, i32> map;  // a 的 value 次方等于 key（1 <= value <= t）
        for (auto i = 0; i < t; i++)  map[pow_a_i[i]] = i;

        for (auto p = 1; p <= t; p++) {
            auto val = pow_a_ti[p];
            auto it = map.find(val);
            if (it != map.end()) {
                auto q = it->second;
                // pow(a, q) = pow(a, tp)
                // pow(a, tp - q) = 1
                return t * p - q;
            }
        }
        assert(false);
        return -1;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 M, K;  std::cin >> M >> K;
        // 由上面的论述，有解当且仅当二者互质
        auto no_solution = std::gcd(M, K) != 1;
        if (no_solution) {
            std::cout << NoSolutionTip << std::endl;
        } else {
            auto x = f(K % M, M);
            std::cout << x << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6005643282492609::solve();
    return 0;
}
