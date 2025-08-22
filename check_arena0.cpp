/**
 * @link https://www.luogu.com.cn/problem/P6329
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
#define never if constexpr (false)
template <typename T> auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
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

namespace Solution_6561858409154546 {
namespace {
    i32 constexpr max_val = 1e6;

    using u8 = std::uint8_t;
    class SegTree {
        struct Node {
            i32 sum{};
            u32 lch{}, rch{}; 
        };

        u32 root{}, root_begin{}, root_end{};

        std::vector<Node> inline static nodes;
        auto static push_up(u32 p) -> void {
            nodes[p].sum = nodes[nodes[p].lch].sum + nodes[nodes[p].rch].sum;
        }
        auto add_at_impl(u32 pos, i32 val, u32 &p, u32 node_begin, u32 node_end) -> void {
            if (p == 0) nodes.emplace_back(), p = nodes.size() - 1;
            if (node_end - node_begin == 1) {
                assert(node_begin == pos);
                nodes[p].sum += val;
                return;
            }
            auto node_mid = std::midpoint(node_begin, node_end);
            if (pos < node_mid) add_at_impl(pos, val, nodes[p].lch, node_begin, node_mid);
            else add_at_impl(pos, val, nodes[p].rch, node_mid, node_end);
            push_up(p);
        }
        auto sum_range_impl(u32 begin, u32 end, u32 p, u32 node_begin, u32 node_end) -> i32 {
            if (p == 0) return 0;
            if (node_begin >= begin && node_end <= end) return nodes[p].sum;
            i32 sum = 0;
            auto node_mid = std::midpoint(node_begin, node_end);
            if (begin < node_mid) sum += sum_range_impl(begin, end, nodes[p].lch, node_begin, node_mid);
            if (end > node_mid) sum += sum_range_impl(begin, end, nodes[p].rch, node_mid, node_end);
            return sum;
        }
    public:
        SegTree(): root_end(max_val + 1) {
            [[maybe_unused]] auto static _ = (nodes.emplace_back(), nodes.reserve(1000000000ULL), 0);
        }
        auto add_at(u32 pos, i32 val) -> void {
            return add_at_impl(pos, val, root, root_begin, root_end);
        }
        auto sum_range(u32 begin, u32 end) -> i32 {
            return sum_range_impl(begin, end, root, root_begin, root_end);
        }
    };
    class CDT;
    class Tree {
    public:
        struct Node {
            u32 to{};
            i32 len{};
        };

        std::vector<std::vector<Node>> graph;
        std::vector<u32> size, level, parent, gch, top;
        std::vector<i32> depth;
        std::vector<u8> removed;

        u32 root = 1, total_size = 0;

        // 获取基本信息
        auto get_info_impl(u32 u, u32 pa) -> void {
            size[u] = 1;
            parent[u] = pa;
            gch[u] = 0;
            ++total_size;

            for (auto [v, len]: graph[u]) {
                if (removed[v] || v == pa) continue;
                depth[v] = depth[u] + len;
                level[v] = level[u] + 1;
                get_info_impl(v, u);
                size[u] += size[v];
                if (size[v] > size[gch[u]]) gch[u] = v;
            }
        }
        // 获取重链头 top
        auto get_top_impl(u32 u, u32 tp) -> void {
            top[u] = tp;
            if (gch[u] != 0) get_top_impl(gch[u], tp);
            for (auto [v, _]: graph[u]) {
                if (removed[v] || v == parent[u] || v == gch[u]) continue;
                get_top_impl(v, v);
            }
        }
        auto get_info() -> void {
            depth[root] = 0;
            level[root] = 0;
            total_size = 0;
            get_info_impl(root, 0);
            get_top_impl(root, root);
        }
        auto lca(u32 u, u32 v) -> u32 {
            while (top[u] != top[v]) {
                if (level[top[u]] < level[top[v]]) std::swap(u, v);
                u = parent[top[u]];
            }
            if (level[u] < level[v]) std::swap(u, v);
            return v;
        }
        auto dis(u32 u, u32 v) -> u32 {
            auto p = lca(u, v);
            return depth[u] + depth[v] - 2 * depth[p];
        }
        // 换根到树的重心
        auto move_root_impl(u32 u, u32 pa) -> void {
            u32 max_child = 0;
            for (auto [v, _]: graph[u]) {
                if (removed[v] || v == pa) continue;
                move_root_impl(v, u);
                chkMax(max_child, size[v]);
            }
            chkMax(max_child, total_size - size[u]);
            if (max_child <= total_size / 2) root = u;
        }
        auto move_root() -> void {
            get_info();
            move_root_impl(root, 0);
            get_info();
        }
        // 建立点分树
        auto make_cdt_impl(u32 u, std::vector<std::pair<u32, u32>> &res) -> void {
            for (auto [v, len]: graph[u]) {
                if (removed[v]) continue;

                removed[u] = true;
                total_size = size[v];
                root = v;
            
                move_root();
                res.emplace_back(u, root);
                make_cdt_impl(root, res);
            }
        }
        auto make_cdt() -> CDT;
        Tree(u32 n): graph(n + 1), size(n + 1), level(n + 1), parent(n + 1), 
                gch(n + 1), top(n + 1), depth(n + 1), removed(n + 1, false), total_size(n) {}

        auto add_edge(u32 u, u32 v, i32 len) -> void {
            graph[u].emplace_back(v, len);
            graph[v].emplace_back(u, len);
        }
    };

    class CDT {
        std::vector<std::vector<u32>> graph;
        std::vector<u32> parent;
        std::vector<i32> val;
        // 每个节点 P，它子树中的所有点 Q，PQ 之间的距离为 dis
        // dis 映射到，所有满足该距离 Q 点的点权之和
        std::vector<SegTree> buc;
        // 类似上一个，但是对于节点 P，距离的含义是 Q 到 P 的上级节点
        std::vector<SegTree> buc_to_parent;

        u32 root{};
        Tree *source{};

        auto get_info_impl(u32 u, u32 pa) -> void {
            parent[u] = pa;
            for (auto v: graph[u]) {
                if (v == pa) continue;
                get_info_impl(v, u);
            }
        }
        auto get_info() -> void {
            get_info_impl(root, 0);
        }
        auto add_point_val_impl(u32 u, i32 delta) -> void {
            for (auto p = u; p != 0; p = parent[p]) {
                buc[p].add_at(source->dis(p, u), delta);
                if (p != root) {
                    buc_to_parent[p].add_at(source->dis(parent[p], u), delta);
                }
            }
        }
        auto query_nearby_impl(u32 u, i32 limit) -> i32 {
            i32 ans = 0;

            // 子树外侧的贡献
            for (u32 r = u, prev = 0; r != 0; prev = r, r = parent[r]) {
                // 从 u 到 lca 的距离
                auto d1 = static_cast<i32>(source->dis(u, r));
                // 设 d2 表示从 v 到 lca 的距离，应当满足 d1 + d2 <= k，d2 <= k - d1
                auto d2_lim = limit - d1;
                if (d2_lim < 0) continue;

                // 记录所有 v 点的点权和
                ans += buc[r].sum_range(0, d2_lim + 1);

                // 去除重复情况
                // 如果两条链隶属于同一个子树，则它一定被重复计算了
                ans -= buc_to_parent[prev].sum_range(0, d2_lim + 1);
            }

            return ans;
        }
    public:
        CDT(u32 n, u32 root, std::vector<std::pair<u32, u32>> const &edges, Tree *source): 
                graph(n + 1), parent(n + 1), val(n + 1), buc(n + 1), buc_to_parent(n + 1), root(root), source(source) {
            for (auto [u, v]: edges) add_edge(u, v);
            get_info();
        }
        auto add_edge(u32 u, u32 v) -> void {
            graph[u].emplace_back(v);
            graph[v].emplace_back(u);
        }
        auto add_point_val(u32 u, i32 delta) -> void {
            return add_point_val_impl(u, delta);
        }
        auto query_nearby(u32 u, i32 limit) -> i32 {
            return query_nearby_impl(u, limit);
        }
        auto assign_point_val(u32 u, i32 to) -> void {
            auto delta = to - val[u];
            add_point_val(u, delta);
            val[u] = to;
        }
    };

    auto Tree::make_cdt() -> CDT {
        std::vector<std::pair<u32, u32>> edges;
        get_info();
        edges.reserve(total_size);

        move_root();
        make_cdt_impl(root, edges);

        // 恢复原树形态
        ranges::fill(removed, false);
        move_root();

        return {total_size, root, edges, this};
    };
    
    auto solve() -> void {
        i32 n{}, m{}; std::cin >> n >> m;

        std::vector<i32> init(n);
        for (auto &x: init) std::cin >> x;

        Tree tr{static_cast<u32>(n)};
        for (auto _ = n - 1; _ --> 0; ) {
            u32 u{}, v{}; std::cin >> u >> v;
            tr.add_edge(u, v, 1);
        }

        auto cdt = tr.make_cdt();

        for (i32 u = 0; u < n; ++u) {
            cdt.assign_point_val(u32(u + 1), init[u]);
        }

        i32 last_ans = 0;
        for (auto _ = m; _ --> 0; ) {
            i32 op{}, x{}, y{};
            std::cin >> op >> x >> y;

            x ^= last_ans, y ^= last_ans;

            if (op == 0) {
                auto ans = cdt.query_nearby(u32(x), y);
                last_ans = ans;
                std::cout << ans << endl;
            } else {
                cdt.assign_point_val(u32(x), y);
            }
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6561858409154546::solve();
    return 0;
}