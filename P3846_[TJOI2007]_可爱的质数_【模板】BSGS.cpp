/**
 * @link https://www.luogu.com.cn/problem/P3846
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 见 AT_abc270_g
 */
namespace Solution_5171200675831619 {
    auto constexpr qpow(i64 a, i64 b, i32 const p) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    // 求解 pow(a, x) = s (mod p)，p 为质数
    // 无解返回 -1
    auto log(i32 a, i32 s, i32 const p) -> i32 {
        auto const get_inv = lam(x, qpow(x, p - 2, p));
        auto t = static_cast<i32>(std::ceil(std::sqrt(p)));

        // 不妨设 x = mt + n
        std::vector<i32> pow_a_ti(t), pow_a_i(t);
        auto pow_a_t = qpow(a, t, p);
        pow_a_ti[0] = pow_a_i[0] = 1;
        for (i32 i = 1; i < t; i++) {
            pow_a_ti[i] = static_cast<i64>(pow_a_ti[i-1]) * pow_a_t % p;
            pow_a_i[i] = static_cast<i64>(pow_a_i[i-1]) * a % p;
        }

        std::map<i32, i32> map;
        for (i32 i = 0; i < t; i++)  map[pow_a_i[i]] = i;
        for (i32 m = 0; m < t; m++) {
            auto val = pow_a_ti[m];
            // val * pow(a, n) = s
            auto another = static_cast<i64>(s) * get_inv(val) % p;
            auto it = map.find(another);
            if (it != map.end()) {
                auto n = it->second;
                return m * t + n;
            }
        }
        return -1;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 P, B, N;  std::cin >> P >> B >> N;
        auto l = log(B, N, P);
        if (l == -1)  std::cout << "no solution" << endl;
        else  std::cout << l << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5171200675831619::solve();
    return 0;
}
