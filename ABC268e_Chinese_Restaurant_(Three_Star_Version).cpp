/**
 * @link https://www.luogu.com.cn/problem/AT_abc268_e
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

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
namespace Solution_1553766536623869 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> p(N);
        std::vector<int> item(N);
        for (auto &x: item)  std::cin >> x;
        for (auto i = 0; i < N; i++) {
            p[item[i]] = i;
        }

        // 先处理出每个人和自己的菜的初始距离
        std::vector<int> init_dis(N);
        for (auto i = 0; i < N; i++) {
            init_dis[i] = (p[i] - i + N) % N;
        }

        // 维护三个范围，需要四个指针 p[4]
        // [p0, p1) 会在下一步 +1
        // [p1, p2) 会在下一步不变
        // [p2, p3) 会在下一步 -1
        // 令 M = (N-1) / 2
        // 需要三个范围分别小于 M，等于 M，大于 M
        // 另外均需要保证在 [0, N)
        // 一个比较取巧的方式可能是直接使用 equal_range 二分查找，复杂度仍为 log，不会更劣
        std::vector<int> dis(init_dis);
        ranges::sort(dis);
        dis.resize(N*2);
        for (auto i = 0; i < N; i++)  dis[i + N] = dis[i], dis[i] -= N;

        std::vector<int> dis2(dis);  // 直接乘以 2，规避浮点数计算
        for (auto &x: dis2)  x += x;
        auto add_tag2 = 0;  // 所有数都需要加上 add_tag，这个数是 add_tag 的一半

        for (auto &x: init_dis)  x = (x + N) % N, x = std::min(x, N - x);
        auto sum = std::accumulate(init_dis.begin(), init_dis.end(), 0LL);  // 算出初始的距离和
        auto ans = sum;
        auto M2 = N - 1;  // M 的二倍
        for (auto i = 0; i < N; i++) {
            auto p0 = std::lower_bound(dis2.begin(), dis2.end(), 0 - add_tag2);
            auto p3 = std::lower_bound(dis2.begin(), dis2.end(), N * 2 - add_tag2);
            auto [p1, p2] = std::equal_range(dis2.begin(), dis2.end(), M2 - add_tag2);
            debug  std::cout << (p0 - dis2.begin()) << " "
                    << (p1 - dis2.begin()) << " "
                    << (p2 - dis2.begin()) << " "
                    << (p3 - dis2.begin()) << std::endl;

            auto less = std::distance(p0, p1);
            auto greater = std::distance(p2, p3);
            
            sum += less, sum -= greater;
            debug  std::cout << "delta = " << less - greater << std::endl;
            chkMin(ans, sum);
            add_tag2 += 2;
        }
        assert(sum == std::accumulate(init_dis.begin(), init_dis.end(), 0LL));

        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1553766536623869::solve();
    return 0;
}
