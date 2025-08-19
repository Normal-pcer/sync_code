/**
 * @link https://www.luogu.com.cn/problem/P4735
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

char constexpr endl = '\n';
#include "./libs/fixed_int.hpp"

namespace Solution_2266588065936995 {
namespace {
    class Trie {
        struct Node {
            u32 count{};
            std::array<u32, 2> children;
        };
        std::deque<Node> tree;
        std::vector<u32> roots;

        u32 static constexpr max_mask = 1U << 30;

        auto query(u32 val, u32 mask, u32 add_p, u32 sub_p) -> u32 {
            if (mask == 0) return 0;
            auto target = bool(val & mask) ^ 1;
            auto other = target ^ 1;
            if (tree[tree[add_p].children[target]].count - tree[tree[sub_p].children[target]].count > 0) {
                return mask | query(val, mask >> 1, tree[add_p].children[target], tree[sub_p].children[target]);
            } else {
                return query(val, mask >> 1, tree[add_p].children[other], tree[sub_p].children[other]);
            }
        }
        auto insert(u32 val, u32 base) -> void {
            tree.emplace_back(tree[base]);
            u32 p = tree.size() - 1;
            roots.emplace_back(p);
            for (u32 mask = max_mask; mask != 0; mask >>= 1) {
                ++tree[p].count;
                tree.emplace_back(tree[tree[p].children[bool(val & mask)]]);
                p = tree[p].children[bool(val & mask)] = tree.size() - 1;  // 我相信它不是 UB
            }
            ++tree[p].count;
        }
    public:
        Trie() {
            tree.emplace_back();
            roots.emplace_back();
        }
        auto insert(u32 val) -> void {
            insert(val, roots.back());
        }
        auto query(u32 val, u32 l, u32 r) -> u32 {
            return query(val, max_mask, roots[r], roots[l - 1]);
        }
    };
    auto solve() -> void {
        i32 n{}, m{}; std::cin >> n >> m;
        u32 sum = 0;
        std::vector<u32> init(n);

        Trie trie{};
        trie.insert(0);
        for (auto &x: init) std::cin >> x, x = (sum ^= x), trie.insert(x);

        for (auto _ = m; _ --> 0; ) {
            char op{}; std::cin >> op;
            if (op == 'A') {
                u32 x{}; std::cin >> x;

                trie.insert((sum ^= x)); 
                init.emplace_back(sum);
            } else {
                u32 l{}, r{}, x{};
                std::cin >> l >> r >> x;

                std::cout << trie.query(x ^ sum, l, r) << endl;
            }
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_2266588065936995::solve();
    return 0;
}
