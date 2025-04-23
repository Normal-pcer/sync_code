/**
 * @link https://www.luogu.com.cn/problem/P11831
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
「我该在哪里停留？我问我自己。」
1. 将图预处理为 n 个 reachable[i] 的形式。
令 reachable[i][j] 为真，表示可以从 i 走到 j。
可以使用 std::bitset 来存储，空间复杂度为 n^2 / w。
考虑如何处理。没有环，可以直接在拓扑序上进行 dp，暴力维护即可。
预处理的复杂度就已经是 n * m / w = 1.5e8 了，但是由于每次测试数据给了 3 秒，还算可以接受。
接下来的部分就完全不需要图上的操作了。

2. 查询。
下文中的 limits 和 values 表示原题题面的 a 和 b。

对于一次查询 l_lim, r_lim, u。
记点集 A 表示所有满足以下条件的点 v：
i. l <= limit[v] <= r.
ii. reachable[u][v].

对于 limits 的值域分块处理。记块长为 bsize_lim。
用块 b0_lim[i] 来维护一个点集，使得每个点 v 满足：
limit[v] in [bsize_lim * i, bsize_lim * (i + 1))

这样就可以把所有合法的整块，一起加入一个点集中。但是复杂度是 sqrt(n) * sqrt(n)。我们一般称为 O(n)。
不过注意到查询很多次，并且修改相对简单很多（每个块上的修改是 O(1) 的）。于是使用前缀和优化。

用块 b_lim[i] 来维护一个点集，使得点 v 满足：
limit[v] in [0, bsize_lim * (i + 1))

这样所有合法的整块，可以转化为两个点集“相减”。散块直接暴力加入这个点集。
这个步骤的复杂度：
n / w + n / bsize_lim

通过以上方法维护出来了点集 A，然后考虑找到最大的 value。
对于 values 的值域分块处理。记块长为 bsize_val。
用块 b_val[i] 来维护一个点集，使得每个点 v 满足：
value[v] in [0, bsize_val * (i + 1))

接下来，可以发现，A 和 b_val[i].set 是否存在交集，是存在单调性的。
于是可以使用二分查找最大的出现合法点的值域块。接下来选择块内 value[v] 最大的即可。
这个步骤的复杂度：
n * log(n / bsize_val) / w + bsize_val

空间复杂度：
((n / bsize_lim) + (n / bsize_val)) * n / w

修改：
修改为单点修改，似乎比较简单。
修改可能会改变两个点的 limit 和 value。
所有的 limits 块或者 values 块都会被单点修改。
复杂度为：(n / bsize_lim) + (n / bsize_val)
*/
namespace Solution_1227247956830917 {
    i32 constexpr maxN = 100'008;
    using PointsSet = std::bitset<maxN>;

    class Graph {
        i32 n;
        std::vector<std::vector<i32>> graph;
        std::vector<i32> outDeg;

        // 获取图的拓扑序
        auto topoSort() const -> std::vector<i32> {
            std::vector<i32> inDeg(n + 1);
            for (i32 u = 1; u <= n; u++) {
                for (auto v: graph[u]) {
                    inDeg[v]++;
                }
            }
            std::deque<i32> q;
            for (i32 u = 1; u <= n; u++) {
                if (inDeg[u] == 0) q.push_back(u);
            }
            std::vector<i32> order;
            order.reserve(n);
            while (not q.empty()) {
                auto u = q.front(); q.pop_front();
                order.push_back(u);

                for (auto v: graph[u]) {
                    inDeg[v]--;
                    if (inDeg[v] == 0) q.push_back(v);
                }
            }
            return order;
        }
    public:
        Graph(i32 n, std::vector<std::pair<i32, i32>> const &edges): n(n), graph(n + 1), outDeg(n + 1) {
            for (auto [u, v]: edges) outDeg[u]++;
            for (auto [u, v]: edges) graph[u].push_back(v);
        }

        // 预处理一个点能到达的点集
        auto getReachable() const -> std::vector<PointsSet> {
            std::vector<PointsSet> reach(n + 1);
            for (i32 u = 1; u <= n; u++) {
                reach[u][u] = true;
            }

            auto order = topoSort();
            for (auto u: order | views::reverse) {
                for (auto v: graph[u]) {
                    reach[u] |= reach[v];
                }
            }

            return reach;
        }
    };

#if false
    // 用于调试：暴力模拟的 Blocks 类
    class Blocks {
        i32 n;
        std::vector<i32> buc;
        std::vector<i32> valueOf;

    public:
        Blocks(i32 n): n(n), buc(n + 1), valueOf(n + 1) {}

        auto insert(i32 point, i32 value) -> void {
            valueOf[point] = value;
            buc[value] = point;
        }
        auto erase(i32 point) -> void {
            buc[valueOf[point]] = 0;
            valueOf[point] = 0;
        }

        // 获取位于一个值域内的子点集
        auto getPointsBetween(i32 min, i32 max, PointsSet const &super) -> PointsSet {
            PointsSet res;

            for (i32 i = min; i <= max; i++) {
                if (buc[i] != 0 and super[buc[i]]) res[buc[i]] = true;
            }

            return res;
        }

        // 获取一个点集内权值最大的点
        auto getMaxValue(PointsSet const &points) -> i32 {
            i32 ans = 0;
            for (i32 u = 1; u <= n; u++) {
                if (points[u]) chkMax(ans, valueOf[u]);
            }

            return ans;
        }
    };
#else
    class Blocks {
        struct Block {
            PointsSet data;
        };
        std::vector<Block> blocks;

