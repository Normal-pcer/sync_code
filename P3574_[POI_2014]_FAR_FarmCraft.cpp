/**
 * @link https://www.luogu.com.cn/problem/P3574
 */
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

/*
感觉可以先研究一棵子树下的解法。
F[i] 表示所有人都安装好软件的最早时间。
G[i] 表示遍历整棵子树所需的时间。

一个比较贪心的做法是，先访问需要等待时间比较长的子树。
这个贪心应该是正确的，因为在此基础上再交换两个子树的顺序一定是更劣的。
*/
namespace Solution_6182671903718080 {
    auto solve() -> void {
        using Point = i32;
        Point maxPoint;
        std::cin >> maxPoint;

        using Time = i32;
        std::vector<Time> waitTime(maxPoint + 1);
        for (auto &x: waitTime | views::drop(1)) std::cin >> x;

        std::vector graph(maxPoint + 1, std::vector<i32>{});
        for (i32 i = 0; i < (maxPoint - 1); ) {
            Point x, y;
            std::cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x);
        }

        std::vector F(maxPoint + 1, (Time)0);
        std::vector G(maxPoint + 1, (Time)0);

        auto dfs = [&](auto &&dfs, Point p, Point prev) -> void {
            auto isLeaf = (prev == 0 and graph[p].size() == 0)
                        or (prev != 0 and graph[p].size() == 1);
            if (isLeaf) {
                // 需要让自己先安装好软件
                F[p] = waitTime[p];
                G[p] = 0;
                return;
            }

            // 贪心策略：按照子树的 
        };
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_6182671903718080::solve();
    return 0;
}
