/**
 * @link https://www.luogu.com.cn/problem/P3306
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

namespace lib {
    template <typename T>
    auto constexpr qpow(std::int64_t a, std::int64_t b, T const p) -> T {
        std::int64_t res = 1;
        for (; b; b >>= 1, a = a * a % p) {
            if (b & 1)  res = res * a % p;
        }
        return res;
    }
    template <typename T, T mod, bool isPrime = false>
    struct ModInt {
        using Self = ModInt<T, mod, isPrime>;
        T value;  // 保证这个值在 [0, mod)

        ModInt(): value(0) {}
        ModInt(T x): value(x % mod) {}

        auto constexpr operator+ (Self other) const -> Self {
            T new_value = value + other.value;
            if (new_value >= mod)  new_value -= mod;
            return {new_value};
        }
        auto constexpr operator- (Self other) const -> Self {
            T new_value = value - other.value;
            if (new_value < 0)  new_value += mod;
            return {new_value};
        }
        auto constexpr operator* (Self other) const -> Self {
            std::int64_t new_value = value;
            new_value *= other.value;
            return {static_cast<T>(new_value % mod)};
        }
        auto constexpr operator/ (Self other) const -> Self requires(isPrime) {
            std::int64_t new_value = value;
            new_value *= qpow(other.value, mod - 2, mod);
            return {static_cast<T>(new_value % mod)};
        }
    };
    template <typename T, size_t Tag>
    struct TagModInt {
        using Self = TagModInt<T, Tag>;
        static inline T mod = 1;
        static inline bool isPrime = false;
        int value;

        TagModInt(): value(0) {}
        TagModInt(T x): value((x % mod + mod) % mod) {}
        static auto constexpr setMod(T p, bool prime = false) -> void {
            mod = p;
            isPrime = prime;
        }
        auto constexpr operator+ (Self other) const -> Self {
            T new_value = value + other.value;
            if (new_value >= mod)  new_value -= mod;
            return {new_value};
        }
        auto constexpr operator- (Self other) const -> Self {
            T new_value = value - other.value;
            if (new_value < 0)  new_value += mod;
            return {new_value};
        }
        auto constexpr operator* (Self other) const -> Self {
            std::int64_t new_value = value;
            new_value *= other.value;
            return {static_cast<T>(new_value % mod)};
        }
        auto constexpr operator/ (Self other) const -> Self {
            assert(other.value != 0);
            assert(isPrime);
            std::int64_t new_value = value;
            new_value *= qpow(other.value, mod - 2, mod);
            return {static_cast<T>(new_value % mod)};
        }
    };
}
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
        auto t = static_cast<int>(std::ceil(std::sqrt(p)) + 1);
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
            auto val = pow_a_ti[i];
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

        auto i = [&]() -> i32 {
            // pow(a, i-1) = (t + m) / (x1 + m), m = b / (a - 1)
            // 需要判断 a = 1
            if (t == x1)  return 1;
            if (a == 1) {
                // x[i] = x[i-1] + b = x[1] + b * (i - 1)
                // i - 1 = (x[i] - x[1]) / b
                if (b == 0) {
                    if (t == x1)  return 1;
                    else  return -1;
                }
                return ((Mod{t} - Mod{x1}) / Mod{b}).value + 1;
            }
            auto m = Mod{b} / (Mod{a} - 1);
            if ((Mod{x1} + m).value == 0) {
                if ((Mod{t} + m).value == 0)  return 1;
                else  return -1;
            }
            if (a == 0) {
                if (t == x1)  return 1;
                if (t == b)  return 2;
                return -1;
            }
            auto i = log(a, (Mod{t} + m) / (Mod{x1} + m));
            if (i == -1)  return -1;
            else  i++;

            assert(i != 0);
            return i;
        }();

        std::cout << i << std::endl;
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
