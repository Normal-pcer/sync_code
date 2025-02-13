/**
 * @link https://www.luogu.com.cn/problem/AT_abc275_c
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1445520369143837 {
    constexpr const int N = 9;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::vector<std::string> mat(N);
        for (auto &s: mat)  std::cin >> s;

        using Point = std::pair<int, int>;
        std::vector<Point> points;
        for (auto i = 0; i < N; i++) {
            for (auto j = 0; j < N; j++) {
                if (mat[i][j] == '#')  points.push_back({i, j});
            }
        }

        auto len = [&](Point x, Point y) -> int {
            auto [xx, xy] = x;
            auto [yx, yy] = y;
            return (xx - yx) * (xx - yx) + (xy - yy) * (xy - yy);
        };

        // 枚举所有点
        auto cnt = 0;
        for (auto x: points) {
            for (auto y: points) {
                for (auto z: points) {
                    for (auto w: points) {
                        if (x == y or y == z or z == w or x == z or y == w or x == w)  continue;
#define CALC_LEN(x, y) auto x##y = len(x, y);
                        CALC_LEN(x, y);
                        CALC_LEN(x, z);
                        CALC_LEN(x, w);
                        CALC_LEN(y, z);
                        CALC_LEN(y, w);
                        CALC_LEN(z, w);

                        auto flag = xy == yz and yz == zw and zw == xw;
                        flag &= xz == yw;
                        if (flag)  cnt++;
                    }
                }
            }
        }

        std::cout << (cnt / 8) << std::endl;  // 显然会重复算
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1445520369143837::solve();
    return 0;
}
