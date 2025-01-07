/**
 * @link https://www.luogu.com.cn/problem/AT_abc246_e
 * @link https://www.luogu.com.cn/remoteJudgeRedirect/atcoder/abc246_e
 */

#ifdef ONLINE_JUDGE
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#endif

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1524404798318523 {
    constexpr const char blank = '.';
    int N;  constexpr const int _N = 1500;
    std::array<std::array<char, _N>, _N> map;
    std::array<std::array<std::array<bool, 4>, _N>, _N> vis;
    int bfs(int x1, int y1, int x2, int y2) {
        struct Point {
            int x, y, val = 0, dir = 0;  // dir：方向所在象限
        };
        std::deque<Point> q{{x1, y1}};
        while (not q.empty()) {
            auto [x, y, val, dir] = q.front();  q.pop_front();
            debug  std::cout << x << ' ' << y << ' ' << endl;
            if (x < 0 or x >= N or y < 0 or y >= N)  continue;
            if (x == x2 and y == y2)  return val;
            if (map[x][y] != blank)  continue;
            if (dir != 0 and vis[x][y][dir - 1])  continue;
            vis[x][y][dir - 1] = true;
#define WALK(i, j, new_dir) \
                if (dir == new_dir) { \
                    q.push_front({i, j, val, new_dir}); \
                } else { \
                    q.push_back({i, j, val + 1, new_dir}); \
                }
            WALK(x+1, y+1, 1);
            WALK(x-1, y+1, 2);
            WALK(x-1, y-1, 3);
            WALK(x+1, y-1, 4);
        }
        return -1;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        std::cin >> N;
        int x1, y1, x2, y2;  std::cin >> x1 >> y1 >> x2 >> y2;
        for (auto i = 0; i < N; i++) {
            auto &line = map[i];
            for (auto j = 0; j < N; j++) {
                std::cin >> line[j];
            }
        }
        std::cout << bfs(x1-1, y1-1, x2-1, y2-1) << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1524404798318523::solve();
    return 0;
}
