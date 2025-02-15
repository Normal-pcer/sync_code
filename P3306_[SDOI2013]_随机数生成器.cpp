/**
 * @link https://www.luogu.com.cn/problem/P3306
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/fixed_int.hpp"
#include "libs/mod.hpp"
using namespace lib;

using Mod = TagModInt<i32, 0>;
/**
 * 类似 AT_abc270_g，通过数列的通项公式可知：
 * pow(a, i-1) = (t + m) / (x1 + m), m = b / (a - 1)
 * a = 1 需要特判，此时是一个等差数列
 * x[i + 1] = x[i] + b，x[i] = x1 + (i - 1) * b
 * i - 1 = (x[i] - x[1]) / b，再判断 b = 0
 * x1 + m = 0? 此时需要 x[i] + m = 0
 */
namespace Solution_2781131823008829 {
    auto constexpr qpow(i64 a, i64 b, i32 const p) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    // pow(a, x) = s (mod p)
    auto log(Mod a, Mod s) -> i32 {
        auto p = Mod::mod;
        auto t = static_cast<int>(std::ceil(std::sqrt(p)));
        std::vector<Mod> pow_a_ti(t), pow_a_i(t);
        auto pow_a_t = qpow(a.value, t, p);
        pow_a_ti[0] = pow_a_i[0] = 1;
        for (i32 i = 1; i < t; i++) {
            pow_a_ti[i] = pow_a_ti[i-1] * pow_a_t;
            pow_a_i[i] = pow_a_i[i-1] * a;
        }

        std::map<i32, i32> map;
        for (i32 i = 0; i < t; i++)  map[pow_a_i[i].value] = i;

        for (i32 i = 0; i < t; i++) {
            auto val = pow_a_ti[t];
            auto another = s / val;
            auto it = map.find(another.value);
            if (it != map.end()) {
                auto j = it->second;
                return i * t + j;
            }
        }
        return -1;
    }
    void solve() {
        i32 p, a, b, x1, t;
        std::cin >> p >> a >> b >> x1 >> t;
        Mod::setMod(p, true);

        // pow(a, i-1) = (t + m) / (x1 + m), m = b / (a - 1)
        auto m = Mod{b} / Mod{a - 1};
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T;  std::cin >> T;
    while (T --> 0) {
        Solution_2781131823008829::solve();
    }
    return 0;
}
