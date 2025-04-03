/**
 * @link https://www.luogu.com.cn/problem/P8280
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/null_number.hpp"
using namespace lib;

/*
发现可以把问题拆分成：一棵子树下有多少种染色方式。
通过染色子树上的节点，可以判断出根节点的颜色。
考虑：记 F[i][j] 表示将 i 节点染色成 j 颜色的方案数。
以二叉树为例：
左子树中任取一个点，右子树中任取一个点，它们的 lca 都是当前的根节点。
可以使用状压记录一种颜色是否存在，然后就可以转移了。
用 F[i][j] 的 j 表示颜色是否存在的二进制状态。
F[i][j | k | common] <- F[lson][j] * F[rson][k] 当且仅当左右子树每一对颜色推导出来的根节点颜色一致。

但是这样对于普通的树根本没法做。
取它的前 son_index 个儿子。考虑和下一个儿子合并信息。合并信息的过程和二叉树类似。
F[i][j][k] 表示节点 i，前 k 个儿子形成的二进制状态为 j，此时的方案数。
F[i][j1 + j2][k] <- F[i][j1][k - 1] * F[son][j2][size]

大概是这样，实际 k 这一维可以直接压缩掉。
*/
namespace Solution_2789635989059777 {
    i32 constexpr mod = 1e9 + 7;
    auto addMod(i32 x, i32 y) -> i32 {
        auto ans = x + y;
        if (ans >= mod) ans -= mod;
        return ans;
    }
    auto mulMod(i32 x, i32 y) -> i32 {
        return static_cast<i32>(static_cast<i64>(x) * y % mod);
    }
    namespace Force {
        auto solve() -> void {
            i32 N, K; std::cin >> N >> K;
            std::vector formula(K + 1, std::vector<i32>(K + 1));
            for (i32 i = 1; i <= K; i++) {
                for (i32 j = 1; j <= K; j++) {
                    std::cin >> formula[i][j];
                }
            }

            std::vector<i32> parent(N + 1);
            std::vector<std::vector<i32>> graph(N + 1);
            for (i32 i = 2; i <= N; i++) {
                std::cin >> parent[i];
                graph[i].push_back(parent[i]);
                graph[parent[i]].push_back(i);
            }

            std::vector<i32> depth(N + 1);
            auto dfs = [&](auto &&self, i32 p, i32 prev) -> void {
                depth[p] = depth[prev] + 1;
                for (auto x: graph[p]) {
                    if (x != prev) {
                        self(self, x, p);
                    }
                }
            };
            dfs(dfs, 1, 0);

            auto lca = [&](i32 u, i32 v) -> i32 {
                if (depth[u] > depth[v]) std::swap(u, v);
                while (depth[v] != depth[u]) v = parent[v];
                while (u != v) u = parent[u], v = parent[v];
                return u;
            };

            std::vector<i32> res{0};
            auto traverse_nums_recursion = [&](auto &&self, i32 n, auto &&f) -> void {
                if (n == 0) return f(res), void();
                for (i32 i = 1; i <= K; i++) {
                    res.push_back(i);
                    self(self, n - 1, f);
                    res.pop_back();
                }
            };

            i32 ans = 0;
            traverse_nums_recursion(traverse_nums_recursion, N, [&](std::vector<i32> const &res) {
                auto correct = [&]() {
                    for (i32 i = 1; i <= N; i++) {
                        for (i32 j = 1; j <= N; j++) {
                            if (i == j) continue;
                            auto a = lca(i, j);
                            if (a == i or a == j) continue;
                            if (res[a] != formula[res[i]][res[j]]) {
                                return false;
                            }
                        }
                    }
                    return true;
                }();
                if (correct) ans++;
            });
            std::cout << ans << endl;
        }
    }
    auto solve() -> void {
        i32 N, K;
        std::cin >> N >> K;

        using Color = i16;
        Color constexpr maxColor = 5;

        std::array<std::array<Color, maxColor>, maxColor> formula{};
        for (Color i = 0; i < K; i++) {
            for (Color j = 0; j < K; j++) {
                std::cin >> formula[i][j];
                formula[i][j]--;  // 颜色为 0-index
            }
        }

        std::vector<i32> parent(N + 1);
        std::vector<std::vector<i32>> graph(N + 1);
        for (i32 i = 2; i <= N; i++) {
            std::cin >> parent[i];
            graph[i].push_back(parent[i]);
            graph[parent[i]].push_back(i);
        }

        using Stat = u16;  // 二进制状态：每个颜色是否取到
        auto max_stat = (Stat)1 << K;

        // 可以合并两个状态，不会发生冲突。
        // 如果是，存储 LCA 的颜色。否则，存储 null。
        std::vector lca_color(max_stat, std::vector<Color>(max_stat));
        for (Stat i = 0; i != max_stat; i++) {
            for (Stat j = 0; j != max_stat; j++) {
                lca_color[i][j] = [&]() -> Color {
                    Color c = null;  // 无效颜色
                    for (Color c0 = 0; c0 != K; c0++) {
                        for (Color c1 = 0; c1 != K; c1++) {
                            if ((i & ((Stat)1 << c0)) and (j & ((Stat)1 << c1))) {
                                if (formula[c0][c1] != formula[c1][c0]) return null;  // 反过来配对即为冲突的
                                // 左侧的 i 和右侧的 j 可以构成一对
                                if (c == null) {  // 还没有确定答案
                                    c = formula[c0][c1];
                                } else if (c != formula[c0][c1]) {
                                    return null;  // 发生冲突
                                }
                            }
                        }
                    }
                    return c;  // 始终没有确定答案，或者答案正确
                }();
            }
        }

        // 一个颜色状态可以和哪些状态配对，不发生冲突
        // 相当于邻接矩阵 -> 临接表
        std::vector<std::vector<Stat>> next_stat(max_stat);
        for (Stat i = 0; i != max_stat; i++) {
            for (Stat j = 0; j != max_stat; j++) {
                if (lca_color[i][j] != null) {
                    next_stat[i].push_back(j);
                }
            }
        }

        std::vector F(N + 1, std::vector<i32>(max_stat));
        auto dfs = [&](auto &&self, i32 p, i32 prev) -> void {
            if ((p != 1 and graph[p].size() <= 1) or (p == 1 and graph[p].size() == 0)) {  // 叶子节点特判
                // 任意选择一个颜色
                for (Color c = 0; c != K; c++) {
                    F[p][(Stat)1 << c] = 1;
                }
                return;
            }

            // G[i][j] 表示当前点前几个子树，颜色存在性的状态为 i，根节点颜色为 j 的方案数
            std::vector<std::array<i32, maxColor>> G(max_stat);
            auto first = true;
            for (auto next: graph[p]) {
                if (next == prev) continue;
                self(self, next, p);

                std::vector<std::array<i32, maxColor>> new_G(max_stat);
                if (first) {
                    for (Stat i = 0; i != max_stat; i++) {
                        for (Color c = 0; c != K; c++) {
                            new_G[i][c] = F[next][i];
                        }
                    }
                } else {  // 正常转移
                    for (Stat left = 0; left != max_stat; left++) {
                        for (Stat right: next_stat[left]) {
                            auto root_c = lca_color[left][right];
                            assert(root_c != null);
    
                            new_G[left | right][root_c] = addMod(
                                new_G[left | right][root_c], 
                                mulMod(G[left][root_c], F[next][right])
                            );
                        }
                    }
                }
                G = new_G;
                first = false;
            }

            for (Stat j = 0; j != max_stat; j++) {
                for (Color c = 0; c != K; c++) {
                    F[p][j | ((Stat)1 << c)] = addMod(F[p][j | ((Stat)1 << c)], G[j][c]);
                }
            }
        };
        dfs(dfs, 1, 0);

        auto ans = std::accumulate(F[1].begin(), F[1].end(), 0, addMod);
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 T; std::cin >> T;
    for (auto t = T; t --> 0; ) {
        Solution_2789635989059777::solve();
    }
    return 0;
}
