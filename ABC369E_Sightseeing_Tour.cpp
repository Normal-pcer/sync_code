/**
 * 
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9555353784332920 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;
        std::cin >> N >> M;
        std::vector<std::vector<int>> graph(N+1, std::vector<int>(N+1, inf));
        std::vector<std::tuple<int, int, int>> edges{{0, 0, 0}};
        edges.reserve(M+1);
        for (auto _: range(1, M+1)) {
            int x, y, val;  std::cin >> x >> y >> val;
            edges.push_back({x, y, val});
            chkMin(graph[x][y], val), chkMin(graph[y][x], val);
        }

        // i 到 j 最短距离为 F[i][j]
        std::vector<std::vector<ll>> F(N+1, std::vector<ll>(N+1, infLL));
        for (auto i: range(1, N+1)) {
            for (auto j: range(1, N+1)) {
                if (i == j)  F[i][j] = 0;
                if (graph[i][j] > 0x3ccccccc)  continue;
                chkMin(F[i][j], (ll)graph[i][j]);
            }
        }
        for (auto k: range(1, N+1)) {
            for (auto i: range(1, N+1)) {
                for (auto j: range(1, N+1)) {
                    chkMin(F[i][j], F[i][k] + F[k][j]);
                }
            }
        }

        int Q;  std::cin >> Q;
        for (auto _: range(Q)) {
            int K;  std::cin >> K;
            std::vector<int> require(K);
            for (auto &i: require)  std::cin >> i;

            // 接下来，对于每一个必选项，枚举它的方向
            std::vector<std::tuple<int, int, int>> res;
            std::function<ll(int)> dfs = [&](int p) {
                if (p >= K) {
                    std::vector<std::tuple<int, int, int>> res_cpy(res);
                    std::sort(res_cpy.begin(), res_cpy.end());
                    auto ans_tot = infLL;
                    do {
                        auto prev = 1;  // 上一次驻留的节点
                        auto ans = 0LL;

                        for (auto [x, y, val]: res) {
                            ans += F[prev][x];
                            ans += val;
                            prev = y;
                        }

                        ans += F[prev][N];
                        chkMin(ans_tot, ans);
                    } while (std::next_permutation(res_cpy.begin(), res_cpy.end()));
                    return ans_tot;
                }
                auto [x, y, val] = edges[require[p]];
                auto ans = infLL;
                res.push_back({x, y, val}), chkMin(ans, dfs(p+1));
                res.pop_back();
                res.push_back({y, x, val}), chkMin(ans, dfs(p+1));
                res.pop_back();
                return ans;
            };

            std::cout << dfs(0) << std::endl;
        }
    }
}

int main() {
    initDebug;
    Solution_9555353784332920::solve();
    return 0;
}