/**
 * @link https://www.luogu.com.cn/problem/P11831
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#ifdef LOCAL
#define debug if (DEBUG_MODE)
#else
#define debug if constexpr (false)
#endif
#define never if constexpr (false)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define _lambda_1(expr) [&]() { return expr; }
#define _lambda_2(a, expr) [&](auto a) { return expr; }
#define _lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
#define _lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
#define _lambda_overload(a, b, c, d, e, ...) _lambda_##e
#define lambda(...) _lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
    namespace views = std::views;
#endif
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"

using namespace lib;
namespace chrono = std::chrono;

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

（修正：为了和后面的统一，这里也改用后缀和）
用块 b_lim[i] 来维护一个点集，使得点 v 满足：
// limit[v] in [0, bsize_lim * (i + 1))
limit[v] in [bsize_lim * i, n + 1)

这样所有合法的整块，可以转化为两个点集“相减”。散块直接暴力加入这个点集。
这个步骤的复杂度：
n / w + bsize_lim

通过以上方法维护出来了点集 A，然后考虑找到最大的 value。
对于 values 的值域分块处理。记块长为 bsize_val。
（修正：这里也采用后缀和；前缀和只能用来找到最小值）
用块 b_val[i] 来维护一个点集，使得每个点 v 满足：
// value[v] in [0, bsize_val * (i + 1))
value[v] in [bsize_lim * i, n + 1)

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

但是这个被卡脖子了，跑不过暴力。
考虑查询时做如下的优化：
每次拿可行点集的一位，与每个 values 分块进行匹配。如果匹配不到，就切换为下一个位继续匹配。类似双指针。
由于后缀和对每一位都有单调性，所以这个方法显然是正确的。
每一次这一位不匹配，接下来的所有块，一定也不包含这一位了。
最后可以得到第一个完全不匹配的位置。
实际上不用每次取一位，每次取 64 位即可做到 n/w。

n * log(n / bsize_val) / w + bsize_val
-> n / w + n / bsize_val + bsize_val
*/
namespace Solution_1227247956830917 {
    // 手写 bitset
    template <uz size>
    class Bitset {
    public:
        using BlockType = u64;
        std::array<BlockType, ((size + 63) >> 6)> data;

        Bitset(): data() {}  // 初始化为全 0

        auto static constexpr maskAt(uz index) -> BlockType { return (BlockType)1 << (index & 63); }
        auto flip(uz index) -> void { data[index >> 6] ^= maskAt(index & 63); }  // 反转
        auto test(uz index) const -> bool { return data[index >> 6] & maskAt(index & 63); }  // 读取
        auto unset(uz index) -> void { data[index >> 6] &= ~maskAt(index & 63); }  // 设为 0
        auto set(uz index) -> void { data[index >> 6] |= maskAt(index & 63); }  // 设为 1
        auto assign(uz index, bool to) -> void { to? set(index): unset(index); }  // 设置
        auto operator|= (Bitset const &other) -> Bitset & {
            for (uz i = 0; i < data.size(); i++) data[i] |= other.data[i];
            return *this;
        }
        auto operator^= (Bitset const &other) -> Bitset & {
            for (uz i = 0; i < data.size(); i++) data[i] ^= other.data[i];
            return *this;
        }
        auto operator&= (Bitset const &other) -> Bitset & {
            for (uz i = 0; i < data.size(); i++) data[i] &= other.data[i];
            return *this;
        }
        auto operator| (Bitset const &other) const -> Bitset {
            Bitset res = *this;
            return res |= other;
        }
        auto operator& (Bitset const &other) const -> Bitset {
            Bitset res = *this;
            return res &= other;
        }
        auto operator^ (Bitset const &other) const -> Bitset {
            Bitset res = *this;
            return res ^= other;
        }
        auto operator~ () const -> Bitset {
            Bitset res = *this;
            for (uz i = 0; i < data.size(); i++) data[i] = ~data[i];
            return res;
        }
        auto operator[] (uz index) const -> bool {
            return test(index);
        }
        auto friend operator<< (std::ostream &os, Bitset const &bits) -> std::ostream & {
            for (uz i = size; i --> 0; ) os << bits.test(i);
            return os;
        }
        auto none() const -> bool {
            return ranges::none_of(data, lam(x, x != 0));
        }
    };

