/**
 * @link https://www.luogu.com.cn/problem/P10232
 */
#ifndef ONLINE_JUDGE
#define GNU_DEBUG
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
using namespace lib;

/**
 * 对于每一个特殊点，把“从一个方向进入这个点”作为一个有向图的节点
 * 两个点之间的最短路就是最小转弯次数
 * 
 * 这个图的每一个点都只有一条出边和一条入边，所以整个图是若干个不连通的环
 * 查询两个点的最短路，只需要看这二者是否在同一个环上
 * 如果在同一个环上，把环上节点编号，可以 O(1) 求解二者的距离。
 * 否则，无解。
 * 
 * 可以先建图，对于编号为 i 的特殊点，[4*i, 4*i+3] 这四个点表示四个方向走到该点
 * 分别存储每一行上的所有特殊点（排序），二分查找即可在对数复杂度下找到左右的点
 * 上下的点同理。
 * 
 * 查询答案：从起点和终点向四个方向延伸，共 16 种情况。
 * 分别查询取最小值即可。
 */
namespace Solution_8670256859201745 {
    enum Direction {Right, Up, Left, Down};
    struct SpecialPoint {
        int x, y;
        enum Type {Left, Right} type = Left;
        int index = 0;
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        std::vector<SpecialPoint> sp_points;
        for (auto i = 0; i < K; i++) {
            int x, y;  std::cin >> x >> y;
            char t;  std::cin >> t;
            x--, y--;  // 行号和列号从 0 开始
            auto Type = (t == 'L'? SpecialPoint::Left: SpecialPoint::Right);
            sp_points.push_back({x, y, Type, i});
        }

        std::vector<std::vector<SpecialPoint>> rows(N);
        std::vector<std::vector<SpecialPoint>> cols(M);
        for (auto const &p: sp_points) {
            rows[p.x].push_back(p);
            cols[p.y].push_back(p);
        }

        for (auto &row: rows) {
            ranges::sort(row, lam(x, y, x.y < y.y));
            auto size = row.size();
            row.resize(size * 2);
            for (auto i = 0U; i < size; i++) {
                row[i + size] = row[i];
                row[i + size].y += M;
            }
        }
        for (auto &col: cols) {
            ranges::sort(col, lam(x, y, x.x < y.x));
            auto size = col.size();
            col.resize(size * 2);
            for (auto i = 0U; i < size; i++) {
                col[i + size] = col[i];
                col[i + size].x += N;
            }
        }

        // 给每一个特殊点拆成四个节点
        auto node_count = 4 * K;
        std::vector<std::vector<int>> graph(node_count);

        // 向四个方向能够触及的点执行操作
        // 操作为一个函数，参数列表：
        // 当前节点编号，下一个节点编号，这次的方向
        auto four_dir = [&](SpecialPoint const &sp, auto &&func) -> void {
            // 寻找四个方向上的节点
            auto right = ranges::upper_bound(rows[sp.x], sp, lam(x, y, x.y < y.y));
            if (right != rows[sp.x].end())  func(sp.index, right->index, Direction::Right);
            auto left = ranges::lower_bound(rows[sp.x], SpecialPoint{sp.x, sp.y + M, sp.type, sp.index}, lam(x, y, x.y < y.y));
            if (left != rows[sp.x].begin())  func(sp.index, std::prev(left)->index, Direction::Left);
            auto down = ranges::upper_bound(cols[sp.y], sp, lam(x, y, x.x < y.x));
            if (down != cols[sp.y].end())  func(sp.index, down->index, Direction::Down);
            auto up = ranges::lower_bound(cols[sp.y], SpecialPoint{sp.x + N, sp.y, sp.type, sp.index}, lam(x, y, x.x < y.x));
            if (up != cols[sp.y].begin())  func(sp.index, std::prev(up)->index, Direction::Up);
        };
        for (auto i = 0; i < K; i++) {
            auto const &sp = sp_points[i];
            auto add_edge = [&](int x, int y, Direction y_dir) -> void {
                auto x_dir = [&]() -> Direction {
                    switch (y_dir) {
                    case Direction::Right:  return Direction::Up;
                    case Direction::Up:  return Direction::Left;
                    case Direction::Left:  return Direction::Down;
                    case Direction::Down:  return Direction::Right;
                    default:  assert(false), __builtin_unreachable();
                    }
                }();

                if (sp.type == SpecialPoint::Left)  x_dir = static_cast<Direction>(static_cast<int>(x_dir) ^ 2);
                auto x_node = 4 * x + static_cast<int>(x_dir);
                auto y_node = 4 * y + static_cast<int>(y_dir);

                graph[x_node].push_back(y_node);
            };
            four_dir(sp, add_edge);
        }

