/**
 * @link https://www.luogu.com.cn/problem/P4454
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 已知 A = pow(g, a) mod P, B = pow(g, b) mod P
 * 希望求出 K = pow(g, a*b) mod P
 * 
 * K = pow(A, b) mod P
 * 只需解出 b。
 */
namespace Solution_1004761065112060 {
    auto qpow(i64 a, i64 b, i32 const p) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    auto mulMod(i64 a, i64 b, i32 const p) -> i32 {
        return a * b % p;
    }
    // 解关于 x 的方程 pow(a, x) = s (mod p)，无解返回 -1
    auto logMod(i32 a, i32 s, i32 const p) -> i32 {
        auto t = static_cast<i32>(std::ceil(std::sqrt(p)));

        std::vector<i32> pow_a_i(t), pow_a_ti(t);
        auto pow_a_t = qpow(a, t, p);

        pow_a_i[0] = pow_a_ti[0] = 1;
        for (i32 i = 1; i < t; i++) {
            pow_a_i[i] = mulMod(pow_a_i[i-1], a, p);
            pow_a_ti[i] = mulMod(pow_a_ti[i-1], pow_a_t, p);
        }

        std::map<i32, i32> map;  // pow_a_i 反向映射
        for (i32 i = 0; i < t; i++)  map[pow_a_i[i]] = i;

        for (i32 i = 0; i < t; i++) {
            i32 val = pow_a_ti[i];
            i32 other = mulMod(s, qpow(val, p - 2, p), p);
            
            auto it = map.find(other);
            if (it != map.end()) {
                auto j = it->second;
                return i * t + j;
            }
        }
        return -1;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 g, p;  std::cin >> g >> p;
        i32 N;  std::cin >> N;

        for (auto t = N; t --> 0; ) {
            i32 pow_g_a, pow_g_b;
            std::cin >> pow_g_a >> pow_g_b;

            auto a = logMod(g, pow_g_a, p);
            auto pow_g_ab = qpow(pow_g_b, a, p);
            std::cout << pow_g_ab << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1004761065112060::solve();
    return 0;
}
