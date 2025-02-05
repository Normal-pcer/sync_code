/**
 * @link https://www.luogu.com.cn/problem/P9029
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

namespace Solution_1567358908762099 {
    template <typename Value, typename Target, typename Func, typename Compare = std::less<>>
    auto lower_bound_value(Value begin, Value end, Target val, Func &&getter, Compare &&cmp = {}, Value eps = 1) {
        while (end - begin >= eps) {
            auto mid = begin + ((end - begin) >> 1);
            if (cmp(getter(mid), val)) {
                begin = mid + eps;
            } else {
                end = mid;
            }
        }
        return begin;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        std::vector<int> a(N);
        for (auto &x: a)  std::cin >> x;
        
        // 将所有物品按照价格排序
        ranges::sort(a, ranges::less{});
        std::vector<ll> ps(a.begin(), a.end());
        std::partial_sum(ps.begin(), ps.end(), ps.begin());
        ps.insert(ps.begin(), 0LL);

        // 选取一个物品可以对 l-f 产生多少贡献？
        // 如果价格 c < K 那么自然为 c
        // 否则为 K - (c - K) = 2K - c
        // 记这个贡献为 f(i)
        // 这个值在整个序列中的变化情况是先增加再减小
        // 希望加和最小，一定是从头尾各自选一段
        // 可以在头尾维护两个指针，每次取两者中比较小的那个，一共取 M 个
        // 这样的复杂度是 O(QN)
        // 考虑优化，对于一个给定的 y，可以二分确定有 cnt 个小于 y 的贡献
        // 显然 cnt 关于这个 y 单调递增
        // 只需要二分找出这个 y（希望 cnt = N），将左右两端连续段的贡献求和即可
        // 通过前缀和可以 O(1) 查询两段的和，两层二分复杂度 O(Q log^2(N))
        while (Q --> 0) {
            int K, M;  std::cin >> K >> M;
            // 定义函数，通过传入的 y 计数有多少段贡献满足条件
            auto get_points = [&](int y) -> auto {
                auto right_proj = lam(c, K + K - c);  // 右半部分需要应用的映射
                // 找到峰值，即 c = K 的位置
                auto highest = ranges::lower_bound(a, K);
                // [begin, highest) 是递增的，[highest, end) 是递减的
                // 分别在左侧和右侧找到第一个等于 y 的贡献
                auto left_target = ranges::upper_bound(a.begin(), highest, y, std::less{});  // [begin, left) 小于等于 y
                auto right_target = ranges::lower_bound(highest, a.end(), y, std::greater{}, right_proj);  // [right, end)
                never std::cout << std::format("l = {}, r = {}", std::distance(a.begin(), left_target), std::distance(right_target, a.end())) << std::endl;

                return std::pair{left_target, right_target};
            };
            auto f = [&](int y) -> int {
                auto [left, right] = get_points(y);
                auto cnt = std::distance(a.begin(), left) + std::distance(right, a.end());
                debug  std::cout << std::format("f({}) = {}", y, cnt) << std::endl;
                return cnt;
            };
            auto get_ans = [&](int y) -> ll {
                auto [left, right] = get_points(y);
                auto left_index = static_cast<ll>(std::distance(a.begin(), left));
                auto right_index = static_cast<ll>(std::distance(a.begin(), right));
                auto right_count = N - right_index;
                auto left_sum = ps.at(left_index);
                auto right_sum = ps.back() - ps.at(right_index);  // c 之和
                right_sum = 2 * right_count * K - right_sum;  // 右侧实际需要 cnt 个 (2K - c) 之和
                auto sum = left_sum + right_sum;
                // 可以通过去掉若干个较大者来减到 M 个元素
                auto cur_count = right_count + left_index;
                assert(cur_count >= M);
                auto max = std::max(
                            (left_index == 0? -inf: a[left_index-1]), 
                            (right_index == N? -inf: 2 * K - a[right_index])
                        );
                assert(max > -inf);
                auto reduce_count = cur_count - M;
                auto reduce_amout = reduce_count * max;
                return sum - reduce_amout;
            };
            
            // y 的取值范围是 (-inf, K+1)
            // f(y) 随 y 单调递增，希望令 y 最小
            auto y = lower_bound_value(-inf, K+1, M, f);
            debug  std::cout << "y = " << y << std::endl;
            std::cout << get_ans(y) << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    try {
        Solution_1567358908762099::solve();
    } catch (...) {
        std::cout << "I AK IOI" << std::endl;
    }
    return 0;
}
