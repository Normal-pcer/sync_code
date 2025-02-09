/**
 * @link https://www.luogu.com.cn/problem/P9759
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
 * 一条路径上的乘积是 k 的倍数，要求这条路径能集齐 k 的所有质因子
 * 考虑 F[i][j][...] 表示走到点 (i, j) 时质因子数量满足条件的方案数
 * 考虑如何描述每个质因子的数量
 * k <= 1e6，那么它最多有多少种质因子
 * 2 * 3 * 5 * 7 * 11 * 13 * 17 = 5e5，所以最多是七个质因子
 * 那直接用 map<char[7], int> 好像就可以
 * 先处理出来 k 的所有质因子，从小到大编号
 * 内层数组的每个数表示对应质因子的出现次数
 * 从左边或者右边转移，所有素数加上当前点的
 * 超时了难绷 https://www.luogu.com.cn/record/201735515
 * 没必要用 char[7]，把它们乘起来其实就好了
 * 这时候 key 的意义是这个数与 K 的 gcd
 * map<int, int> 即可
 */
namespace Solution_1404362619582699 {
    constexpr const int mod = 998244353;

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, K;  std::cin >> N >> K;
        std::vector grid(N, std::vector<int>(N));
        for (auto &row: grid) {
            for (auto &item: row) {
                std::cin >> item;
            }
        }

        std::vector F(N, std::vector<std::unordered_map<int, int>>(N));
        F[0][0][std::gcd(grid[0][0], K)] = 1;  // 起点
        for (auto i: range(N)) {
            for (auto j: range(N)) {
                if (i == 0 and j == 0)  continue;
                if (grid[i][j] == -1)  continue;
                auto transform_from = [&](int x, int y) -> void {
                    if (x < 0 or y < 0 or x >= N or y >= N)  return;
                    for (auto [stat, cnt]: F[x][y]) {
                        auto new_stat = std::gcd<ll>(static_cast<ll>(stat) * grid[i][j], K);
                        F[i][j][new_stat] = (F[i][j][new_stat] + cnt) % mod;
                    }
                };
                transform_from(i-1, j), transform_from(i, j-1);
            }
        }

        debug {
            for (auto i: range(N)) {
                for (auto j: range(N)) {
                    for (auto [k, cnt]: F[i][j]) {
                        std::cout << std::format("F[{}][{}][{}] = {}", i, j, k, cnt) << std::endl;
                    }
                }
            }
        }

        std::cout << F[N-1][N-1][K] << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1404362619582699::solve();
    return 0;
}
