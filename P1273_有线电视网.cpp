/**
 * @link https://www.luogu.com.cn/problem/P1273
 */

#include "./libs/debug_macros.hpp"

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1883854020657160 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        struct Node {
            int to, cost;
        };
        std::vector<std::vector<Node>> graph(N+1);
        for (auto i: range(1, N-M+1)) {
            int K;  std::cin >> K;
            for (auto _: range(K)) {
                int A, C;  std::cin >> A >> C;
                graph.at(i).push_back({A, C});
            }
        }
        std::vector<int> users(M);
        for (auto &i: users) {
            std::cin >> i;
        }

        std::vector<int> cnt(N+1); {
            std::function<void(int, int)> dfs_cnt = [&](int p, int prev) {
                for (auto dest: graph.at(p))  if (dest.to != prev) {
                    dfs_cnt(dest.to, p);
                    cnt[p] += cnt[dest.to];
                }
                chkMax(cnt.at(p), 1);
            };
            dfs_cnt(1, 0);
        }

        std::vector F(N+1, std::vector<int>(M+1, -inf));
        std::function<void(int, int)> dfs = [&](int p, int prev) {
            chkMax(F[p][0], 0);
            for (auto dest: graph.at(p))  if (p != prev) {
                dfs(dest.to, p);
                for (auto i: range(0, std::max(M+1, cnt.at(p)+1)) | views::reverse) {
                    for (auto j: range(0, M-i+1)) {
                        if (j > cnt.at(dest.to))  break;
                        chkMax(F[p][i+j], F[p][i] + F[dest.to][j] - dest.cost);
                    }
                }
            }
            if (p > N - M) {
                chkMax(F[p][1], users[p-(N-M)-1]);
            }
        };

        dfs(1, 0);

        auto ans = ranges::max(range(0, M+1) | views::filter(lam(x, F[1][x] >= 0)));
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1883854020657160::solve();
    return 0;
}
