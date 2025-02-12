/**
 * @link https://www.luogu.com.cn/problem/AT_abc269_d
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

/**
 * 沐咕爷爷用违禁词击落洛谷讨论区
 * 
 * 图中的编号方式
 * 两个维度 +1：右上
 * x +1：右
 * y +1：左上
 * 
 * dfs 直接走就行
 */
namespace Solution_1648457765803292 {
    constexpr const int MaxMapSize = 2010;
    constexpr const int MapOffset = 1005;
    constexpr const int MaxPosition = 1000;
    template <typename T>
    using MapType = std::array<std::array<T, MaxMapSize>, MaxMapSize>;
    MapType<bool> vis;
    MapType<bool> hasPoint;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<std::pair<int, int>> points(N);
        for (auto &[x, y]: points)  std::cin >> x >> y, hasPoint[x + MapOffset][y + MapOffset] = true;

        auto isVisited = [&](int x, int y) -> bool & {
            return vis[x + MapOffset][y + MapOffset];
        };
        auto dfs = [&](auto &&self, int x, int y) -> void {
            if (x < -MaxPosition or x > MaxPosition)  return;
            if (y < -MaxPosition or y > MaxPosition)  return;
            if (not hasPoint[x + MapOffset][y + MapOffset])  return;
            if (isVisited(x, y))  return;
            isVisited(x, y) = true;

            auto walk = [&](int dx, int dy) -> void {
                self(self, x + dx, y + dy);
            };
            walk(+1, +1), walk(-1, -1);
            walk(+1, 0), walk(-1, 0);
            walk(0, +1), walk(0, -1);
        };

        auto ans = 0;
        for (auto [x, y]: points) {
            if (isVisited(x, y))  continue;
            else  ans++, dfs(dfs, x, y);
        }

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1648457765803292::solve();
    return 0;
}