        // 在这个图上找环
        // 从某个节点开始向下延伸即可
        int next_ring = 0;
        struct RingPos { int ring, pos; };

        std::vector<RingPos> pos(node_count);
        std::vector<char> vis(node_count);
        std::vector<int> ring_size;

        auto dfs = [&](auto &&self, int p, int ring, int this_pos) -> void {
            if (vis[p])  return;
            vis[p] = true;
            ring_size[ring]++;
            pos[p] = {ring, this_pos};
            for (auto x: graph[p])  self(self, x, ring, this_pos + 1);
        };
        for (auto i = 0; i < node_count; i++) {
            if (vis[i])  continue;
            ring_size.push_back(0);
            dfs(dfs, i, next_ring++, 0);
        }

        auto query_nodes = [&](int x, int y) -> int {
            auto [xr, xp] = pos[x];
            auto [yr, yp] = pos[y];

            if (xr != yr)  return inf;
            auto dis = yp - xp;
            if (dis < 0)  dis += ring_size[xr];
            return dis;
        };

        int Q;  std::cin >> Q;
        while (Q --> 0) {
            int x1, y1, x2, y2;
            std::cin >> x1 >> y1 >> x2 >> y2;
            x1--, y1--, x2--, y2--;

            // 起点和终点分别向四个方向延伸
            // 获得起点方向之后
            auto cur_ans = inf;
            auto start_callback = [&](int, int start_to, Direction start_dir) -> void {
                four_dir(SpecialPoint{x2, y2, SpecialPoint::Left, 0}, [&](int, int end_from, Direction end_dir) -> void {
                    // 路线即为 起点 -> start_to ->（某个环上）-> end_from -> 终点
                    auto dir1 = start_dir;
                    // 计算第二次的方向
                    // 如果 end_from 为 L，就是 end_dir 顺时针旋转 pi/2
                    auto dir2 = static_cast<Direction>((static_cast<int>(end_dir) + 1) % 4);
                    if (sp_points[end_from].type == SpecialPoint::Right) {
                        dir2 = static_cast<Direction>((static_cast<int>(end_dir) ^ 2) % 4);
                    }

                    auto node1 = 4 * start_to + static_cast<int>(dir1);
                    auto node2 = 4 * end_from + static_cast<int>(dir2);
                    auto dis = query_nodes(node1, node2) + 1;
                    chkMin(cur_ans, dis);
                });
            };

            // 先检查是否可以零次转弯
            auto zero_turn = false;
            if (x1 == x2) {
                auto y1_ = y1, y2_ = y2;
                if (y1_ > y2_)  std::swap(y1_, y2_);
                // 检查 y1, y2 之间是否有特殊点（不包含两点）
                auto y_min = y1_ + 1, y_max = y2_ - 1;
                auto range_begin = ranges::lower_bound(rows[x1], SpecialPoint{x1, y_min}, lam(x, y, x.y < y.y));
                auto range_end = ranges::upper_bound(rows[x1], SpecialPoint{x1, y_max}, lam(x, y, x.y < y.y));
                if (range_begin == range_end)  zero_turn = true;
            } else if (y1 == y2) {
                auto x1_ = x1, x2_ = x2;
                if (x1_ > x2_)  std::swap(x1_, x2_);

                auto x_min = x1_ + 1, x_max = x2_ - 1;
                auto range_begin = ranges::lower_bound(cols[y1], SpecialPoint{x_min, y1}, lam(x, y, x.x < y.x));
                auto range_end = ranges::upper_bound(cols[y1], SpecialPoint{x_max, y1}, lam(x, y, x.x < y.x));
                if (range_begin == range_end)  zero_turn = true;
            }
            if (zero_turn)  cur_ans = 0;
            else  four_dir(SpecialPoint{x1, y1, SpecialPoint::Left, 0}, start_callback);

            if (cur_ans == inf)  std::cout << -1 << std::endl;
            else  std::cout << cur_ans << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8670256859201745::solve();
    return 0;
}
