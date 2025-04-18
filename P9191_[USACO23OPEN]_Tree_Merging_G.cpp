/**
 * @link https://www.luogu.com.cn/problem/P9191
 */
#include "./lib_v7.hpp"

#include "./libs/fixed_int.hpp"

/*
首先可以观察出哪些节点消失了。
一个消失的节点，一定是合并到了更大的权值。
如果它子树上存在任意一个点还在这个图上，那么就可以确定了。
否则，找到另一个兄弟节点，每一层都可以吞掉当前子树。

令 mergeable[a][b] 表示 a 节点可以合并到 b 上。
于是有如下要求：
    a 节点不在最终状态，而 b 节点在最终状态。
    a < b。
    对于 a 的每个儿子 sa：
        要么存在一个 b 的儿子 mergeable[sa][sb]。
        要么 sa 在最终状态且父亲为 b。

*/
namespace Solution_7166223336083077 {
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<std::set<i32>> graph(n + 1);
        std::vector<i32> parent(n + 1);
        std::vector<i32> depth(n + 1);

        std::vector<char> hasParent(n + 1);
        for (auto _ = n - 1; _ --> 0; ) {
            i32 child, parent;
            std::cin >> child >> parent;
            hasParent[child] = true;
            graph[child].insert(parent);
            graph[parent].insert(child);
        }

        auto root = static_cast<i32>(
            ranges::find(hasParent.begin() + 1, hasParent.end(), false) - hasParent.begin());
        {
            auto dfs = [&](auto &&dfs, i32 u, i32 fa) -> void {
                parent[u] = fa;
                depth[u] = depth[fa] + 1;
                for (auto v: graph[u]) {
                    if (v == fa) continue;
                    dfs(dfs, v, u);
                }
            };
            dfs(dfs, root, 0);
        }

        i32 m;
        std::cin >> m;
        std::vector<std::vector<i32>> newGraph(n + 1);
        std::vector<i32> newParent(n + 1);
        for (auto _ = m - 1; _ --> 0; ) {
            i32 child, parent;
            std::cin >> child >> parent;
            newGraph[child].push_back(parent);
            newGraph[parent].push_back(child);
            newParent[child] = parent;
        }

        std::vector<char> isRemains(n + 1);
        ranges::transform(newGraph, isRemains.begin(), lam(const &x, not x.empty()));

        std::vector<std::pair<i32, i32>> depthAndNodes(n);  // 节点深度和编号
        ranges::transform(views::iota(1, n + 1), depthAndNodes.begin(), [&](i32 node) {
            return std::pair{depth[node], node};
        });
        ranges::sort(depthAndNodes, std::greater{});  // 按照深度降序

        std::vector mergeable(n + 1, std::vector<char>(n + 1));
        for (auto [_, a]: depthAndNodes) {
            if (isRemains[a]) continue;
            for (auto [_, b]: depthAndNodes) {
                if (not isRemains[b]) continue;
                if (depth[a] != depth[b]) continue;
                if (a >= b) continue;

                mergeable[a][b] = true;
                for (auto sa: graph[a]) {
                    if (sa == parent[a]) continue;
                    if (isRemains[sa]) {
                        if (newParent[sa] != b) {
                            mergeable[a][b] = false;
                            break;
                        }
                        continue;
                    }

                    auto allSon = newGraph[b] | views::filter(lam(sb, sb != newParent[b]));
                    bool exists = ranges::any_of(allSon, [&](auto sb) -> bool {
                        if (sb == parent[b]) return false;
                        return isRemains[sb] and mergeable[sa][sb];
                    });
                    if (not exists) {
                        mergeable[a][b] = false;
                        break;
                    }
                }
            }
        }

        debug {
            for (i32 a = 1; a <= n; a++) {
                for (i32 b = 1; b <= n; b++) {
                    if (mergeable[a][b]) {
                        std::cout << "mergeable: " << a << ", " << b << endl;
                    }
                }
            }
        }

        std::vector<std::pair<i32, i32>> ops;
        {
            std::vector<char> vis(n + 1);
            std::deque<i32> q;

            // 合并两个节点，不在 fa[x] 中删除 x。
            auto merge = [&](i32 x, i32 y) -> void {
                assert(x < y);
                ops.push_back({x, y});

                // x 的儿子合并到 y 上
                for (auto v: graph[x]) {
                    if (v == parent[x]) continue;
                    graph[y].insert(v);
                    graph[v].erase(x);
                    graph[v].insert(y);
                    parent[v] = y;
                }
                graph[x].clear();
            };

            q.push_back(root);
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                if (vis[u]) continue;
                vis[u] = true;

                for (auto it = graph[u].begin(); it != graph[u].end(); ) {
                    auto &v = *it;
                    if (vis[v]) {
                        ++it;
                        continue;
                    }
                    auto isMerged = false;

                    for (auto it2 = std::next(it); it2 != graph[u].end(); ++it2) {
                        auto &other = *it2;
                        if (parent[v] != parent[other]) continue;  // 必须父亲一样
                        if (mergeable[v][other]) {
                            // 能够合并，直接进行合并
                            isMerged = true;
                            merge(v, other);
                            it = graph[u].erase(it);
                            break;
                        }
                    }
                    if (not isMerged) {
                        // 正常
                        q.push_back(v);
                        ++it;
                    }
                }
            }
        }

        // assert(static_cast<i32>(ops.size()) == n - m);
        std::cout << ops.size() << endl;
        for (auto [x, y]: ops) {
            std::cout << x << ' ' << y << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 t;
    std::cin >> t;

    for (auto _ = t; _ --> 0; ) {
        Solution_7166223336083077::solve();
    }
    return 0;
}
