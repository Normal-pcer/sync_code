/**
 * @link
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
#include "./libs/panic.hpp"

namespace Solution_6314180276493067 {
namespace {
    std::mt19937 rng{std::random_device{}()};

    class Treap {
        struct Node {
            i32 val{}; // 存储的数值
            u32 size = 1; // 子树大小
            u32 prio = rng(); // 随机权重

            u32 lch{}, rch{}; // 左右子节点
        };

        std::deque<Node> tree;
        std::deque<u32> collected;

        u32 root = 0;

        auto alloc(i32 val) -> u32 {
            if (not collected.empty()) {
                auto res = collected.back();
                collected.pop_back();
                tree[res] = {val};
                return res;
            }
            tree.emplace_back(val);
            return tree.size() - 1;
        }
        auto free(u32 p) -> void {
            collected.emplace_back(p);
        }

        auto push_up(u32 p) -> void {
            tree[p].size = tree[tree[p].lch].size + tree[tree[p].rch].size + 1;
        }

        // 拆分成两棵平衡树，一个全部小于等于 pivot，一个全部大于，通过写引用参数输出
        auto split(i32 pivot, u32 p, u32 &leq, u32 &greater) -> void {
            if (p == 0) {
                leq = greater = 0;
                return;
            }
            if (tree[p].val <= pivot) leq = p, split(pivot, tree[p].rch, tree[p].rch, greater);
            else greater = p, split(pivot, tree[p].lch, leq, tree[p].lch);
            push_up(p);
        }

        // 保证 x 的最大元素，不超过 y 的最小元素
        auto merge(u32 p1, u32 p2) -> u32 {
            if (p1 == 0 or p2 == 0) return p1 | p2; // 神奇写法
            if (tree[p1].prio <= tree[p2].prio) {
                tree[p1].rch = merge(tree[p1].rch, p2);
                push_up(p1);
                return p1;
            } else {
                tree[p2].lch = merge(p1, tree[p2].lch);
                push_up(p2);
                return p2;
            }
        }
        auto merge(std::initializer_list<u32> const &ps) -> u32 {
            u32 res = 0;
            for (auto x: ps) res = merge(res, x);
            return res;
        }
        auto nth_element_impl(u32 index, u32 p) const -> i32 {
            auto left = tree[tree[p].lch].size;
            if (index < left) return nth_element_impl(index, tree[p].lch);
            else if (index == left) return tree[p].val;
            else return nth_element_impl(index - left - 1, tree[p].rch);
        }
    public:
        Treap() {
            tree.emplace_back(); // 1-index
            tree[0].size = 0;
        }

        auto insert(i32 val) -> void {
            u32 x{}, y = alloc(val), z{};
            split(val, root, x, z);
            root = merge({x, y, z});
        }

        auto erase(i32 val) -> void {
            u32 leq{}, greater{};
            u32 less{}, equal{};
            split(val, root, leq, greater);
            split(val - 1, leq, less, equal);
            root = merge({less, tree[equal].lch, tree[equal].rch, greater});
        }

        auto rank(i32 val) -> u32 {
            u32 less{}, geq_{};
            split(val - 1, root, less, geq_);
            auto ans = tree[less].size;
            root = merge(less, geq_);
            return ans;
        }

        auto get(u32 p) -> std::vector<i32> {
            std::vector<i32> res;
            auto &tree = this->tree;
            auto impl = [&](this auto impl, u32 p) -> void {
                if (p == 0) return;
                impl(tree[p].lch);
                res.emplace_back(tree[p].val);
                impl(tree[p].rch);
            };
            impl(p);
            return res;
        }
        auto print() -> void {
            for (auto x: get(root)) {
                std::cout << x << ' ';
            }
            std::cout << endl;
        }

        auto nth_element(u32 index) -> i32 {
            return nth_element_impl(index, root);
        }

        auto prev(i32 val) -> i32 {
            u32 less{}, geq_{};
            split(val - 1, root, less, geq_);
            auto ans = nth_element_impl(tree[less].size - 1, less);
            root = merge(less, geq_);
            return ans;
        }
        
        auto next(i32 val) -> i32 {
            u32 leq{}, greater{};
            split(val, root, leq, greater);
            auto ans = nth_element_impl(0, greater);
            root = merge(leq, greater);
            return ans;
        }
    };
    auto solve() -> void {
        i32 t{}; std::cin >> t;

        Treap set{};
        for (auto _ = t; _ --> 0; ) {
            i32 op{}, x{};
            std::cin >> op >> x;

            if (op == 1) set.insert(x);
            else if (op == 2) set.erase(x);
            else if (op == 3) std::cout << set.rank(x) + 1 << endl;
            else if (op == 4) std::cout << set.nth_element(x - 1) << endl;
            else if (op == 5) std::cout << set.prev(x) << endl;
            else std::cout << set.next(x) << endl;

            debug set.print();
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6314180276493067::solve();
    return 0;
}
