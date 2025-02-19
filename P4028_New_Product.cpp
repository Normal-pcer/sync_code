/**
 * @link https://www.luogu.com.cn/problem/P4028
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_1808978330610571 {
    auto constexpr mulMod(i64 a, i64 b, i32 const p) -> i32 {
        return a * b % p;
    }
    auto constexpr qpow(i64 a, i64 b, i32 const p) -> i32 {
        i64 res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    auto logMod(i32 a, i32 s, i32 const p) -> i32 {
        auto t = static_cast<i32>(std::ceil(std::sqrt(p)));
        std::vector<i32> pow_a_i(t), pow_a_ti(t);
        auto pow_a_t = qpow(a, t, p);

        pow_a_i[0] = pow_a_ti[0] = 1;
        for (i32 i = 1; i < t; i++) {
            pow_a_i[i] = mulMod(pow_a_i[i-1], a, p);
            pow_a_ti[i] = mulMod(pow_a_ti[i-1], pow_a_t, p);
        }

        std::map<i32, i32> map;
        for (i32 i = 0; i < t; i++)  map[pow_a_i[i]] = i;

        for (i32 i = 0; i < t; i++) {
            auto val = pow_a_ti[i];
            auto other = mulMod(s, qpow(val, p - 2, p), p);
            if (map.contains(other)) {
                auto j = map.at(other);
                return i * t + j;
            }
        }
        return -1;
    }
    char const constexpr *noSolution = "Couldn't Produce!";
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 T;  std::cin >> T;
        while (T --> 0) {
            i32 p, a, b;  std::cin >> p >> a >> b;
            a %= p;
            auto ans = [&]() -> i32 {
                if (b >= p)  return -1;
                if (a == 0) {
                    if (b != 0)  return -1;
                }
                return logMod(a, b, p);
            }();
            if (ans == -1)  std::cout << noSolution << endl;
            else  std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1808978330610571::solve();
    return 0;
}
