/**
 * @link https://www.luogu.com.cn/problem/P3128
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

namespace Solution_7216935459152744 {

    int N, K;
    const int _N = 5e4+5, _ST = 20;
    int ST;
    std::vector<std::vector<int>> graph;

    std::vector<int> diff;  // 点权的差分
    std::vector<int> val;
    std::vector<int> depth;
    std::vector<std::vector<int>> F;  // 倍增

    void dfs_pre(int p, int prev) {
        depth.at(p) = depth.at(prev) + 1;
        F.at(p).at(0) = prev;
        for (auto i: range(1, (int)F.at(p).size())) {
            F.at(p).at(i) = F.at( F.at(p).at(i-1) ).at(i-1);
        }
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_pre(dest, p);
        }
    }

    std::pair<int, bool> LCA(int x, int y) {
        auto distance = depth.at(x) - depth.at(y);
        for (auto i = 0; i < ST; i++) {
            if (distance & (1<<i)) {
                x = F.at(x).at(i);
            }
        }
        if (x == y)  return {x, true};
        for (int i = ST-1; i >= 0; i--) {
            if (F.at(x).at(i) != F.at(y).at(i)) {
                x = F.at(x).at(i), y = F.at(y).at(i);
            }
        }

        return {F.at(x).at(0), false};
    }

    void modify(int x, int y) {
        if (depth.at(x) < depth.at(y))  std::swap(x, y);
        auto [ancestor, single] = LCA(x, y);

        if (single)  /*std::cout << '!' << std::endl,*/ diff.at(x)++, diff.at(F.at(y).at(0))--;
        else  diff.at(x)++, diff.at(y)++, diff.at(ancestor)--, diff.at(F.at(ancestor).at(0))--;
    }

    void dfs_finally(int p, int prev) {
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs_finally(dest, p);
            val.at(p) += val.at(dest);
        }
        val.at(p) += diff.at(p);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N >> K;
        graph.resize(N+1);
        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y), graph.at(y).push_back(x);
        }

        ST = std::__lg(N) + 1;
        depth.resize(N+1), std::fill_n(std::back_inserter(F), N+1, std::vector<int>(ST));
        diff.resize(N+1), val.resize(N+1);

        dfs_pre(1, 0);
        for (auto _: range(K)) {
            int x, y;  std::cin >> x >> y;
            modify(x, y);

            debug {
                for (auto i: range(1, N+1))  std::cout << std::format("[{}]{}", i, diff.at(i)) << ' ';
                std::cout << std::endl;
            }
        }

        dfs_finally(1, 0);

        std::cout << rgs::max(val) << std::endl;
    }
}

int main() {
    initDebug;
    Solution_7216935459152744::solve();
    return 0;
}