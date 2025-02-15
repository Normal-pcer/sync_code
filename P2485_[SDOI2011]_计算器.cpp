/**
 * @link https://www.luogu.com.cn/problem/P2485
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 第一种询问，快速幂即可
 * 第二种询问，即 y / z (mod p)
 * 第三种询问，BSGS 模板
 */
namespace Solution_2298416323137184 {
    char const constexpr NoSolution[] = "Orz, I cannot find x!";
    auto constexpr qpow(i64 a, i64 b, i32 const p) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    // 解 pow(a, x) = s (mod p)，p 为质数，无解返回 -1
    auto log(i32 a, i32 s, i32 const p) -> i32 {
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
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 T, K;  std::cin >> T >> K;
        while (T --> 0) {
            i32 y, z, p;  std::cin >> y >> z >> p;
            if (K == 1) {
                std::cout << qpow(y, z, p) << std::endl;
            } else if (K == 2) {
                if (y % p == 0 and z != 0)  std::cout << NoSolution << std::endl;
                else  std::cout << (static_cast<i64>(z) * qpow(y, p - 2, p) % p) << std::endl;
            } else if (K == 3) {
                auto ans = log(y % p, z % p, p);
                if (ans == -1)  std::cout << NoSolution << std::endl;
                else  std::cout << ans << std::endl;
            } else {
                assert(false), __builtin_unreachable();
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2298416323137184::solve();
    return 0;
}
