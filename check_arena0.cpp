/**
 * @link https://www.luogu.com.cn/problem/AT_abc270_g
 */
#include "./libs/debug_macros.hpp"


#include "./lib_v4.hpp"


using namespace lib;

#include "./libs/fixed_int.hpp"

/**
 * x[i] = A * x[i-1] + B
 * x[i] + m = A * (x[i-1] + m)，故 m = B / (A - 1)
 * x[i] + m = pow(A, i) * (x[0] + m)
 * 求 x[i] = G 时的 i
 * 
 * 即 pow(A, i) = K（K 为常数）
 * 令 q = ceil(sqrt(p))
 * 预处理出 pow(A, 0), pow(A, q), pow(A, 2*q), pow(A, 3*q), ...
 * 以及 pow(A, 0), pow(A, 1), pow(A, 2), ..., pow(A, q-1)
 * 
 * A = 1 需要特判，x[i] = x[i-1] + B，希望 x[i] = G
 * x[i] = x[0] + B * i = G
 * B * i = G - x[0]
 * i = (G - x[0]) / B
 */
namespace Solution_1355059899452297 {
    auto constexpr qpow(i64 a, i64 b, const i32 mod) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % mod)  if (b & 1) res = res * a % mod;
        return res;
    }
    void solve() {
        i32 P, A, B, S, G;  std::cin >> P >> A >> B >> S >> G;
        auto get_inv = lam(x, qpow(x, P-2, P));

        if (A == 1) {
            // 特判，此时不构成等比数列
            auto i = static_cast<i64>((G - S) % P + P) % P * get_inv(B) % P;
            std::cout << i << std::endl;
            return;
        }
        if (S == G) {  // 特判，此时答案为 x[0]
            std::cout << 0 << std::endl;
            return;
        }

        // 求解关于 x 的方程 pow(a, x) = s（模 P 意义下）
        auto log_mod = [&](int a, int s) -> i64 {
            auto q = static_cast<i32>(std::sqrt(P) * 1.5);
            std::vector<i32> pow_a_qi(q);  // pow(a, i * q) % P
            std::vector<i32> pow_a_i(q);  // pow(a, i) % P

            pow_a_i[0] = pow_a_qi[0] = 1;
            for (auto i = 1; i < q; i++) {
                pow_a_i[i] = static_cast<i64>(pow_a_i[i-1]) * a % P;
            }
            auto pow_a_q = qpow(a, q, P);
            for (auto i = 1; i < q; i++) {
                pow_a_qi[i] = static_cast<i64>(pow_a_qi[i-1]) * pow_a_q % P;
            }

            std::map<i32, i32> map;  // map[i] = j: pow_a_i[j] = i
            for (auto i = 0; i < q; i++)  map.insert({pow_a_i[i], i});

            // pow(a, x) = s
            // pow(a, i * q + j) = s
            // 枚举这个 i，尝试解出 j
            for (auto i = 0; i < q; i++) {
                // pow(a, i*q) * pow(a, j) = s
                // pow(a, j) = s / pow(a, i*q)
                auto expected_pow_a_j = static_cast<i64>(s) * get_inv(pow_a_qi[i]) % P;
                auto it = map.find(expected_pow_a_j);
                if (it != map.end()) {  // 存在
                    auto [pow_a_j, j] = *it;
                    return i * q + j;
                }
            }
            return -1;
        };

        auto m = static_cast<i64>(B) * get_inv(A - 1) % P;  // m 为常数
        // x[i] = pow(A, i) * (x[0] + m) - m
        // x[i] = G，求 i
        // pow(A, i) * (x[0] + m) - m = G
        // pow(A, i) = (G + m) / (x[0] + m)
        // x[0] = s
        auto pow_a_i = static_cast<i64>(G + m) * get_inv(S + m) % P;
        auto i = log_mod(A, pow_a_i);
        std::cout << i << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int T;  std::cin >> T;
    while (T --> 0) {
        Solution_1355059899452297::solve();
    }
    return 0;
}
