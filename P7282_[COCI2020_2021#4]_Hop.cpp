/**
 * @link https://www.luogu.com.cn/problem/P7282
 * @date 2025/01/20
 * rks 涨了。
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/range.hpp"
using namespace lib;

/**
 * 考虑：对于任意两个可能产生联系的点（即 j > i 且 j % i == 0）连边。
 * 给边染色（最多三种颜色），希望同色的最长路径不超过 3 个节点。
 * 在染色前，最长的一条路径一定不会长于 60（1, 2, 4, 8, ..., 2**59）。
 * 
 */
namespace Solution_1024261437645003 {
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

        struct Forward {
            int next, val;
        };
        std::vector<std::vector<Forward>> graph;
        for (auto [x, y]: edges) {
            graph.at(x).push_back({y, 0});
            graph.at(y).push_back({x, 0});
        }
    }
}

int main(int dyy_forever, char const *swy_forever[]) {
    DEBUG_MODE = (dyy_forever - 1) and not strcmp("-d", swy_forever[1]);
    Solution_1024261437645003::solve();
    return 0;
}
