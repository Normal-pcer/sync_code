/**
 * @link https://www.luogu.com.cn/problem/P3177
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

#include "./libs/debug_log.hpp"

using namespace lib;

namespace Solution_4613954895447522 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        K = std::min(K, N - K);
        struct Node { int to, val; };
        std::vector<std::vector<Node>> graph(N+1);
        for (auto _: range(N-1)) {
            int x, y, val;  std::cin >> x >> y >> val;
            graph.at(x).push_back({y, val});
            graph.at(y).push_back({x, val});
        }
        std::vector F(N+1, std::vector<ll>(K+1));
        std::vector<int> size(N+1);
        std::function<void(int, int)> dfs = [&](int p, int prev) {
            F[p][0] = 0, size[p] = 1;
            for (auto dest: graph.at(p))  if (dest.to != prev) {
                const auto x = dest.to, len = dest.val;
                dfs(x, p);

                // std::vector Fp_out(F[p].begin(), F[p].end());
                auto &Fp_out = F[p];
                size[p] += size[x];
                for (auto i: range(std::min(size[p], K) + 1) | views::reverse) {
                    for (auto j: range(std::max(i - size[p] + size[x], 0), std::min(i, size[x]) + 1)) {
                        auto cnt = (ll)j * (K - j) + (ll)(size[x] - j) * (N - K - size[x] + j);
                        debug  printValues(cnt);
                        chkMax(Fp_out[i], F[p][i-j] + F[x][j] + len * cnt);
                        debug  printValues(p, prev, dest.to, dest.val, i, j, Fp_out[i], F[p][i-j] + F[x][j] + len * cnt);
                    }
                }
                // ranges::copy(Fp_out, F[p].begin());
            }
        };
        dfs(1, 0);
        for (auto i: range(N+1)) {
            for (auto j: range(K+1)) {
                debug  printValues(i, j, F[i][j]);
            }
        }
        auto ans = F[1][K];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4613954895447522::solve();
    return 0;
}
