/**
 * @link https://www.luogu.com.cn/problem/P2633
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_5558157793800670 {
    template <typename T, typename Func>
    concept CallableAs = std::convertible_to<T, std::function<Func>>;

    class PersistentSegTree {
    public:
        struct Node {
            i32 sum = 0;
            i32 left = 0, right = 0;
        };
    private:
        inline static std::deque<Node> nodes;

        std::vector<i32> rootOfVersion;
        i32 rootBegin = 0, rootEnd = 0;

        auto pushUp(i32 p) {
            nodes[p].sum = nodes[nodes[p].left].sum + nodes[nodes[p].right].sum;
        }
        auto addAt(i32 pos, i32 val, i32 &p, i32 nodeBegin, i32 nodeEnd) -> void {
            nodes.push_back(nodes[p]), p = static_cast<i32>(nodes.size()) - 1;
            if (nodeEnd - nodeBegin == 1) {
                assert(nodeBegin == pos);
                nodes[p].sum += val;
                return;
            }
            auto nodeMid = std::midpoint(nodeBegin, nodeEnd);
            if (pos < nodeMid) addAt(pos, val, nodes[p].left, nodeBegin, nodeMid);
            else addAt(pos, val, nodes[p].right, nodeMid, nodeEnd);
            pushUp(p);
        }
        template <uz n, typename Func>
        auto multiVersionFindFirst(Func &&getValue, i32 val, std::array<i32, n> current, i32 nodeBegin, i32 nodeEnd) -> i32 
                requires CallableAs<Func, i32(std::array<Node, n>)> {
            // 当前处于叶子节点
            if (nodeEnd - nodeBegin == 1) {
                std::array<Node, n> currentNodes;
                ranges::transform(current, currentNodes.begin(), lam(x, nodes[x]));
                auto currentVal = getValue(currentNodes);
                if (currentVal >= val) return nodeBegin;
                else return nodeEnd;
            }
            // 否则，判断左子树的维护范围
            auto nodeMid = std::midpoint(nodeBegin, nodeEnd);
            std::array<i32, n> leftChildrenIndex;
            std::array<i32, n> rightChildrenIndex;
            std::array<Node, n> leftChildren;  // 所有左孩子的节点数组
            for (uz i = 0; i < n; i++) {
                leftChildrenIndex[i] = nodes[current[i]].left;
                rightChildrenIndex[i] = nodes[current[i]].right;
                leftChildren[i] = nodes[leftChildrenIndex[i]];
            }

            auto leftVal = getValue(leftChildren);

            if (leftVal >= val) {
                return multiVersionFindFirst(getValue, val, leftChildrenIndex, nodeBegin, nodeMid);
            } else {
                return multiVersionFindFirst(getValue, val - leftVal, rightChildrenIndex, nodeMid, nodeEnd);
            }
        }
    public:
        PersistentSegTree(i32 rootBegin, i32 rootEnd, i32 maxVersion): 
                rootOfVersion(maxVersion + 1), rootBegin(rootBegin), rootEnd(rootEnd) {
            if (nodes.empty()) {
                nodes.push_back({});
            }
        }
        auto addAt(i32 baseVersion, i32 newVersion, i32 pos, i32 val) -> void {
            rootOfVersion[newVersion] = rootOfVersion[baseVersion];
            // asm("int 3");
            addAt(pos, val, rootOfVersion[newVersion], rootBegin, rootEnd);
        }
        /**
         * 在多个版本上进行「联查」。
         * @param getValue 通过有序的 n 个线段树节点来计算权值。调用签名如下：
         * i32 getValue(std::array<Node, n> currentNodes);
         * @param val 正在查找的权值
         * @returns 第一个下标 x，使得线段树上区间 [1, x] 对应的权值恰好大于等于 val。
         */
        template <uz n, typename Func>
        auto multiVersionFindFirst(Func &&getValue, i32 val, std::array<i32, n> versions) -> i32 
                requires CallableAs<Func, i32(std::array<Node, n>)> {
            std::array<i32, n> current;
            for (uz i = 0; i < n; i++) {
                current[i] = rootOfVersion[versions[i]];
            }
            return multiVersionFindFirst<n>(getValue, val, current, rootBegin, rootEnd);
        }
    };
    class Tree {
        i32 n;
        std::vector<i32> a;  // 点权
        std::vector<std::vector<i32>> graph;  // 临接表
        std::vector<i32> size, depth, fa, son, top, order, index;

        // 使用线段树的第 i 个版本，表示点 i 至根节点这一条链上出现的所有点
        PersistentSegTree sgt;

        // 处理 size, depth, fa, son
        auto dfs1(i32 u, i32 prev) -> void {
            depth[u] = depth[prev] + 1;
            fa[u] = prev;
            size[u] = 1;
            for (auto v: graph[u]) {
                if (v != prev) {
                    dfs1(v, u);
                    size[u] += size[v];
                    if (size[v] > size[son[u]]) {
                        son[u] = v;
                    }
                }
            }
        }
        // 处理 top, order, index
        auto dfs2(i32 u, i32 t) -> void {
            index[u] = static_cast<i32>(order.size());
            order.push_back(u);
            top[u] = t;
            if (son[u] != 0) {
                dfs2(son[u], t);
            }
            for (auto v: graph[u]) {
                if (v != fa[u] and v != son[u]) {
                    dfs2(v, v);
                }
            }
        }
        // 在线段树中插入
        auto dfs3(i32 u, i32 prev) -> void {
            sgt.addAt(prev, u, a[u], 1);
            for (auto v: graph[u]) {
                if (v != prev) {
                    dfs3(v, u);
                }
            }
        }
        auto lca(i32 u, i32 v) -> i32 {
            while (top[u] != top[v]) {
                if (depth[top[u]] < depth[top[v]]) std::swap(u, v);
                u = fa[top[u]];
            }
            if (depth[u] > depth[v]) std::swap(u, v);
            return u;
        }
    public:
        Tree(i32 n, std::vector<i32> const &a, std::vector<std::pair<i32, i32>> const &edges):
                n(n), a(a), graph(n + 1), size(n + 1), depth(n + 1), fa(n + 1), son(n + 1), top(n + 1), index(n + 1), sgt(0, n + 1, n) {
            for (auto [x, y]: edges) {
                graph[x].push_back(y);
                graph[y].push_back(x);
            }

            dfs1(1, 0);
            dfs2(1, 1);
            dfs3(1, 0);
        }

        // 题干所述的查询
        auto query(i32 u, i32 v, i32 k) -> i32 {
            // [u] + [v] - [lca] - [fa[lca]] 即为整个路径
            auto lcaUV = lca(u, v);
            using Node = PersistentSegTree::Node;
            auto ans = sgt.multiVersionFindFirst<4>([&](std::array<Node, 4> arr) -> i32 {
                return arr[0].sum + arr[1].sum - arr[2].sum - arr[3].sum;
            }, k, {u, v, lcaUV, fa[lcaUV]});
            return ans;
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<i32> a(n + 1);
        for (auto &x: a | views::drop(1)) {
            std::cin >> x;
        }
        auto values = a; 
        {
            ranges::sort(values);
            auto tmp = ranges::unique(values.begin(), values.end());
            values.erase(tmp.begin(), tmp.end());

            debug {
                std::cout << "values: " << std::endl;
                for (auto x: values) std::cout << x << ' ';
                std::cout << std::endl;
            }

            auto getIndex = [&](i32 x) -> i32 {
                auto it = ranges::lower_bound(values, x);
                assert(it != values.end() and *it == x);
                return it - values.begin();
            };

            ranges::transform(a, a.begin(), getIndex);
        }

        std::vector<std::pair<i32, i32>> edges(n - 1);
        for (auto &[x, y]: edges) std::cin >> x >> y;

        Tree tr{n, a, edges};
        i32 prevAns = 0;
        for (auto _ = m; _ --> 0; ) {
            i32 u, v, k;
            std::cin >> u >> v >> k;
            u ^= prevAns;

            auto ans = values[tr.query(u, v, k)];
            std::cout << ans << endl;
            debug std::cout << std::flush;
            prevAns = ans;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_5558157793800670::solve();
    return 0;
}
