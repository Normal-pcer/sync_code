/**
 * @link https://www.luogu.com.cn/problem/AT_abc268_e
 */
#pragma GCC optimize(3)
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/io.hpp"

using namespace lib;

/**
 * 用一个数表示人和菜的位置关系
 * 每次旋转一次桌子，所有位置关系都会 +1
 * 人和菜的距离是与这个数同余的绝对值最小的数
 * 
 * 如果 N 为奇数，M = floor(N/2)；否则不存在等于 M
 * 把这个距离排序，分为小于、等于、大于 N/2 的三段
 * 等于 N/2 的段在一个环形上左移
 * 
 * 每次旋转，小于段提供的距离 +1，大于段提供的距离 -1，等于段提供的距离不变
 * 通过双指针重新确定位置。移动的距离一定为 N，所以复杂度为 O(N)
 * 错误的。排序复杂度带个 log。
 */
constexpr const int MaxN = 2e5+1;
std::array<int, MaxN> p;
std::array<int, MaxN> dis2;
auto constexpr near_mod(int x, const int mod) -> int {
    // assert(-mod <= x and x < mod);
    return x + (-(x < 0) & mod);
}
namespace Solution_1553766536623869 {
    void solve() {
        int N;  io >> N;
        for (auto i = 0; i < N; i++)  io >> p[i];

        // 先处理出每个人和自己的菜的初始距离
        auto sum = 0;
        for (auto i = 0; i < N; i++) {
            dis2[i] = near_mod(p[i] - i, N);
            sum += std::min(dis2[i], N - dis2[i]);
            dis2[i] += dis2[i];
        }

        // 维护三个范围，需要四个指针 p[4]
        // [p0, p1) 会在下一步 +1
        // [p1, p2) 会在下一步不变
        // [p2, p3) 会在下一步 -1
        // 令 M = (N-1) / 2
        // 需要三个范围分别小于 M，等于 M，大于 M
        // 另外均需要保证在 [0, N)
        // 一个比较取巧的方式可能是直接使用 equal_range 二分查找，复杂度仍为 log，不会更劣
        std::sort(dis2.begin(), dis2.begin() + N);
        for (auto i = 0; i < N; i++)  dis2[i + N] = dis2[i], dis2[i] -= N + N;

        auto add_tag2 = 0;  // 所有数都需要加上 add_tag，这个数是 add_tag 的一半

        auto ans = sum;
        auto M2 = N - 1;  // M 的二倍
        for (auto i = 0; i < N; i++) {
            auto p0 = std::lower_bound(dis2.begin(), dis2.end(), 0 - add_tag2);
            auto p3 = std::lower_bound(dis2.begin(), dis2.end(), N + N - add_tag2);
            auto [p1, p2] = std::equal_range(dis2.begin(), dis2.end(), M2 - add_tag2);

            auto less = std::distance(p0, p1);
            auto greater = std::distance(p2, p3);
            
            sum += less, sum -= greater;
            chkMin(ans, sum);
            add_tag2 += 2;
        }

        io << ans << endl;
    }
}

int main(int, char const **) {
    Solution_1553766536623869::solve();
    return 0;
}
