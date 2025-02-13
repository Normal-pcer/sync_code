/**
 * @link https://www.luogu.com.cn/problem/AT_abc275_c
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

namespace Solution_1758901606298503 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        // int N;  std::cin >> N;
        constexpr const int N = 9;
        std::vector<std::string> mat(N);
        for (auto &s: mat)  std::cin >> s;

        auto get = [&](int x, int y) -> bool {
            if (x < 0 or x >= N)  return false;
            if (y < 0 or y >= N)  return false;
            return mat[x][y] == '#';
        };

        std::vector<std::pair<int, int>> points;
        for (auto i = 0; i < N; i++) {
            for (auto j = 0; j < N; j++) {
                if (mat[i][j] == '#')  points.push_back({i, j});
            }
        }

        // 枚举底边
        auto cnt = 0;
        for (auto [px, py]: points) {
            for (auto [qx, qy]: points) {
                if (px == qx and py == qy)  continue;
                // if (py >= qy)  continue;
                auto dx = qx - px, dy = qy - py;
                auto rx = qx - dy, ry = qy + dx;
                auto sx = px - dy, sy = py + dx;
                // std::cout << px << " " << py << " " << qx << " " << qy << " " << rx << " " << ry << " " << sx << " " << sy << std::endl;
                if (get(rx, ry) and get(sx, sy))  cnt++;
            }
        }

        std::cout << cnt / 4 << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1758901606298503::solve();
    return 0;
}
