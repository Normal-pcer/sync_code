/**
 * @link https://www.luogu.com.cn/problem/P9178
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
 * hamming(x, y) 即 popcount(x xor y)
 * 以下的 ~ 符号均表示 M 位数按位取反
 * 对于一个 M 位数 x，显然有 ham(x, y) + ham(~x, y) = M
 * H[x] 表示 x 与另一个数的最小距离
 * 
 * 初始值：
 * x 在数组中 - H[x] = 0
 * 否则 - H[x] = inf
 * 转移：
 * x 的任意一位反转，可以从反转后的数转移，距离加一
 * H[x] <= H[x xor (1<<i)] + 1 (chkMin)
 * 
 * 转移顺序：i 递增，其次 x 递增
 * 首先可以断定，x -> y -> x 这样的转移不可能是最优
 * 枚举到最高位的时候，y 试图从 x 转移，而 x 是通过修改更低位（或不修改）转移来的。
 * 这是合理的。如果 x 必须要修改最高位，那就会是从 y 转移，一定不是最优。
 * 更低的位同理。
 * 
 * 答案：
 * 对于原序列每一个数 a[i]
 * 答案即为 M - H[~a[i]]
 */
namespace Solution_1422241632498212 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, M;  std::cin >> N >> M;

        auto inv = lam(x, x ^ ((1 << M) - 1));
        std::vector<int> a(N);
        for (auto &x: a)  std::cin >> x;

        std::vector<int> H(1 << M, inf);
        for (auto x: a)  H[x] = 0;
        for (auto i: range(M)) {
            for (auto x: range(1 << M)) {
                chkMin(H[x], H[x ^ (1<<i)] + 1);
            }
        }
        for (auto x: a)  std::cout << (M - H[inv(x)]) << " ";
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1422241632498212::solve();
    return 0;
}
