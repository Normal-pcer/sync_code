/**
 * @link https://www.luogu.com.cn/problem/P9179
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
namespace lib {   
    template <typename T> auto range(const T to) {  return std::views::iota(T(), to);  }
    template <typename T> auto range(const T begin, const T end) {  return std::views::iota(begin, end);  }
    template <typename T, typename U> struct StepViewIterator {
        T _cur;  const U _step;
        StepViewIterator(T cur, const U step): _cur(cur), _step(step) {}
        auto operator++() { _cur+=_step; }  auto operator++(int) { _cur+=_step; }
        auto operator!=(const StepViewIterator& other) { return _step>=0? _cur<other._cur: _cur>other._cur; }
        auto operator*() { return _cur; }
    };
    template <typename T, typename U> struct StepView: std::ranges::view_interface<StepView<T, U>> {
        const T _begin, _end;  const U _step;
        StepView(const T begin, const T end, const U step): _begin(begin), _end(end), _step(step) {}
        auto begin() const { return StepViewIterator<T, U>(_begin, _step); }
        auto end() const { return StepViewIterator<T, U>(_end, _step); }
    };
    template <typename T, typename U> auto range(T begin, T end, U step) { return StepView<T, U>(begin, end, step); }
}
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
