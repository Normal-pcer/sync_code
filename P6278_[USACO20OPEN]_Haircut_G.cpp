/**
 * @link https://www.luogu.com.cn/problem/P6278
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
 * 查询 j：两个点 A, B 之间构成逆序对
 * 要求：index[A] < index[B]，min(j, val[A]) > min(j, val[B])
 * 当两个点中较小的那一个小于 j 之后，这一对不再会被计算
 * F[i] 表示：以 i 为较小值的逆序对数量
 * 查询时减掉一个前缀和
 */
namespace Solution_1490717655565874 {
    class BIT {
        std::vector<ll> c;
        static constexpr auto lowbit(int x) -> int { return x & -x; }
    public:
        BIT(int N): c(N+1) {}

        auto prefixSum(int x) const -> ll {
            x++;  auto res = 0LL;
            while (x != 0) {
                res += c[x];
                x -= lowbit(x);
            }
            return res;
        }
        auto addAt(int x, ll val) -> void {
            x++;
            while (x < static_cast<int>(c.size())) {
                c[x] += val;
                x += lowbit(x);
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> val(N);
        for (auto &x: val)  std::cin >> x;

        std::vector<ll> F(N+1);
        BIT bit{N+1};
        auto cnt = 0;
        for (auto x: val) {
            auto less_or_eq = bit.prefixSum(x);
            auto greater = cnt - less_or_eq;
            F[x] += greater;
            bit.addAt(x, 1);
            cnt++;
        }

        // for (auto i = 0ULL; i != F.size(); i++)  std::cout << "F[" << i << "] = " << F[i] << std::endl;
        std::vector<ll> F_ps(F.begin(), F.end());
        std::partial_sum(F_ps.begin(), F_ps.end(), F_ps.begin());

        std::cout << 0 << endl;
        for (auto j = 0; j < N - 1; j++) {
            auto ans = F_ps[j];
            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1490717655565874::solve();
    return 0;
}