    i32 constexpr maxN = 100'008;
    using PointSet = Bitset<maxN>;

    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U x, Func &&func, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(func(mid), x)) begin = mid + 1;
            else end = mid;
        }
        return begin;
    }

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
            for (i32 u = 1; u <= n; u++) graph[u].reserve(outDeg[u]);
            for (auto [u, v]: edges) graph[u].push_back(v);
        }

        // 预处理一个点能到达的点集
        auto getReachable() const -> std::vector<PointSet> {
            std::vector<PointSet> reach(n + 1);
            for (i32 u = 1; u <= n; u++) {
                reach[u].set(u);
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

    // 计时器，用于分析运行速度
    class Timer {
        using Clock = chrono::system_clock;

        std::map<std::string, Clock::duration> static inline time;
        std::string label;
        Clock::time_point prevTime;
        bool enabled = true;
    public:
        Timer(std::string label): label(std::move(label)), prevTime(Clock::now()) {}
        ~Timer() { stop(); }

        auto stop() -> void {
            if (not enabled) return;
            time[label] += Clock::now() - prevTime;
            enabled = false;
        }

        auto start() -> void {
            prevTime = Clock::now();
            enabled = true;
        }

        auto static getTable() -> std::map<std::string, Clock::duration> const & {
            return time;
        }
        auto static clearTable() -> void {
            time.clear();
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
            PointSet owned;
        };
        std::vector<Block> blocks;

        i32 n;
        i32 blockSize;  // 第 i 个块维护 [blockSize * i, n + 1) 值域区间的信息
        i32 blockCount;  // 现在有多少个块

        std::vector<i32> valueOf;
        std::vector<i32> pointOf;
    public:
        Blocks(i32 n, i32 blockSize): n(n), blockSize(blockSize), valueOf(n + 1), pointOf(n + 1) {
            blockCount = getBlockOf(n) + 1;
            blocks.resize(blockCount);
        }

        auto constexpr blockBegin(i32 i) const -> i32 {
            return blockSize * i;
        }
        auto constexpr blockEnd(i32) const -> i32 {
            return n + 1;
        }
        auto constexpr getBlockOf(i32 value) const -> i32 {
            return value / blockSize;
        }

        auto insert(i32 point, i32 value) -> void {
            assert(valueOf[point] == 0);  // 没有被插入过
            for (i32 i = 0; value >= blockBegin(i); i++) {
                assert(not blocks[i].owned[point]);
                blocks[i].owned.set(point);
            }
            valueOf[point] = value;
            pointOf[value] = point;
        }
        auto erase(i32 point) -> void {
            assert(valueOf[point] != 0);  // 已经存在
            auto value = valueOf[point];
            for (i32 i = 0; value >= blockBegin(i); i++) {
                assert(blocks[i].owned[point]);
                blocks[i].owned.unset(point);
            }
            valueOf[point] = 0;
            pointOf[value] = 0;
        }

        // 获取位于一个值域内的子点集
        auto getPointsBetween(i32 min, i32 max, PointSet const &super) const -> PointSet {
            debug {
                std::cerr << "blocks: " << std::endl;
                for (auto const &block: blocks) {
                    std::cerr << block.owned << std::endl;
                }
            }

            // 第一个整块和最后一个整块的下一个
            // total ^ extra 即为中间所有整块的贡献
            auto totalBlock = getBlockOf(min) + 1;
            auto extraBlock = getBlockOf(max);

            PointSet res{};
            // 尝试记录一个可能的 value
            auto join = [&](i32 val) -> void {
                if (auto p = pointOf[val]; p != 0 and super[p]) {
                    res.set(p);
                }
            };

            if (extraBlock - totalBlock <= 1) {
                // 全都是散块
                for (i32 val = min; val <= max; val++) join(val);
                return res;
            } else {
                // 对于其中的所有整块，直接通过前缀和统计
                // [size * totalBlock, size * extraBlock)
                res = super & (blocks[totalBlock].owned ^ blocks[extraBlock].owned);
                debug std::cerr << "res = " << res << std::endl;

                // 散块暴力加入
                // [size * extraBlock, max]
                assert(max >= blockSize * extraBlock);
                for (i32 val = blockBegin(extraBlock); val <= max; val++) join(val);
                debug std::cerr << "res = " << res << std::endl;

                // [min, size * totalBlock)
                assert(min <= blockSize * totalBlock);
                for (i32 val = min; val != blockBegin(totalBlock); val++) join(val);
                debug std::cerr << "res = " << res << std::endl;
                return res;
            }
        }

#if false
        // 获取一个点集内权值最大的点
        auto getMaxValue(PointSet const &points) -> i32 {
            // 寻找第一个和 points 没有交集的块
            auto firstNoIntersect = lowerBoundValue(0, blockCount, true, [&](i32 i) -> bool {
                return (blocks[i].owned & points).none();
            });
            if (firstNoIntersect == 0) return 0;
            auto maxBlock = firstNoIntersect - 1;  // 最大值所处的块
            for (i32 val = std::min(n + 1, blockBegin(maxBlock + 1)); val --> blockBegin(maxBlock); ) {
                if (auto p = pointOf[val]; p != 0 and blocks[maxBlock].owned[p] and points[p]) {
                    return val;
                }
            }
            assert(false);
            return 0;
        }
#else
        auto getMaxValue(PointSet const &points) const -> i32 {
            // 双指针
            auto pointsPtr = points.data.begin();
            auto valuesPtr = blocks.begin();

            {
                // Timer _{"双指针"};
                for (; valuesPtr != blocks.end() and pointsPtr != points.data.end(); ++valuesPtr) {
                    auto unmatch = [&](auto pointsPtr) -> bool {
                        auto index = pointsPtr - points.data.begin();
                        // std::cerr << "unmatch returns " << ((valuesPtr->owned.data[index] & *pointsPtr) == 0) << std::endl;
                        return (valuesPtr->owned.data[index] & *pointsPtr) == 0;  // 存在交集即可
                    };
                    // 如果和当前点没有匹配上，就跳到下一组点
                    while (pointsPtr != points.data.end() and unmatch(pointsPtr)) ++pointsPtr;
                    if (pointsPtr == points.data.end()) break;  // 找到答案，直接跳出
                }
            }

            auto firstNoIntersect = static_cast<i32>(valuesPtr - blocks.begin());
            if (firstNoIntersect == 0) return 0;

            {
                // Timer _{"统计最大值"};
                auto maxBlock = firstNoIntersect - 1;
                // std::cerr << std::min(n + 1, blockBegin(maxBlock + 1)) - blockBegin(maxBlock) << std::endl;
                for (i32 val = std::min(n + 1, blockBegin(maxBlock + 1)); val --> blockBegin(maxBlock); ) {
                    if (auto p = pointOf[val]; p != 0 and blocks[maxBlock].owned[p] and points[p]) {
                        return val;
                    }
                }
            }
            assert(false);
            return 0;
        }
#endif
    };
#endif

    auto solve() -> void {
        // std::cerr << "qwq" << std::endl << std::flush;
        // Timer::clearTable();
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

        Blocks bLim(n, static_cast<i32>(std::sqrt(n))), bVal(n, static_cast<i32>(std::sqrt(n)));
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

                // Timer _{"Query"};

                // 获取被选中的点集
                auto selected = bLim.getPointsBetween(l, r, reach[u]);
                debug std::cerr << "selected: " << selected << std::endl;
                auto ans = bVal.getMaxValue(selected);
                std::cout << ans << endl;
            }
        }

        // for (auto const &[key, time]: Timer::getTable()) {
        //     std::cerr << key << ": " << chrono::duration_cast<chrono::milliseconds>(time).count() << "ms" << std::endl;
        // }
        // std::cerr << "123" << std::endl << std::flush;
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
