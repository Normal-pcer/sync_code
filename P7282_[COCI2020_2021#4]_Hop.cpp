/**
 * @link https://www.luogu.com.cn/problem/P7282
 * @date 2025/01/20
 * rks 涨了。
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 考虑：对于任意两个可能产生联系的点（即 j > i 且 j % i == 0）连边。
 * 给边染色（最多三种颜色），希望同色的最长路径不超过 3 个节点。
 * 在染色前，最长的一条路径一定不会长于 60（1, 2, 4, 8, ..., 2**59）。
 * 
 */
namespace Solution_1024261437645003 {
    struct Graph {
        struct Forward {
            int next, val;
        };
        int N;
        std::vector<std::vector<Forward>> graph;
        std::vector<char> vis;

        Graph(int N): N(N), graph(N+1), vis(N+1) {}
        auto dfs(int p, int depth) -> void {
            vis[p] = true;
            for (auto &[x, c]: graph.at(p))  if (not vis[x]) {
                dfs(x, depth + 1);
                c = (depth % 3) + 1;
                std::cout << x << " " << p << " " << c << std::endl;
            }
        }
        // 染色
        auto color() -> void {
            for (auto i: range(1, N+1)) {
                if (not vis[i]) {
                    dfs(i, 0);
                }
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        
        int N;  std::cin >> N;
        std::vector<ll> val(N+1);  // 读入开 long long
        for (auto &x: val | views::drop(1))  std::cin >> x;

        std::vector<std::pair<int, int>> edges;  // 边
        for (auto i: range(1, N+1)) {
            for (auto j: range(i+1, N+1)) {
                auto x = val[i], y = val[j];
                if (x < y)  std::swap(x, y);
                if (x % y == 0)  edges.push_back({i, j});
            }
        }
        Graph g{N};
        for (auto [x, y]: edges) {
            g.graph.at(x).push_back({y, 0});
            g.graph.at(y).push_back({x, 0});
        }

        g.color();
        std::vector c(N+1, std::vector<int>(N+1));
        for (auto i: range(1, N+1)) {
            for (auto [x, val]: g.graph[i]) {
                if (val == 0)  continue;
                std::cout << x << " " << i << " " << val << std::endl;
                c[i][x] = c[x][i] = val;
            }
        }
        for (auto i: range(1, N)) {
            for (auto j: range(1, i+1)) {
                // std::cout << j << i+1 << c[j][i+1] << " ";
                if (c[j][i+1] == 0)  std::cout << 1 << " ";
                else  std::cout << c[j][i+1] << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main(int dyy_forever, char const *swy_forever[]) {
    DEBUG_MODE = (dyy_forever - 1) and not strcmp("-d", swy_forever[1]);
    Solution_1024261437645003::solve();
    return 0;
}
