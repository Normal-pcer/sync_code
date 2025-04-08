/**
 * @link https://www.luogu.com.cn/problem/P4284
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
只需要求出每个点被充电的概率。
对一个节点的贡献，或许可以简单地分成子树内、子树外和自己三个部分。

每个点被充电的概率看起来转移有点奇怪。考虑不被充电的概率。
考虑子树内的贡献。一个节点不被充电，需要每一条向孩子的连边满足：
    要么节点没有充电，要么中间的连边没有开启。

令 F[i] 表示：i 点没有被子树中的点或自己点亮的概率。
转移：
    初始 F[u] = 1 - u.prob
    对于 u 向孩子的连边 edge：
        令 p1 = F[edges.to]
        p2 = 1 - edge.prob
        F[u] *= (p1 + p2 - p1*p2)

G[i] 表示：i 点没有被子树外的点点亮的概率。
然后可以发现 G[i] 其实没法算。
考虑用类似换根的方式来更新 F[i]。

把根节点从 u 转到 v（连边为 edge），观察 F[i] 的变化量：
    首先，从 u 的子树中剔除 v 的子树：
        p1 = F[v]
        p2 = 1 - edge.prob
        new_F[u] <- F[u] / (p1 + p2 - p1*p2)
    接下来，u 的小子树作为孩子连到 v 上：
        p1 = new_F[u]  # 只统计小子树上的
        p2 = 1 - edge.prob
        new_F[v] <- F[v] * (p1 + p2 - p1*p2)

在换根的过程中，通过参数传递实际的 F[v] 即可。
*/
namespace Solution_2673123382433204 {
    using f64 = double;
    auto solveForce() -> void {
        i32 n; std::cin >> n;

        struct GraphNode {
            i32 to;
            double prob;
        };
        std::vector<std::tuple<i32, i32, f64>> edges;
        
        for (auto _ = n - 1; _ --> 0; ) {
            i32 a, b; f64 p;
            std::cin >> a >> b >> p;
            p /= 100;
            edges.push_back({a, b, p});
        }

        std::vector<f64> probPoint(n + 1);
        for (i32 i = 1; i <= n; i++) {
            std::cin >> probPoint[i];
            probPoint[i] /= 100;
        }

        i32 constexpr maxIter = 1e5;
        i64 ans = 0;
        for (auto _ = maxIter; _ --> 0; ) {
            std::vector<char> open(n + 1);
            std::mt19937 rng{std::random_device{}()};
            auto rand = lam((std::uniform_real_distribution<f64>{0, 1}(rng)));
            for (i32 p = 1; p <= n; p++) {
                open[p] = rand() < probPoint[p];
            }
            
            std::vector<std::vector<GraphNode>> graph(n + 1);
            for (auto [a, b, p]: edges) {
                if (rand() < p) {
                    graph[a].push_back({b, p});
                    graph[b].push_back({a, p});
                }
            }
            std::deque<i32> q;
            for (i32 p = 1; p <= n; p++) {
                if (open[p]) {
                    q.push_back(p);
                }
            }
            std::vector<char> vis(n + 1);
            while (not q.empty()) {
                auto x = q.front(); q.pop_front();
                if (vis[x]) continue;
                vis[x] = true;
                open[x] = true;

                for (auto e: graph[x]) {
                    if (vis[e.to]) continue;
                    q.push_back(e.to);
                }
            }

            ans += ranges::count(open, true);
        }
        std::cout << std::fixed << std::setprecision(6) << static_cast<f64>(ans) / maxIter << endl;
    }
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        struct GraphNode {
            i32 to;
            f64 prob;
        };
        std::vector<std::vector<GraphNode>> graph(n + 1);

        for (auto _ = n - 1; _ --> 0; ) {
            i32 x, y, percent;
            std::cin >> x >> y >> percent;
            auto prob = static_cast<f64>(percent) / 100;
            graph[x].push_back({y, prob});
            graph[y].push_back({x, prob});
        }

        std::vector<f64> probOf(n + 1);
        for (i32 i = 1; i <= n; i++) {
            i32 percent;
            std::cin >> percent;
            probOf[i] = static_cast<f64>(percent) / 100;
        }

        std::vector<f64> F(n + 1);
        auto getInitF = [&](i32 u, i32 prev, auto &&getInitF) -> void {
            F[u] = 1 - probOf[u];
            for (auto edge: graph[u]) {
                if (edge.to == prev) continue;
                getInitF(edge.to, u, getInitF);

                auto p1 = F[edge.to];
                auto p2 = 1 - edge.prob;
                F[u] *= (p1 + p2 - p1 * p2);
            }
        };

        getInitF(1, 0, getInitF);

        std::vector<f64> G(n + 1);
        // 当前根节点在 u，进行换根
        auto modifyRoot = [&](i32 u, i32 prev, f64 F_u, auto &&modifyRoot) -> void {
            for (auto edge: graph[u]) {
                if (edge.to == prev) continue;

                auto new_F_u = [&] -> f64 {
                    auto p1 = F[v];
                    auto p2 = 1 - edge.prob;
                    return F[u] / (p1 + p2 - p1 * p2);
                };
                auto new_F_v = [&] -> f64 {
                    auto p1 = new_F_u;
                    auto p2 = 1 - edge.prob;
                    return F[v] * (p1 + p2 - p1 * p1);
                };

                modifyRoot(edge.to, u, new_F_v, modifyRoot);
            }
        };
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("d", argv[1]) == 0);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_2673123382433204::solveForce();
    return 0;
}
