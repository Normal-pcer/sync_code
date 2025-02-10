/**
 * @link https://www.luogu.com.cn/problem/P9029
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

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
