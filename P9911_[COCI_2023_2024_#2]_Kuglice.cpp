/**
 * @link https://www.luogu.com.cn/problem/P9911
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
 * 双端队列，两个人从任意一端取数
 * 第一次拿到一个颜色即可得分
 * 1 1 2 1 3
 * 
 * 取若干个之后一定剩下一个连续区间 [l, r]
 * F[l][r] 表示一个玩家面对上述区间做出最佳抉择后，比对手多的分数
 * F[l][r] <= -F[l+1][r] + ?
 * 此时取走的数为 a[l]，可以判断一下这个数是否为第一次出现（即 [0, l) Union [r, N) 区间中不存在这个数）
 * 如果是第一次出现，玩家可以获得 1 分
 * 同样地，也可以从 F[l][r+1] 转移
 * 
 * 考虑上述的 check 怎么写。
 * 如果使用记忆化搜索，每层递归都会多一个出现过的数，使用一个 multiset 维护即可。
 */
namespace Solution_7015738533239413 {
    class Game {
        int N;
        std::vector<int> deque;
        std::vector<std::vector<int>> F;

        auto getF(int l, int r, std::vector<int> &counter) -> int {
            if (l > r)  return 0;  // 空区间
            auto check = [&](int x) -> bool { return counter[x] == 0; };

            if (F[l][r] <= -inf) {
                auto take_left = static_cast<int>(check(deque[l]));
                counter.at(deque[l])++;
                take_left -= getF(l+1, r, counter);
                counter.at(deque[l])--;

                auto take_right = static_cast<int>(check(deque[r]));
                counter.at(deque[r])++;
                take_right -= getF(l, r-1, counter);
                counter.at(deque[r])--;

                F[l][r] = std::max(take_left, take_right);
            }

            return F[l][r];
        }
    public:
        Game(int N, std::vector<int> const &deque): 
                N(N), deque(deque), F(N, std::vector(N, -inf)) {}
        auto getAns() -> std::pair<int, int> {
            std::vector<int> counter(N+1);
            auto diff = getF(0, N-1, counter);

            std::vector<int> unique(deque);
            std::sort(unique.begin(), unique.end());
            auto it = std::unique(unique.begin(), unique.end());
            auto K = static_cast<int>(std::distance(unique.begin(), it));
            // a - b = diff, a + b = K（去重后的数量）
            return {(diff + K) >> 1, (K - diff) >> 1};
        };
    };
    void solve() {
        int N;  std::cin >> N;
        std::vector<int> deque(N);
        for (auto &x: deque)  std::cin >> x;

        Game game{N, deque};
        auto [a, b] = game.getAns();
        std::cout << a << ":" << b << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7015738533239413::solve();
    return 0;
}
