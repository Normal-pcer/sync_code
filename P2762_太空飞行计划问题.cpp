/**
 * @link https://www.luogu.com.cn/problem/P2762
 * @link https://neooj.com:8082/oldoj/problem.php?id=2489
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
连一排点表示收益，另一排点表示开销。
从超级起点，到收益点是正常的价钱；开销点到超级终点也是。
收益点和开销点之间连的是无穷大。
*/
namespace Solution_4329982060672190 {
    i32 inf = 0x3f3f3f3f;
    class Graph {
        struct EdgeNode {
            i32 to;
            i32 flow;
            i32 next = 0;
        };
        i32 n;
    public:
        std::vector<EdgeNode> edges;
        std::vector<i32> firstEdge;

        Graph(i32 n): n(n), edges(2), firstEdge(n + 1, 0) {}

        auto addEdge(i32 u, i32 v, i32 val) -> void {
            edges.push_back({.to = v, .flow = val, .next = firstEdge[u]});
            firstEdge[u] = static_cast<i32>(edges.size()) - 1;
            edges.push_back({.to = u, .flow = 0, .next = firstEdge[v]});
            firstEdge[v] = static_cast<i32>(edges.size()) - 1;
        }

        std::vector<i32> level;
        auto getFlow(i32 s, i32 t) -> i32 {
            // 获取层次图，返回是否可以 from -> to。
            auto getSliced = [&](i32 from, i32 to) -> bool {
                level = std::vector<i32>(n + 1, -1);
                std::deque<i32> q;
                level[from] = 1;
                q.push_back(from);
                while (not q.empty()) {
                    auto u = q.front(); q.pop_front();
                    for (auto i = firstEdge[u]; i != 0; i = edges[i].next) {
                        auto const &e = edges[i];
                        if (e.flow == 0) continue;
                        if (level[e.to] != -1) continue;
                        level[e.to] = level[u] + 1;
                        q.push_back(e.to);
                    }
                }
                return level[to] != -1;
            };

            // 尽可能流向孩子
            // u: 当前节点
            // t: 目标节点
            // flowLimit: 由于各种原因造成的流量限制，例如上游的管道，以及其他孩子过多分流
            // 返回：最多向这个子树流掉多少
            auto tryFlow = [&](auto &&tryFlow, i32 u, i32 t, i32 flowLimit) -> i32 {
                if (u == t) return flowLimit;  // 到达终点，可以全部流走

                i32 totalFlow = 0;  // 总共流走了多少
                for (auto &i = firstEdge[u]; i != 0; i = edges[i].next) {
                    auto &e = edges[i];
                    if (e.flow == 0) continue;  // 不可能流通
                    if (level[e.to] != level[u] + 1) continue;  // 保证层次正确
                    // 尝试从这个子树流走
                    // 同时受到现有限制和这条边的流量限制
                    auto current = tryFlow(tryFlow, e.to, t, std::min(flowLimit, e.flow));

                    e.flow -= current;  // 这条边的承载能力减小
                    edges[i ^ 1].flow += current;  // 允许反向流过
                    totalFlow += current;  // 统计答案
                    flowLimit -= current;  // 这个孩子进行分流，进一步减小其他人的限制

                    // 这棵子树已经不可能流过东西了
                    if (current == 0) {
                        level[e.to] = -1;  // 设为无效值，即隐性地删除它
                    }
                    // 接下来不可能再流动了
                    if (flowLimit == 0) {
                        break;
                    }
                }
                // debug std::cerr << std::format("try({}, {}, {}) -> {}", u, t, flowLimit, totalFlow) << std::endl;
                return totalFlow;
            };

            i32 ans = 0;
            // 重建层次图，允许经过新的反向边
            while (getSliced(s, t)) {
                auto inf = std::numeric_limits<i32>::max();
                auto copy = firstEdge;  // 备份当前的边
                // 在当前的图中尽力流动
                while (auto flow = tryFlow(tryFlow, s, t, inf)) {
                    ans += flow;
                }
                firstEdge = copy;
            }
            return ans;
        }
    };
    auto solve() -> void {
        i32 m, n;
        std::cin >> m >> n;

        std::vector<i32> rewards(m + 1);
        std::vector<std::vector<i32>> used(m + 1);
        std::vector<i32> cost(n + 1);

        for (char ch{}; ch != '\n'; std::cin.get(ch));
        for (i32 i = 1; i <= m; i++) {
            std::string line;
            std::getline(std::cin, line);
            std::stringstream ss{line};

            i32 x;
            ss >> rewards[i];
            while (ss >> x) used[i].push_back(x); 
        }
        for (i32 i = 1; i <= n; i++) std::cin >> cost[i];

        auto getRewardNode = lam(x, x);
        auto getCostNode = lam(x, x + m);
        i32 const startNode = n + m + 1, targetNode = n + m + 2;

        Graph g{n + m + 2};
        for (i32 u = 1; u <= m; u++) {
            for (auto v: used[u]) {
                g.addEdge(getRewardNode(u), getCostNode(v), inf);
            }
        }

        for (i32 tool = 1; tool <= n; tool++) {
            g.addEdge(getCostNode(tool), targetNode, cost[tool]);
        }

        for (i32 exp = 1; exp <= m; exp++) {
            g.addEdge(startNode, getRewardNode(exp), rewards[exp]);
        }

        auto ans = std::accumulate(rewards.begin(), rewards.end(), 0) - g.getFlow(startNode, targetNode);
        for (i32 i = 1; i <= m; i++) {
            if (g.level[getRewardNode(i)] != -1) {
                std::cout << i << " ";
            }
        }
        std::cout << endl;
        for (i32 i = 1; i <= n; i++) {
            if (g.level[getCostNode(i)] != -1) {
                std::cout << i << " ";
            }
        }
        std::cout << endl;
        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4329982060672190::solve();
    return 0;
}
