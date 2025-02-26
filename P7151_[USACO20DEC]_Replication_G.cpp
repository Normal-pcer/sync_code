/**
 * @link https://www.luogu.com.cn/problem/P7151
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 显然，「分身」能够走到的地方，「本体」一定也可以走到。
 * 
 * 在一个点是否会被「卡脖子」，取决于它和最近的墙壁之间的距离，以及此时它的半径。
 * 先处理出每一个点与最近一块墙壁的距离（「曼哈顿距离」）。从每一块墙壁开始 bfs 即可。
 * 
 * 接下来，假设一个「宇宙机器人」到处乱跑，它的半径一定是逐渐增加的，所以一个缝隙一定要尽早通过才有机会。
 * 尽可能快地到达一个点，如果它在此时被「卡脖子」，以后肯定更加没有希望了。
 * bfs 模拟这个过程即可。在某个地方被「卡脖子」之后可以留下此时的大小。
 * 最后在每个被「卡脖子」的位置记录一下此时的大小即可。
 */
namespace Solution_3269067797949153 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<std::string> mat(N);

        for (auto &x: mat) {
            std::cin >> x;
        }

        // 所有的点距离墙的距离
        std::vector dis_to_wall(N, std::vector<i32>(N));
        [&]() {
            struct Node {
                i32 x, y;
                i32 dis = 0;
            };

            std::deque<Node> q;
            for (i32 i = 0; i < N; i++) {
                for (i32 j = 0; j < N; j++) {
                    if (mat[i][j] == '#')  q.push_back({i, j});
                }
            }

            std::vector vis(N, std::vector<char>(N));
            while (not q.empty()) {
                auto [x, y, dis] = q.front();  q.pop_front();
                if (x < 0 or x >= N)  continue;
                if (y < 0 or y >= N)  continue;
                if (mat[x][y] == '#')  continue;
                if (vis[x][y])  continue;
                vis[x][y] = true;
                dis_to_wall[x][y] = dis;

                std::array<i32, 4> constexpr dx_list = { -1, +1,  0,  0 };
                std::array<i32, 4> constexpr dy_list = {  0,  0, +1, -1 };

                for (i32 i = 0; i < 4; i++) {
                    auto dx = dx_list[i], dy = dy_list[i];
                    q.push_back({x + dx, y + dy, dis + 1});
                }
            }
        }();

        struct End {
            i32 x, y;
            i32 size;  // 在点 (x, y)，半径为 size 时被「卡脖子」
        };
        std::vector<End> ends;  // 所有被「卡脖子」的位置
        std::vector reach(N, std::vector<char>(N));  // 可以到达点 (i, j)
        [&]() {
            struct Node {
                int x, y;
                int size = 0;  // 半径为 size，在半径变为 size 之后又走了 steps 步
                int steps = 0;
            };
            std::deque<Node> q;

            for (i32 i = 0; i < N; i++) {
                for (i32 j = 0; j < N; j++) {
                    if (mat[i][j] == 'S') {
                        q.push_back({i, j});
                    }
                }
            }
            std::vector vis(N, std::vector<char>(N));
            while (not q.empty()) {
                auto p = q.front();  q.pop_front();
                if (p.x < 0 or p.x >= N)  continue;
                if (p.y < 0 or p.y >= N)  continue;
                if (mat[p.x][p.y] == '#')  continue;
                if (vis[p.x][p.y])  continue;
                // 判断撞墙
                if (dis_to_wall[p.x][p.y] >= p.size) {
                    
                    continue;
                }

                vis[p.x][p.y] = true;

                std::array<i32, 4> constexpr dx_list = { -1, +1,  0,  0 };
                std::array<i32, 4> constexpr dy_list = {  0,  0, +1, -1 };

                i32 new_size = p.size, new_steps = p.steps + 1;
                if (new_steps == M)  new_steps = 0, new_size++;  // 变大
                if (dis_to_wall[p.x][p.y] >= new_size) {
                    ends.push_back({p.x, p.y, p.size});  // 停止动作
                    continue;
                }
                for (i32 i = 0; i < 4; i++) {
                    auto dx = dx_list[i], dy = dy_list[i];
                    q.push_back({p.x + dx, p.y + dy, new_size, new_steps});
                    if (p.x + dx < 0 or p.x + dx >= N)  continue;
                    if (p.y + dy < 0 or p.y + dy >= N)  continue;
                    if (dis_to_wall[p.x + dx][p.y + dy] >= new_size) {
                        ends.push_back({p.x, p.y, new_size});
                        continue;
                    }
                }
            }

            reach = vis;  // 被「本体」经过一定可以到达
        }();

        // 从死亡点
        auto walk = [&](i32 x, i32 y, i32 s) {
            struct Node {
                i32 x, y;
                i32 steps;
            };
            std::deque<Node> q{{x, y, s}};

            auto &vis = reach;
            while (not q.empty()) {
                auto [x, y, steps] = q.front();  q.pop_front();
                if (x < 0 or x >= N)  continue;
                if (y < 0 or y >= N)  continue;
                if (vis[x][y])  continue;
                vis[x][y] = true;

                if (s != 0) {
                    std::array<i32, 4> constexpr dx_list = { -1, +1,  0,  0 };
                    std::array<i32, 4> constexpr dy_list = {  0,  0, +1, -1 };

                    for (i32 i = 0; i < 4; i++) {
                        auto dx = dx_list[i], dy = dy_list[i];
                        q.push_back({x + dx, y + dy, s - 1});
                    }
                }
            }
        };

        for (auto x: ends) {
            walk(x.x, x.y, x.size);
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3269067797949153::solve();
    return 0;
}
