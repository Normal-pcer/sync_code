/**
 * @link https://www.luogu.com.cn/problem/P10194
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr(0)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
using ll = long long; using ull = unsigned long long;
int constexpr inf = 0x3f3f3f3f;  long long constexpr infLL = 0x3f3f3f3f3f3f3f3fLL;  char constexpr endl = '\n';

#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
    namespace views = std::views;
#endif
}

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 来观察一桶牛奶的变化。
 * 一桶牛奶现在的余量取决于这一路上的最小容量。
 * 例如：所有容量
 * 3 4 5 2 1
 * 第一桶奶：
 * 3 3 3 2 1
 * 第二桶奶：
 * 4 4 2 1 1
 * 以此类推。
 * 
 * 一桶牛奶的余量发生变化，仅当它处于的位置是前缀最小值。
 * 例如：4 3 5 2 3
 * 它每次转圈之后变为：
 * 4 3 5 2 3
 * 4 3 5 2 3 
 * 3 3 2 2 3
 * 3 2 2 2 3
 * 2 2 2 2 3
 * 2 2 2 2 2
 * 
 * 先考虑每桶奶在哪些时刻会变化？
 * v0: -1 on 1, -1 on 3
 * v1: -1 on 2
 * v2: -3 on 1
 * ...
 * 
 * 总结一下：
 * -4, -1, -1, -1
 * 答案即为其前缀和。
 * 
 * 4 3 5 2 中的“2”，在如下三个区间中充当了前缀最小值：
 * 4 3 5 2 (3 -> 2)
 * 3 5 2 (3 -> 2)
 * 5 2 (5 -> 2)
 * 
 * v0 -1 on 3, v2 - 1 on 2, v2 - 3 on 1
 * 
 * 从前往后遍历原数组，维护一个单调递增栈。
 * [0]4 [1]3 [2]5 [3]2 [4]3
 *                  ^
 * 
 * 此时的栈：
 * [1]3 [2]5 ([3]2)
 * [2, 2]：5 -> 2 （[1, 1] 减 3）
 * [0, 1]：3 -> 2 （[2, 3] 区间减 1）
 * 
 * 涉及到区间减，可以再维护它的差分数组。
 */
namespace Solution_9569068163890147 {
    void solve_force() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> limit(N);
        for (auto &x: limit)  std::cin >> x;
        auto val = limit;

        for (i32 i = 0; i < N; i++) {
            ranges::rotate(limit, limit.begin() + 1);
            for (i32 i = 0; i < N; i++)  chkMin(val[i], limit[i]);
            // for (auto x: val)  std::cout << x << " ";
            // std::cout << endl;
            std::cout << std::accumulate(val.begin(), val.end(), 0) << endl;
        }
    }
    // 好像有点乱。把差分数组封装一个类，提供区间加法的接口。
    // AD: adjacent_difference
    class ADArray {
        std::vector<i64> ad;
    public:
        ADArray(uz size = 0): ad(size) {}

        auto addRange(i32 begin, i32 end, i32 val) -> void {
            debug  std::cout << std::format("add [{}, {})  {}", begin, end, val) << std::endl;
            ad[begin] += val;
            if (end != static_cast<i32>(ad.size()))  ad[end] -= val;
        }

        auto get() -> std::vector<i64> {
            std::vector<i64> res(ad.size());
            std::partial_sum(ad.begin(), ad.end(), res.begin());
            return res;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> limit(N);
        for (auto &x: limit)  std::cin >> x;

        auto delta = [&]() {  // 每次操作的增量
            ADArray ada(N);

            // 倍长的 limit
            std::vector<i32> a(N + N);
            ranges::copy(limit, ranges::copy(limit, a.begin()).out);

            // 单调栈，存储 i 使得 limit[i] 单调递增（不严格）
            std::deque<i32> st;
            for (i32 i = 0; i < N + N; i++) {
                debug  std::cout << "i = " << i << std::endl;
                debug {
                    std::cout << "st:";
                    for (auto x: st)  std::cout << " " << x;
                    std::cout << std::endl;
                }
                // 维护单调栈
                while (not st.empty() and a[st.back()] > a[i]) {
                    auto cur = st.back();  st.pop_back();
                    auto back = st.empty()? -1: st.back();  // 左端点取 (back, cur] 的前缀最小值为 a[cur]
                    back = std::max(back, i - N);
                    auto delta = a[cur] - a[i];  // 区间减少的幅度
                    auto seg_begin = i - std::min(cur, N - 1);
                    auto seg_end = i - back;
                    debug  std::cout << "back = " << back << std::endl;
                    if (back + 1 < N) {  // 去重
                        ada.addRange(seg_begin, seg_end, -delta);
                    }
                }
                st.push_back(i);
            }

            return ada.get();
        }();

        i64 cur = std::accumulate(limit.begin(), limit.end(), (i64)0);
        for (auto x: delta | views::drop(1)) {
            cur += x;
            std::cout << cur << endl;
        }
        std::cout << cur;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9569068163890147::solve();
    return 0;
}
