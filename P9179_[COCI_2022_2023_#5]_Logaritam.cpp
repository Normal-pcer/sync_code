/**
 * @link https://www.luogu.com.cn/problem/P9179
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * a[xy] = a[x] + a[y]
 * 可以确定 a[1] = 0
 * 
 * a[4] = a[2] + a[2]
 * a[6] = a[2] + a[3]
 * 考虑修改 a[4] -> a[2] -> a[3]，自然是两次修改
 * a[6] -> a[2] -> a[4]
 *      -> a[3]
 * 选择 a[3] 即可做到一次修改，猜想：优先改较大的质因数
 * 
 * 同理，看样例 2
 * a[12] = a[2] + a[2] + a[3]
 * a[12] -> a[3] -> a[6], a[9], a[15], a[18]，所以是 5 个
 * 猜测：直接改掉最大的一个质因数，它的所有倍数也要改掉
 */
namespace Solution_6127778988705236 {
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
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        // 对于每一次询问 x，希望找到它的最大质因数，质数需要筛到 x 的最大值即 N
        PrimeManager pm{N+5};

        while (Q --> 0) {
            int x;  std::cin >> x;
            auto ans = [&]() -> int {
                if (x == 1)  return -1;  // 无解，因为需要保证 a[1] = 1
                // 寻找 x 的最大质因数
                auto max_p = -1;
                if (not pm.notPrime[x])  max_p = x;
                else {
                    auto tmp = x;
                    auto it = pm.primes.begin();
                    while (pm.notPrime[tmp]) {
                        assert(it != pm.primes.end());
                        while (tmp % *it == 0 and pm.notPrime[tmp])  tmp /= *it;
                        it++;
                    }
                    max_p = tmp;
                }

                // 最大质因数的所有倍数都被修改
                auto multiple_cnt = N / max_p;
                return multiple_cnt - 1;  // 减去 x 本身
            }();
            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6127778988705236::solve();
    return 0;
}
