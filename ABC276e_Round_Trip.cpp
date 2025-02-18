/**
 * @link https://www.luogu.com.cn/problem/AT_abc276_e
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_1071586754871606 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<std::string> mat(N);
        for (auto &s: mat) {
            std::cin >> s;
        }

        i32 sx = -1, sy = -1;
        for (i32 i = 0; i < N; i++) {
            for (i32 j = 0; j < N; j++) {
                if (mat[i][j] == 'S')  sx = i, sy = j;
            }
        }

        auto find = [&](int sx, int sy, int tx, int ty, auto &&mat) -> bool {
            struct Point {
                int x = -1, y = -1, dis = 0;
            };
            
            std::vector vis(N, std::vector<char>(M));
            std::deque<Point> q;

            q.push_back({sx, sy, 0});
            while (not q.empty()) {
                auto [x, y, dis] = q.front();  q.pop_front();
                if (x < 0 or x >= N)  continue;
                if (y < 0 or y >= M)  continue;
                if (mat[x][y] == '#')  continue;
                if (mat[x][y] == 'S')  continue;
                if (x == tx and y == ty)  return true;
                if (vis[x][y])  continue;
                vis[x][y] = true;

                for (auto del: {0x21, 0x01, 0x12, 0x10}) {
                    auto dx = (del >> 4) - 1, dy = (del & 15) - 1;
                    q.push_back({x + dx, y + dy, dis + 1});
                }
            }
            return false;
        };

        auto near4 = [&](int sx, int sy, auto &&f) -> void {
            for (auto del: {0x21, 0x01, 0x12, 0x10}) {
                auto dx = (del >> 4) - 1, dy = (del & 15) - 1;
                if (0 <= sx + dx and sx + dx < N) {
                    if (0 <= sy + dy and sy + dy < M) {
                        f(sx + dx, sy + dy);
                    }
                }
            }
        };

        near4(sx, sy, [&](int x1, int y1) -> void {
            near4(sx, sy, [&](int x2, int y2) -> void {
                if (x1 == x2 and y1 == y2)  return;
                auto flag = find(x1, y1, x2, y2, mat);
                if (flag) {
                    std::cout << "Yes" << std::endl;
                    std::exit(0);
                }
            });
        });
        std::cout << "No" << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1071586754871606::solve();
    return 0;
}