        i32 n;
        i32 blockSize;  // 第 i 个块维护 [0, blockSize * i) 值域区间的信息
        i32 blockCount;

        std::vector<i32> valueOf;
        std::vector<i32> pointOf;
    public:
        Blocks(i32 n): n(n), blockSize(static_cast<i32>(1)), valueOf(n + 1), pointOf(n + 1) {
            blockCount = n / blockSize + 2;
            blocks.resize(blockCount);
        }

        auto constexpr blockBegin(i32) const -> i32 {
            return 0;
        }
        auto constexpr blockEnd(i32 i) const -> i32 {
            return blockSize * i;
        }
        auto constexpr getBlockOf(i32 value) const -> i32 {
            return value / blockSize + 1;
        }

        auto insert(i32 point, i32 value) -> void {
            assert(valueOf[point] == 0);  // 没有被插入过
            for (i32 i = 0; i < blockCount; i++) {
                if (value < blockEnd(i)) continue;
                // 如果这个点位于块的管理值域，则记录
                blocks[i].data[point] = true;
            }
            valueOf[point] = value;
            pointOf[value] = point;
        }
        auto erase(i32 point) -> void {
            assert(valueOf[point] != 0);  // 已经存在
            auto value = valueOf[point];
            for (i32 i = 0; value < blockSize * i; i++) {
                blocks[i].data[point] = false;
            }
            valueOf[point] = 0;
            pointOf[value] = 0;
        }

        // 获取位于一个值域内的子点集
        auto getPointsBetween(i32 min, i32 max, PointsSet const &super) const -> PointsSet {
            debug {
                std::cerr << "blocks: " << std::endl;
                for (auto const &block: blocks) {
                    std::cerr << block.data << std::endl;
                }
            }
            auto block = getBlockOf(max);
            auto extraBlock = getBlockOf(min);

            PointsSet res{};
            // 尝试记录一个可能的 value
            auto join = [&](i32 val) -> void {
                if (auto p = pointOf[val]; p != 0 and super[p]) {
                    res[p] = true;
                }
            };

            if (block - extraBlock <= 1) {
                // 全都是散块
                for (i32 val = min; val <= max; val++) join(val);
                return res;
            } else {
                // 对于其中的所有整块，直接通过前缀和统计
                // [size * extraBlock, size * (block - 1))
                res = super & (blocks[block - 1].data ^ blocks[extraBlock].data);

                debug std::cerr << res << std::endl;
                // 散块暴力加入
                // [size * (block - 1), max]
                assert(max >= blockSize * (block - 1));
                for (i32 val = blockEnd(block - 1); val <= max; val++) join(val);
                // [min, size * extraBlock)
                debug std::cerr << res << std::endl;
                assert(min <= blockSize * extraBlock);
                for (i32 val = min; val != blockEnd(extraBlock); val++) join(val);
                debug std::cerr << res << std::endl;
                return res;
            }
        }

        // 获取一个点集内权值最大的点
        auto getMaxValue(PointsSet const &points) -> i32 {
            i32 ans = 0;
            for (i32 u = 1; u <= n; u++) {
                if (points[u]) chkMax(ans, valueOf[u]);
            }

            return ans;
        }
    };
#endif

    auto solve() -> void {
        i32 n, m, q;
        std::cin >> n >> m >> q;

        std::vector<std::pair<i32, i32>> edges(m);
        for (auto &[x, y]: edges) {
            std::cin >> x >> y;
        }
        Graph g{n, edges};

        auto reach = g.getReachable();
        never {
            for (i32 u = 1; u <= n; u++) {
                for (i32 v = 1; v <= n; v++) {
                    if (reach[u][v]) {
                        std::cerr << "reachable: " << u << ", " << v << std::endl;
                    }
                }
            }
        }

        std::vector<i32> limits(n + 1), values(n + 1);
        for (auto &x: limits | views::drop(1)) std::cin >> x;
        for (auto &x: values | views::drop(1)) std::cin >> x;

        Blocks bLim(n), bVal(n);
        for (i32 u = 1; u <= n; u++) {
            bLim.insert(u, limits[u]);
            bVal.insert(u, values[u]);
        }

        for (auto _ = q; _ --> 0; ) {
            i32 op;
            std::cin >> op;

            if (op == 1) {
                i32 u, v;
                std::cin >> u >> v;

                std::swap(limits[u], limits[v]);
                bLim.erase(u), bLim.erase(v);
                bLim.insert(u, limits[u]);
                bLim.insert(v, limits[v]);
            } else if (op == 2) {
                i32 u, v;
                std::cin >> u >> v;

                std::swap(values[u], values[v]);
                bVal.erase(u), bVal.erase(v);
                bVal.insert(u, values[u]);
                bVal.insert(v, values[v]);
            } else {
                // 查询
                i32 u, l, r;
                std::cin >> u >> l >> r;

                // 获取被选中的点集
                auto selected = bLim.getPointsBetween(l, r, reach[u]);
                debug std::cerr << "selected: " << selected << std::endl;
                auto ans = bVal.getMaxValue(selected);
                std::cout << ans << endl;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 c, t;
    std::cin >> c >> t;

    static_cast<void>(c);
    for (auto _ = t; _ --> 0; ) {
        Solution_1227247956830917::solve();
    }
    return 0;
}
