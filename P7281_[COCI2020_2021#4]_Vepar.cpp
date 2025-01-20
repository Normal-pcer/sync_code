/**
 * @link https://www.luogu.com.cn/problem/P7281
 */
#include "libs/debug_macros.hpp"
#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_6039322816620378 {
    constexpr const int MaxNumber = 10'000'999;

    struct PrimeFilter {
        std::bitset<MaxNumber> notPrime = 3;
        std::vector<int> primes;

        PrimeFilter(int N) {
            for (auto i = 2; i < N; i++) {
                if (not notPrime[i])  primes.push_back(i);
                for (auto p: primes) {
                    if (p * i > N)  break;
                    notPrime[p * i] = true;
                    if (i % p == 0)  break;
                }
            }
        }
    } primeFilter{10'000'900};
    auto const &primes = primeFilter.primes;
    auto const &notPrime = primeFilter.notPrime;

    /**
     * 计数 x 的阶乘的质因数，写入 out 中
     * 考虑：对于一个素数 p
     * floor(x / p) 个数是 p 的倍数
     * floor(x / p ** 2) 个数是 p ** 2 的倍数
     * ...
     */
    auto count(int x, auto &out) -> void {
        for (size_t p_i = 0; primes[p_i] <= x; p_i++) {
            auto p = primes[p_i];
            for (ll exp = 1LL, pow = p; pow <= x; exp++, pow *= p) {
                if (pow == 0)  std::exit(32);
                if (p == 0)  std::exit(33);
                out[p_i] += x / pow;
            }
        }
    }
    /**
     * 计数 [l, r] 累乘的质因数
     */
    auto count(int l, int r, auto &out) -> void {
        if (l > 1) {
            count(l - 1, out);
            for (auto &x: out)  x = -x;
        }
        count(r, out);
    }
    void solve() {
        debug {
            std::cout << "primes: ";
            for (auto x: primes | views::take(20))  std::cout << x << " ";
            std::cout << std::endl;
        }
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;

        std::vector<ll> cnt0(primes.size()), cnt1(primes.size());
        count(c, d, cnt0), count(a, b, cnt1);

        debug {
            std::cout << "cnt0: " << std::endl;
            for (auto x: cnt0 | views::take(6))  std::cout << x << " ";
            std::cout << std::endl;
            std::cout << "cnt1: " << std::endl;
            for (auto y: cnt1 | views::take(6))  std::cout << y << " ";
            std::cout << std::endl;
        }

        std::cout << [&]() -> char const * {
            for (size_t i = 0; i != primes.size(); i++) {
                if (cnt0[i] < cnt1[i])  return "NE";
            }
            return "DA";
        }() << endl;
    }
}
// void handler(int) {}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    // std::signal(SIGFPE, handler);
    int T;  std::cin >> T;
    while (T --> 0) {
        Solution_6039322816620378::solve();
    }
    return 0;
}
