/**
 * @link https://www.luogu.com.cn/problem/P3166
 */
#include "./libs/debug_macros.hpp"


#include "./lib_v4.hpp"


using namespace lib;

/**
 * 数三角形，只需求出有多少个三元组三点共线。
 */
namespace Solution_1658374306243153 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        auto const MaxY = N, MaxX = M;  // 横轴为 x

        struct Segment {
            int dx, dy;
        };
        auto count_same = [&](Segment seg) -> ll {
            if (seg.dx > MaxX)  return 0;
            if (seg.dy > MaxY)  return 0;
            // 数出一个线段经过平移有多少种全等的线段
            return static_cast<ll>(MaxX - std::abs(seg.dx) + 1) * (MaxY - std::abs(seg.dy) + 1);
        };

        // 枚举所有的线段种类
        // 固定网格原点为一个端点，枚举另一个端点并倍长
        auto collinear = 0LL;
        for (auto x = 0; x <= MaxX; x++) {
            for (auto y = 0; y <= MaxY; y++) {
                if (x == 0 and y == 0)  continue;
                // 这条线段上有多少格点？
                auto point_cnt = std::gcd(x, y) - 1;
                // 原点、每个格点、终点之间是一组三点共线
                auto del = count_same({x, y}) * point_cnt;
                // 如果不是水平或者竖直，可以有一个对称的
                if (x != 0 and y != 0)  del += del;
                collinear += del;
                debug  std::cout << std::format("x = {}, y = {} -> {}", x, y, del) << std::endl;
            }
        }
        auto points = static_cast<ll>(N + 1) * static_cast<ll>(M + 1);
        auto triangles = points * (points - 1) * (points - 2) / 6 - collinear;
        std::cout << triangles << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1658374306243153::solve();
    return 0;
}
