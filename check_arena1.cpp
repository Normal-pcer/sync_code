/**
 * @link https://www.luogu.com.cn/problem/P7297
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 考虑将所有相同品种的奶牛放在一条链上，例如：
 * c = 1:  1, 2, 3, 4, 5
 *        a1
 * c = 2:  1, 2, 3, 4, 5
 *              a3
 * c = 3:  1, 2, 3, 4, 5
 *                 a4
 * c = 4:  1, 2, 3, 4, 5
 *           a2       a5
 * 
 * 一条链的相邻两个点之间会有双向连边，边权为 1。
 * 不同链之间还可以有边权为 0 的连边。
 * 从颜色 4 连接到 2，便可以：
 * 
 *           a2       a5
 * c = 4:  1, 2, 3, 4, 5
 *         |  |  |  |  |
 * c = 2:  1, 2, 3, 4, 5
 *              a3
 * 
 * 这样有一个问题是自己不一定能连向自己。可以把每个颜色的链拆分成出和入两种。
 * 似乎只有目标颜色和奶牛颜色相同的时候才能连中间这个边，不然会乱转移。
 */
namespace Solution_4625777215395361 {
    auto solve_force() -> void {
        i32 N, K;  std::cin >> N >> K;
        std::vector<i32> a(N);
        for (auto &x: a)  std::cin >> x, x--;  // 颜色使用 0-index

        std::vector mat(K, std::vector<char>(K));
        for (auto &line: mat) {
            for (auto &x: line) {
                char tmp;  std::cin >> tmp;
                x = (tmp == '1');
            }
        }
        
        struct GraphNode {
            i32 p = 0, dis = 0;
        };
        std::vector<std::vector<GraphNode>> graph(N);
        for (i32 i = 0; i != N; i++) {
            for (i32 j = 0; j != N; j++) {
                if (i == j)  continue;
                if (mat[a[i]][a[j]]) {
                    graph[i].push_back({j, std::abs(i - j)});
                }
            }
        }

        auto ans = [&]() -> i32 {
            struct Node {
                i32 p = 0, val = 0;

                // 反向三路比较
                auto operator<=> (Node const &other) const -> std::weak_ordering {
                    return other.val <=> val;
                }
                auto operator== (Node const &other) const -> bool = default;
            };

            std::vector<char> vis(N);
            std::priority_queue<Node> q;
            q.push({0, 0});
            while (not q.empty()) {
                auto x = q.top(); q.pop();
                if (x.p == N - 1) return x.val;
                if (vis[x.p]) continue;
                vis[x.p] = true;
                for (auto next: graph[x.p]) {
                    if (not vis[next.p]) {
                        q.push({next.p, x.val + next.dis});
                    }
                }
            }
            return -1;
        }();

        std::cout << ans << endl;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, K;  std::cin >> N >> K;
        std::vector<i32> a(N);
        for (auto &x: a)  std::cin >> x, x--;  // 颜色使用 0-index

        std::vector mat(K, std::vector<char>(K));
        i32 mat_popcount = 0;
        for (auto &line: mat) {
            for (auto &x: line) {
                char tmp;  std::cin >> tmp;
                x = (tmp == '1');
                mat_popcount += (tmp == '1');
            }
        }

        // 定义节点在图上的编号
        // 为原始存在的点定义点编号
        auto get_index_for_origin = lam(x, x);

        // 为链上的点定义点编号
        // 链编号：k * 2 为出链，k * 2 + 1 为入链
        auto get_out_chain = lam(x, x * 2);
        auto get_in_chain = lam(x, x * 2 + 1);
        auto get_index_for_chain = lam(chain_index, point_index, (chain_index + 1) * N + point_index);

        std::vector<std::tuple<i32, i32, bool>> edges;  // (p0, p1, 边权)
        [&]() {
            // 边比较多。让 vector 预留一定的空间。
            // 预估的边数
            auto edge_count_similar = (4 * K * N) + (mat_popcount * 2 * N) + (K * N) + (2 * N);
            edges.reserve(edge_count_similar);

            // 每条链上相邻点连边
            for (i32 c = 0; c < K + K; c++) {
                for (i32 i = 0; i < N - 1; i++) {
                    auto p0 = get_index_for_chain(c, i);
                    auto p1 = get_index_for_chain(c, i + 1);
                    edges.push_back({p0, p1, true});
                    edges.push_back({p1, p0, true});
                }
            }

            // 互相可达的链，对应点连满双向边
            for (i32 i = 0; i < K; i++) {
                for (i32 j = 0; j < K; j++) {
                    if (not mat[i][j]) continue;
                    for (i32 p = 0; p < N; p++) {
                        auto p0 = get_index_for_chain(get_out_chain(i), p);
                        auto p1 = get_index_for_chain(get_in_chain(j), p);
                        if (a[p] == j) edges.push_back({p0, p1, false});
                        if (a[p] == i) edges.push_back({p1, p0, false});
                    }
                }
            }

            // 一入一出一定能连满单向边
            never for (i32 i = 0; i < K; i++) {
                for (i32 p = 0; p < N; p++) {
                    auto p0 = get_index_for_chain(get_in_chain(i), p);
                    auto p1 = get_index_for_chain(get_out_chain(i), p);
                    edges.push_back({p0, p1, false});
                }
            }

            // 每个点连到自己所在的链上
            for (i32 i = 0; i < N; i++) {
                auto in_c = get_in_chain(a[i]);
                auto out_c = get_out_chain(a[i]);

                edges.push_back({get_index_for_origin(i), get_index_for_chain(out_c, i), false});
                edges.push_back({get_index_for_chain(in_c, i), get_index_for_origin(i), false});
            }
        }();
        debug {
            std::cout << "edges:" << std::endl;
            for (auto &[x, y, val]: edges) {
                std::cout << x << " " << y << " " << val << std::endl;
            }

            for (i32 k = 0; k < K; k++) {
                std::cout << "in" << k << ": " << get_index_for_chain(get_in_chain(k), 0) << std::endl;
                std::cout << "out" << k << ": " << get_index_for_chain(get_out_chain(k), 0) << std::endl;
            }
        }

        // 建图
        i32 graph_nodes_count = (K * 2 + 1) * N;
        struct GraphNode {
            i32 p = 0;
            bool dis = 0;
        };
        std::vector<std::vector<GraphNode>> graph(graph_nodes_count);

        for (auto &[x, y, val]: edges) {
            graph[x].push_back({y, val});
        }

        // 查询最短路
        auto start_node = get_index_for_origin(0);
        auto target_node = get_index_for_origin(N - 1);
        auto ans = [&]() -> i32 {
            // 可以直接优化成 0-1 bfs
            struct Point {
                i32 p = 0, val = 0;
            };
            std::deque<Point> q;
            std::vector<char> vis(graph_nodes_count);
            q.push_back({start_node, 0});
            while (not q.empty()) {
                auto x = q.front(); q.pop_front();
                if (x.p == target_node) return x.val;
                if (vis[x.p]) continue;
                vis[x.p] = true;
                for (auto next: graph[x.p]) {
                    if (not vis[next.p]) {
                        if (next.dis) q.push_back({next.p, x.val + 1});
                        else q.push_front({next.p, x.val});
                    }
                }
            }
            return -1;
        }();

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4625777215395361::solve_force();
    return 0;
}
