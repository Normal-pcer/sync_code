/**
 * @link https://www.luogu.com.cn/problem/AT_abc249_d
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 求 a[i] / a[j] = a[k] 的 (i, j, k) 数量。
 * 凭直觉，2e5 以内的数，因数个数会比较有限
 * 可以对于每一个 a[i] 枚举它的所有因数
 */
namespace Solution_5200972368419325 {
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
    constexpr const int MaxPrime = 2e5 + 10;
    PrimeManager pm{MaxPrime};

    struct Factors {
        struct Factor {
            int base = 1, exp = 0;
        };

        std::vector<Factor> f;
        static auto fromValue(int x) -> Factors {
            auto it = pm.primes.begin();
            std::map<int, int> map;
            while (pm.notPrime[x] and x != 1) {
                assert(it != pm.primes.end());
                while (x % *it == 0)  map[*it]++, x /= *it;
                it++;
            }
            if (x != 1)  map[x]++;

            Factors res;
            for (auto [x, exp]: map) {
                if (exp != 0)  res.f.push_back({x, exp});
            }
            return res;
        }

        auto _dfs(std::vector<Factor>::const_iterator it, auto &&call) const -> void {
            static std::vector<Factor> res;

            if (it == f.end())  return call(res), void();
            for (auto i = 0; i <= it->exp; i++) {
                res.push_back({it->base, i});
                _dfs(it + 1, call);
                res.pop_back();
            }
        }

        auto forEach(auto &&call) const -> void {
            _dfs(f.begin(), call);
        }

        auto number() const -> int {
            auto res = 1;
            for (auto [x, y]: f) {
                for (auto i = 0; i < y; i++)  res *= x;
            }
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        debug {
            Factors::fromValue(4).forEach([&](auto x) {
                std::cout << Factors{x}.number() << std::endl;
            });
        }

        int N;  std::cin >> N;
        std::vector<int> a(N);
        for (auto &x: a)  std::cin >> x;

        constexpr const int MaxValue = 2e5 + 10;
        std::vector<int> counter(MaxValue);

        for (auto x: a) {
            counter[x]++;
        }

        auto ans = 0LL;
        
        for (auto x: a) {
            auto fac = Factors::fromValue(x);
            auto this_cnt = 0LL;
            fac.forEach([&](std::vector<Factors::Factor> const &f) -> void {
                auto y = Factors{f}.number();
                debug  std::cout << "x = " << x << ", y = " << y << std::endl;
                assert(x % y == 0);
                auto z = x / y;
                this_cnt += static_cast<ll>(counter[y]) * counter[z];
            });
            ans += this_cnt;
        }

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_5200972368419325::solve();
    return 0;
}
