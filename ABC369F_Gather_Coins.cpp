/**
 * @link https://www.luogu.com.cn/problem/AT_abc369_f
 */

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define from(i,b,e) for(auto i=(b);i<(e);i++)
#define rev(i,e,b) for(auto i=(e);i>(b);i--)
#define main() main(int argc, char const *argv[])
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
#define always if constexpr(1)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{}
#if __cplusplus > 201703LL
namespace rgs {  using namespace std::ranges; using namespace std::views;  }
#endif


namespace lib {   
    template <typename T> auto range(const T to) {  return std::views::iota(T(), to);  }
    template <typename T> auto range(const T begin, const T end) {  return std::views::iota(begin, end);  }
    template <typename T, typename U> struct StepViewIterator {
        T _cur;  const U _step;
        StepViewIterator(T cur, const U step): _cur(cur), _step(step) {}
        auto operator++() { _cur+=_step; }  auto operator++(int) { _cur+=_step; }
        auto operator!=(const StepViewIterator& other) { return _cur<other._cur; }
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

namespace Solution_1778585983417334 {

    const int _N = 262'144;
    struct Node {
        int val;
        int owner;

        auto operator<=> (const Node&) const = default;
    } c[_N];

    inline int lowbit(int x) { return x & -x; }
    void update(int x, Node val) {
        while (x < _N) {
            chkMax(c[x], val);
            x += lowbit(x);
        }
    }
    auto query(int x) {
        auto res = (Node){0, 0};
        while (x) {
            chkMax(res, c[x]);
            x -= lowbit(x);
        }
        return res;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int H, W, N;
        std::cin >> H >> W >> N;
        std::vector<std::pair<int, int>> coins(N+2);

        *coins.begin() = std::make_pair(1, 1);
        for (auto &[x, y]: coins | rgs::drop(1) | rgs::take(N))  std::cin >> x >> y;
        *coins.rbegin() = std::make_pair(H, W);
        rgs::sort(coins);

        // for (auto [x, y]: coins)  std::cout << x << ' ' << y << std::endl;
        // std::cout << "------" << std::endl;

        std::vector<int> prev(N+2);
        for (auto i: range(1, (int)coins.size())) {
            auto y = coins[i].second;
            auto [max, owner] = query(y);
            // debug std::cout << std::format("{} <- ({}, {})", i, max, owner) << std::endl;
            prev[i] = owner;
            update(y, {max+1, i});
        }

        std::vector<int> steps;
        auto cur = coins.size() - 1;
        while (cur)  steps.push_back(cur), cur = prev[cur];
        steps.push_back(0);

        std::reverse(steps.begin(), steps.end());
        // for (auto i: steps)  std::cout << std::format("({}, {})", coins[i].first, coins[i].second) << ' ';
        // std::cout << std::endl;

        std::string res;
        res.reserve(H+W+1);
        for (auto i: range(1, (int)steps.size())) {
            auto [x0, y0] = coins[steps[i-1]];
            auto [x1, y1] = coins[steps[i]];

            auto down = x1 - x0;
            auto right = y1 - y0;
            for (auto _: range(down))  res += 'D';
            for (auto _: range(right))  res += 'R';
        }

        std::cout << query(W).val - 1 << std::endl;
        std::cout << res << std::endl;
    }
}

int main() {
    initDebug;
    Solution_1778585983417334::solve();
    return 0;
}