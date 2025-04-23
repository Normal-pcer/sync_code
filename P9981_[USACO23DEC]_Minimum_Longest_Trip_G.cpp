/**
 * @link https://www.luogu.com.cn/problem/P9981
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
f[i] 表示从一个点出发的最长路，按照拓扑序转移即可。

考虑如何保证字典序最小。
首先的思路是根据先前的信息暴力提取序列，然后比较字典序。
使用倍增哈希，判断首个不一样的地方并比较。不能使用二分哈希，因为它需要始终维护一个前缀信息。
*/
namespace Solution_8987280464595201 {
    struct Graph {
        struct Node {
            i32 to, label;
        };
        i32 n;
        std::vector<std::vector<Node>> graph;
        std::vector<i32> inDeg;

        Graph(i32 n, std::vector<std::array<i32, 3>> const &edges): n(n), graph(n + 1), inDeg(n + 1) {
            for (auto &[u, v, label]: edges) {
                graph[u].push_back({v, label});
                inDeg[v]++;
            }
        }

        auto topoSort() const -> std::optional<std::vector<i32>> {
            auto in = inDeg;

            std::deque<i32> q;
            for (i32 i = 1; i <= n; i++) {
                if (in[i] == 0) q.push_back(i);
            }
            
            std::vector<i32> order;
            order.reserve(n);
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                order.push_back(u);

                for (auto [v, _]: graph[u]) {
                    in[v]--;
                    if (in[v] == 0) {
                        q.push_back(v);
                    }
                }
            }

            if (n == static_cast<i32>(order.size())) {
                return order;
            } else {
                return std::nullopt;
            }
        }
    };

    u64 constexpr hashBase = 99'824'453;
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<std::array<i32, 3>> edges(m);
        for (auto &[u, v, label]: edges) {
            std::cin >> u >> v >> label;
        }

        Graph g{n, edges};
        if (auto res = g.topoSort(); res.has_value()) {
            auto const &order = *res;

            struct Result {
                i32 len = 0;
                i32 from = 0;
                i32 thisLabel = 0;
                i64 labelSum = 0;

                auto operator<=> (Result const &other) const -> std::weak_ordering {
                    return len <=> other.len;
                }
                auto operator== (Result const &other) const -> bool {
                    return len == other.len;
                }
            };
            std::vector<Result> f(n + 1);

            auto logN = std::bit_width(static_cast<u32>(n));
            // sj[j][i] 表示 从 i 开始的方案，第 2**j 项（0-index）。
            std::vector solutionJump(logN, std::vector<i32>(n + 1));
            // hj[j][i] 表示 从 i 开始的方案，前 2**j 项的哈希。
            std::vector hashJump(logN, std::vector<u64>(n + 1));

            for (auto u: order | views::reverse) {
                ranges::sort(g.graph[u], std::less{}, lam(e, e.label));
                for (auto e: g.graph[u]) {
                    f[u] = std::max(
                        f[u], 
                        {.len = f[e.to].len + 1, .from = e.to, 
                            .thisLabel = e.label, .labelSum = f[e.to].labelSum + e.label}, 
                        [&](Result const &a, Result const &b) -> bool {
#if false
                            if (auto cmp = a <=> b; cmp != 0) return cmp < 0;
                            std::vector<i32> x{a.thisLabel}, y{b.thisLabel};
                            for (auto cur = a.from; cur != 0; cur = f[cur].from) x.push_back(f[cur].thisLabel);
                            for (auto cur = b.from; cur != 0; cur = f[cur].from) y.push_back(f[cur].thisLabel);
                            return x > y;
#else
                            if (auto cmp = a <=> b; cmp != 0) return cmp < 0;
                            if (auto cmp = a.thisLabel <=> b.thisLabel; cmp != 0) return cmp > 0;
                            i32 p0 = a.from, p1 = b.from;
                            for (auto j = logN; j --> 0; ) {
                                if (hashJump[j][p0] == hashJump[j][p1]) {
                                    p0 = solutionJump[j][p0];
                                    p1 = solutionJump[j][p1];
                                }
                            }
                            
                            return f[p0].thisLabel > f[p1].thisLabel;
#endif
                        }
                    );
                }

                // 维护具体方案的倍增
                solutionJump[0][u] = f[u].from;
                for (i32 j = 1; j < logN; j++) {
                    solutionJump[j][u] = solutionJump[j - 1][solutionJump[j - 1][u]];
                    if (solutionJump[j][u] == 0) break;
                }

                // 对前面的一段哈希
                // hj[0][u]：只有一条线段
                hashJump[0][u] = f[u].thisLabel;
                {
                    u64 scale = hashBase;
                    for (i32 j = 1; j < logN; j++, scale *= scale) {
                        hashJump[j][u] = hashJump[j - 1][u] * scale + hashJump[j - 1][solutionJump[j - 1][u]];
                    }
                }
            }
            
            for (i32 u = 1; u <= n; u++) {
                std::cout << f[u].len << " " << f[u].labelSum << endl;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_8987280464595201::solve();
    return 0;
}
