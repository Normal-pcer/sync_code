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
        auto get_index_for_chain = lam(chain_index, point_index, (chain_index + 1) * N + point_index);

        // 建图
        i32 graph_nodes_count = (K * 2 + 1) * N;
        struct GraphNode {
            i32 p = 0;
            i32 next = -1;
            bool dis = 0;
        };
        std::vector<GraphNode> graph_edges;
        std::vector<i32> graph_begin(graph_nodes_count, -1);
        auto add_edge = [&](i32 x, i32 y, bool dis) -> void {
            auto cur_begin = graph_begin[x];
            auto new_begin = static_cast<i32>(graph_edges.size());
            graph_edges.push_back({y, cur_begin, dis});
            graph_begin[x] = new_begin;
        };

        // 建图
        [&]() {
            // 一个原始点可以从对应颜色的点走过来，可以走到其他可到达颜色的点。
            // 以及一条链上相邻的需要双向边
            auto edges_size_similar = 3 * K * N;
            graph_edges.reserve(edges_size_similar);

            for (i32 p = 0; p < N; p++) {
                add_edge(get_index_for_chain(a[p], p), get_index_for_origin(p), false);
                for (i32 k = 0; k < K; k++) {
                    if (mat[a[p]][k]) {
                        add_edge(get_index_for_origin(p), get_index_for_chain(k, p), false);
                    }
                }
            }

            for (i32 c = 0; c < K; c++) {
                for (i32 p = 0; p < N - 1; p++) {
                    add_edge(get_index_for_chain(c, p), get_index_for_chain(c, p + 1), true);
                    add_edge(get_index_for_chain(c, p + 1), get_index_for_chain(c, p), true);
                }
            }
        }();
        a.clear(), a.shrink_to_fit();
        graph_edges.shrink_to_fit();
        // 查询最短路
        auto start_node = get_index_for_origin(0);
        auto target_node = get_index_for_origin(N - 1);
        auto ans = [&]() -> i32 {
            // 可以直接优化成 0-1 bfs
            struct Point {
                i32 p = 0, val = 0;
                // std::string s{};
            };
            std::deque<Point> q;
            std::vector<char> vis(graph_nodes_count);
            q.push_back({start_node, 0});
            while (not q.empty()) {
                auto x = q.front(); q.pop_front();
                if (x.p == target_node) return x.val;
                if (vis[x.p]) continue;
                vis[x.p] = true;
                // std::cout << x.s << std::endl;
                for (auto t = graph_begin[x.p]; t != -1; t = graph_edges[t].next) {
                    auto next = graph_edges[t];
                    if (not vis[next.p]) {
                        if (next.dis) q.push_back({next.p, x.val + 1/* , std::format("{} -> {}", x.s, next.p) */});
                        else q.push_front({next.p, x.val/*, std::format("{} -> {}", x.s, next.p) */});
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
    Solution_4625777215395361::solve();
    return 0;
}
