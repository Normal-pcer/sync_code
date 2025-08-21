/**
 * @link https://www.luogu.com.cn/problem/P3806
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

#endif
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
点分治模板。
假设当前查询的值为 val，如果一个节点下面挂载了一个长度为 x 的链，和一个长度为 val - x 的链，则可以记录答案。

从树的重心 root 上开始遍历，对于子树一共有三种步骤（使用桶 bucket 记录，挂载到 root 上的链长度）：
- 查看该子树下的链长，是否有些可以和 bucket 中已知的链，拼成一个目标链。
- 把该子树下的链长，存放到桶 bucket 中。
- 递归处理子树内部的答案。
*/
namespace Solution_3910019787670509 {
namespace {
    using u8 = std::uint8_t;

    class Bucket {
        std::vector<std::uint8_t> buc;
        std::deque<u32> modified;
    public:
        Bucket(i32 max): buc(max + 1) {}

        auto operator[] (uz index) -> u8 & {
            modified.emplace_back(index);
            return buc[index];
        }

        auto has(i32 val) const -> bool {
            return 0 <= val and val < static_cast<i32>(buc.size());
        }

        auto clear() -> void {
            for (auto i: modified) {
                buc[i] = false;
            }
            modified.clear();
        }
    };

    class Tree {
        struct Node {
            u32 to{};
            i32 len{};
        };
        std::vector<std::vector<Node>> graph;

        std::vector<u32> size;
        std::vector<u8> removed;
        std::vector<i32> depth;
        u32 root = 0, total_size;

        auto get_size_impl(u32 u, u32 pa) -> void {
            size[u] = 1;
            for (auto [v, _]: graph[u]) {
                if (removed[v] or v == pa) continue;
                get_size_impl(v, u);
                size[u] += size[v];
            }
        }
        auto get_size() -> void {
            get_size_impl(root, 0);
        }
        // 把当前树的根节点移动到重心上
        auto move_root_impl(u32 u, u32 pa) -> void {
            u32 max_child = 0;
            for (auto [v, _]: graph[u]) {
                if (removed[v] or v == pa) continue;
                move_root_impl(v, u);
                chkMax(max_child, size[v]);
            }
            chkMax(max_child, total_size - size[u]);
            if (max_child <= total_size / 2) root = u;
        }
        auto move_root() -> void {
            get_size();
            move_root_impl(root, 0);
            get_size();
        }

        // 在子树中寻找：是否有一条链，可以和已有的拼接成目标
        // 同时维护全部子树下的 depth
        // 需要保证 u 节点的 depth 正确
        auto find_ans(u32 u, u32 pa, Bucket &buc) -> void {
            for (u32 i = 0; i != queries.size(); ++i) {
                if (answers[i]) continue;
                auto expect = queries[i] - depth[u];
                if (buc.has(expect) and buc[expect]) answers[i] = true;
            }
            for (auto [v, len]: graph[u]) {
                if (removed[v] or v == pa) continue;
                depth[v] = depth[u] + len;
                find_ans(v, u, buc);
            }
        }

        // 把子树中的全部链长存储到桶中
        auto record_lens(u32 u, u32 pa, Bucket &buc) -> void {
            if (buc.has(depth[u])) buc[depth[u]] = true;
            for (auto [v, _]: graph[u]) {
                if (removed[v] or v == pa) continue;
                record_lens(v, u, buc);
            }
        }

        // 点分治主要逻辑
        // u 视为根节点
        auto dfs(u32 u, Bucket &buc) -> void {
            buc.clear();

            buc[0] = true;
            for (auto [v, len]: graph[u]) {
                if (removed[v]) continue;
                depth[v] = len;
                find_ans(v, u, buc);
                record_lens(v, u, buc);
            }

            // 统计子树内的独立答案
            for (auto [v, _]: graph[u]) {
                if (removed[v]) continue;
                removed[u] = true; // 单独抽取子树
                total_size = size[v];
                root = v;
                move_root(); // 重心为新的根节点
                dfs(root, buc);
            }
        }
    public:
        std::vector<i32> queries;
        std::vector<u8> answers;
        Tree(u32 n): graph(n + 1), size(n + 1), removed(n + 1), depth(n + 1), total_size(n) {}

        auto query() -> auto & {
            answers.resize(queries.size());
            Bucket buc{10000005};

            root = 1, move_root();
            dfs(root, buc);

            return answers;
        }
        auto add_edge(u32 u, u32 v, i32 len) -> void {
            graph[u].emplace_back(v, len);
            graph[v].emplace_back(u, len);
        }
    };

    auto solve() -> void {
        u32 n{}, m{};
        std::cin >> n >> m;

        Tree tree{n};
        for (auto _ = n - 1; _ --> 0; ) {
            u32 u{}, v{}; i32 x{};
            std::cin >> u >> v >> x;
            tree.add_edge(u, v, x);
        }

        std::vector<i32> queries(m);
        for (auto &x: queries) std::cin >> x;
        tree.queries = std::move(queries);

        for (auto ans: tree.query()) {
            if (ans) std::cout << "AYE\n";
            else std::cout << "NAY\n";
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_3910019787670509::solve();
    return 0;
}
