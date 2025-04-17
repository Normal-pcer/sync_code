/**
 * @link https://www.luogu.com.cn/problem/P9189
 */
#include "./lib_v6.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/*
能够满足一个房间 room 的要求，需要此时同时拥有 room.color 和 room.needs 的钥匙。

感觉比较优的方式是把所有的钥匙都收集起来，最后一起分配。
考虑收集。
一个策略是：从起点开始，记录已经拿到的钥匙，对于每条边，如果这个房间可以到达，那么一定向外扩展。
如果这个房间不能到达，那么对于每一个颜色，只要获得了它，就一定可以到达这个房间，然后向外扩展。
最后判断是否可以到达所有房间。
*/
namespace Solution_3564216837703723 {
    struct Room {
        i32 color;
        i32 has;
        i32 needs;
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<Room> rooms(n + 1);
        for (auto &room: rooms | views::drop(1)) std::cin >> room.color;
        for (auto &room: rooms | views::drop(1)) std::cin >> room.has;
        for (auto &room: rooms | views::drop(1)) std::cin >> room.needs;

        std::vector<std::vector<i32>> graph(n + 1);
        for (auto _ = m; _ --> 0; ) {
            i32 x, y;
            std::cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x);
        }

        std::vector<char> isAlone(n + 1);
        [&](i32 s) {
            std::deque<i32> q;
            q.push_back(s);

            std::vector<char> vis(n + 1);
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                if (vis[u]) continue;
                vis[u] = true;

                for (auto v: graph[u]) {
                    if (vis[v]) continue;
                    q.push_back(v);
                }
            }
            ranges::transform(vis, isAlone.begin(), std::logical_not{});
        }(1);

        std::vector<char> isCollected(n + 1);
        auto collect = [&]() -> bool {
            std::set<i32> keys;
            std::deque<i32> q;
            std::vector<std::vector<i32>> waitingColor(n + 1);
            std::vector<char> vis(n + 1);

            q.push_back(1);
            keys.insert(rooms[1].has);
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                if (vis[u]) continue;
                vis[u] = true;
                isCollected[u] = true;

                assert(not isAlone[u]);

                // 收集到一个颜色
                if (auto [it, success] = keys.insert(rooms[u].has); success) {
                    for (auto v: waitingColor[rooms[u].has]) {
                        if (vis[v]) continue;
                        q.push_back(v);
                    }
                    waitingColor[rooms[u].has].clear();
                }

                // 向外扩展
                for (auto v: graph[u]) {
                    if (vis[v]) continue;
                    if (keys.contains(rooms[v].color)) {
                        // 直接进入
                        q.push_back(v);
                    } else {
                        // 等待
                        waitingColor[rooms[u].color].push_back(v);
                    }
                }
            }

            auto ans = ranges::all_of(views::iota(1, n + 1), [&](i32 i) {
                return vis[i] or (rooms[i].needs == rooms[i].color);
            });
            return ans;
        };

        auto allocate = [&]() -> bool {
            std::deque<i32> q;
            std::set<i32> keys;
            std::vector<std::vector<i32>> waitingColor(n + 1);
            
            q.push_back(1);
            keys.insert(rooms[1].needs);

            std::vector<char> vis(n + 1);
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                if (vis[u]) continue;
                vis[u] = true;

                if (auto [it, success] = keys.insert(rooms[u].needs); success) {
                    for (auto v: waitingColor[rooms[u].needs]) {
                        if (vis[v]) continue;
                        q.push_back(u);
                    }
                    waitingColor[rooms[u].needs].clear();
                }

                for (auto v: graph[u]) {
                    if (vis[v]) continue;
                    if (not isCollected[v]) continue;
                    if (rooms[v].color == rooms[v].needs or keys.contains(rooms[v].color)) {
                        if (vis[v]) continue;
                        q.push_back(v);
                    } else {
                        waitingColor[rooms[u].color].push_back(u);
                    }
                }
            }

            auto ans = ranges::all_of(views::iota(1, n + 1), [&](i32 i) {
                return vis[i] or (rooms[i].has == rooms[i].color);
            });
            return ans;
        };

        auto ans = collect() and allocate();
        if (ans) std::cout << "YES" << endl;
        else std::cout << "NO" << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::cin.exceptions(std::ios::badbit | std::ios::eofbit | std::ios::failbit);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 t;
    std::cin >> t;

    for (auto _ = t; _ --> 0; ) {
        Solution_3564216837703723::solve();
    }
    return 0;
}
