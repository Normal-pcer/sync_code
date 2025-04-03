#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define __lambda_1(expr) [&]() { return expr; }
#define __lambda_2(a, expr) [&](auto a) { return expr; }
#define __lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define __lambda_overload(a, b, c, d, e, ...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
char endl = '\n';
#define FILENAME "fire"

typedef int32_t i32;

namespace Solution_6065476225470371 {
    auto solve() -> void {
        i32 r, c;
        std::cin >> r >> c;

        std::vector<std::string> mat(r);

        struct Point {
            i32 x{};
            i32 y{};

            Point() {}
            Point(i32 x, i32 y): x(x), y(y) {}
        };

        Point s{};
        for (auto &line: mat) {
            std::cin >> line;
        }

        std::vector<Point> fire;
        for (i32 i = 0; i < r; i++) {
            for (i32 j = 0; j < c; j++) {
                if (mat[i][j] == 'F') {
                    fire.emplace_back(i, j);
                } else if (mat[i][j] == 'J') {
                    s.x = i, s.y = j;
                }
            }
        }

        i32 const inf = 0x3f3f3f3f;
        // 距离最近的火源
        std::vector<std::vector<i32>> dis(r, std::vector<i32>(c, inf));

        std::array<i32, 4> dxs{ -1, +1,  0,  0 };
        std::array<i32, 4> dys{  0,  0, -1, +1 };
        [&]() {  // 最短路
            struct Node {
                Point p{};
                i32 dis{};

                Node() {}
                Node(Point p, i32 dis): p(p), dis(dis) {}
            };
            std::deque<Node> q;

            for (auto f: fire) q.emplace_back(f, 0);
            std::vector<std::vector<char>> vis(r, std::vector<char>(c));
            while (not q.empty()) {
                auto x = q.front(); q.pop_front();
                if (vis[x.p.x][x.p.y]) continue;
                if (mat[x.p.x][x.p.y] == '#') continue;
                vis[x.p.x][x.p.y] = true;
                dis[x.p.x][x.p.y] = x.dis;
                for (i32 i = 0; i < 4; i++) {
                    auto nx = x.p.x + dxs[i];
                    auto ny = x.p.y + dys[i];
                    if (nx < 0 or nx >= r) continue;
                    if (ny < 0 or ny >= c) continue;
                    if (not vis[nx][ny]) {
                        q.emplace_back(Point(nx, ny), x.dis + 1);
                    }
                }
            }
        }();

        // 初始时间为 0
        // 不能经过距离火源小于等于当前时间的点
        auto ans = [&]() {
            struct Node {
                Point p{};
                i32 dis{};

                Node() {}
                Node(Point p, i32 dis): p(p), dis(dis) {}
            };
            std::deque<Node> q;
            std::vector<std::vector<char>> vis(r, std::vector<char>(c));

            q.emplace_back(s, 0);
            while (not q.empty()) {
                auto x = q.front(); q.pop_front();
                if (vis[x.p.x][x.p.y]) continue;
                if (mat[x.p.x][x.p.y] == '#') continue;
                if (dis[x.p.x][x.p.y] <= x.dis) continue;
                vis[x.p.x][x.p.y] = true;
                for (i32 i = 0; i < 4; i++) {
                    auto nx = x.p.x + dxs[i];
                    auto ny = x.p.y + dys[i];
                    if (nx < 0 or nx >= r) return x.dis + 1;
                    if (ny < 0 or ny >= c) return x.dis + 1;
                    if (not vis[nx][ny]) {
                        q.emplace_back(Point{nx, ny}, x.dis + 1);
                    }
                }
            }
            return -1;
        }();
        if (ans == -1) std::cout << "IMPOSSIBLE" << endl;
        else std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    // std::freopen(FILENAME ".in", "r", stdin);
    // std::freopen(FILENAME ".out", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    i32 T;
    std::cin >> T;
    while (T --> 0) {
        Solution_6065476225470371::solve();
    }
    return 0;
}
