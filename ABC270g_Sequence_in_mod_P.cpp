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
        auto get_inv = lam(x, (assert(x % P != 0), qpow(x, P-2, P)));

        if (S == G) {  // 特判，此时答案为 x[0]
            std::cout << 0 << std::endl;
            return;
        }
        if (A == 1) {
            // 特判，此时不构成等比数列
            if (B % P == 0)  std::cout << -1 << std::endl;
            else {
                auto i = static_cast<i64>((G - S) % P + P) % P * get_inv(B) % P;
                std::cout << i << std::endl;
            }
            return;
        }
        if (A == 0) {
            if (B == G)  std::cout << 1 << std::endl;
            else  std::cout << -1 << std::endl;
            return;
        }

        // 解 pow(a, x) = s (mod p)，p 为质数，无解返回 -1
        auto log = [](i32 a, i32 s, i32 const p) -> i32 {
            if (a == 0) {
                if (s == 0)  return 1;
                else  return -1;
            }
            auto t = static_cast<i32>(std::ceil(std::sqrt(p)));

            auto pow_a_t = qpow(a, t, p);
            std::vector<i32> pow_a_i(t), pow_a_ti(t);
            pow_a_i[0] = pow_a_ti[0] = 1;
            for (i32 i = 1; i < t; i++) {
                pow_a_i[i] = static_cast<i64>(pow_a_i[i-1]) * a % p;
                pow_a_ti[i] = static_cast<i64>(pow_a_ti[i-1]) * pow_a_t % p;
            }
            std::map<i32, i32> map;
            for (i32 i = 0; i < t; i++)  map[pow_a_i[i]] = i;

            for (i32 m = 0; m < t; m++) {
                auto val = pow_a_ti[m];
                auto another = static_cast<i64>(s) * qpow(val, p - 2, p) % p;
                auto it = map.find(another);
                if (it != map.end()) {
                    auto n = it->second;
                    return m * t + n;
                }
            }
            return -1;
        };

        std::cout << [&]() -> i32 {
            if ((A - 1) % P == 0)  return -1;
            auto m = static_cast<i64>(B) * get_inv(A - 1) % P;  // m 为常数
            // x[i] = pow(A, i) * (x[0] + m) - m
            // x[i] = G，求 i
            // pow(A, i) * (x[0] + m) - m = G
            // pow(A, i) = (G + m) / (x[0] + m)
            // x[0] = s
            if ((S + m) % P == 0)  return -1;
            auto pow_a_i = static_cast<i64>(G + m) * get_inv(S + m) % P;
            auto i = log(A % P, pow_a_i % P, P);
            return i;
        }() << endl;
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
