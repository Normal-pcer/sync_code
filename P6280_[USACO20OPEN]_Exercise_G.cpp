/**
 * @link https://www.luogu.com.cn/problem/P6280
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_1667443414434721 {
    class PrimeManager {
    public:
        int N = 0;
        std::vector<int> primes;
        std::vector<char> notPrime;

        PrimeManager(int N): N(N), primes(), notPrime(N+1) {
            // 欧拉筛
            notPrime[0] = notPrime[1] = true;
            for (auto p = 2; p <= N; p++) {
                if (not notPrime[p])  primes.push_back(p);
                for (auto q: primes) {
                    if (q * p > N)  break;
                    notPrime[q * p] = true;
                    if (p % q == 0)  break;
                }
            }
        }
    };
    constexpr const int MaxPrime = 1e4 + 10;
    PrimeManager pm{MaxPrime};
    void solve() {
        std::ios::sync_with_stdio(false);
        i32 N, M;  std::cin >> N >> M;
        
        std::vector<i32> F(N+1);
        F[0] = 1;
        for (auto p: pm.primes) {
            for (i32 j = N; j >= p; j--) {
                i32 copy = p;
                while (copy <= j) {
                    F[j] = (F[j] + static_cast<ll>(F[j - copy]) * copy % M) % M;
                    copy *= p;
                }
            }
        }
        i32 ans = 0;
        for (auto x: F)  ans = (ans + x) % M;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1667443414434721::solve();
    return 0;
}
