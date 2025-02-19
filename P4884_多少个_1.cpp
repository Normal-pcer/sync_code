/**
 * @link https://www.luogu.com.cn/problem/P4884
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 9x + 1 = pow(10, n)
 * pow(10, n) = 9K + 1(mod m)
 */
namespace Solution_7759522137419517 {
    using i128 = __int128;
    auto mulMod(i128 a, i128 b, i64 const p) -> i64 {
        return a * b % p;
    }
    auto qpow(i64 a, i64 b, i64 const p) -> i64 {
        i64 res = 1;
        for (; b; b >>= 1, a = mulMod(a, a, p)) {
            if (b & 1)  res = mulMod(res, a, p);
        }
        return res;
    }

    // è§£ pow(a, x) = s
    auto logMod(i32 a, i64 s, i64 const p) -> i64 {
        auto t = static_cast<i32>(std::ceil(std::sqrt(p)));
        auto pow_a_t = qpow(a, t, p);

        std::vector<i64> pow_a_i(t), pow_a_ti(t);
        pow_a_i[0] = pow_a_ti[0] = 1;
        for (i32 i = 1; i < t; i++) {
            pow_a_i[i] = mulMod(pow_a_i[i-1], a, p);
            pow_a_ti[i] = mulMod(pow_a_ti[i-1], pow_a_t, p);
        }

        std::map<i64, i32> map;
        for (i32 i = 0; i < t; i++)  map[pow_a_i[i]] = i;

        for (i32 i = 0; i < t; i++) {
            auto val = pow_a_ti[i];
            auto another = mulMod(s, qpow(val, p - 2, p), p);
            if (map.contains(another)) {
                auto j = map.at(another);
                return static_cast<i64>(i) * t + j;
            }
        }
        assert(false);
        return -1;
    }
    void solve() {
        i64 K, p;  std::cin >> K >> p;

        auto s = 9 * K + 1;
        auto x = logMod(10, s, p);

        std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7759522137419517::solve();
    return 0;
}
