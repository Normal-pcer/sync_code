/**
 * @link https://www.luogu.com.cn/problem/P10120
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/null_number.hpp"
using namespace lib;

/*
定义“lcnt[i]”表示，一个人机，最后一次连续喝了几瓶 i 口味的冰红茶
例如，一个喝茶序列如下：
1110001111 lcnt[1] = 4, lcnt[0] = 0

使用线段树维护以下信息：
- alive 存活的人机数量
- max[2]：
    - 叶子节点，表示这个人机的 lcnt[i]。
    - 否则，表示这个区间下的最大值。
- assign_tag[2]：赋值
- add_tag[2]：加法

两个标签，先赋值再加。
*/
namespace Solution_6974121065486076 {
    using Flavor = i32;
    class SegTree {
        struct Node {
            i32 begin{}, end{};
            i32 alive{1};
            std::array<i64, 2> max{};
            std::array<i64, 2> assign_tag{};
            std::array<i64, 2> add_tag{};

            Node() {
                assign_tag.fill(null);
                add_tag.fill(0);
            }

            // 先赋值，再加
            auto assignAdd(Flavor index, i64 assign, i64 add) -> void {
                if (assign != null) {
                    max[index] = assign;
                    assign_tag[index] = assign;
                    add_tag[index] = 0;
                }
                if (add != 0) {
                    max[index] += add;
                    add_tag[index] += add; 
                }
            }
        };

        std::vector<Node> tree;
        auto static constexpr lch(i32 p) -> i32 { return p << 1; }
        auto static constexpr rch(i32 p) -> i32 { return p << 1 | 1; }

        auto pushUp(i32 p) -> void {
            for (i32 i: {0, 1}) {
                tree[p].max[i] = std::max(tree[lch(p)].max[i], tree[rch(p)].max[i]);
            }
            tree[p].alive = tree[lch(p)].alive + tree[rch(p)].alive;
        }

        auto pushDown(i32 p) -> void {
            for (i32 i: {0, 1}) {
                for (i32 ch: {lch(p), rch(p)}) {
                    tree[ch].assignAdd(i, tree[p].assign_tag[i], tree[p].add_tag[i]);
                }
            }
            tree[p].assign_tag.fill(null);
            tree[p].add_tag.fill(0);
        }

        auto build(i32 begin, i32 end, i32 p) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (end - begin == 1) return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lch(p)), build(mid, end, rch(p));
            pushUp(p);
        }
    public:
        SegTree(i32 n): tree(std::bit_ceil(u32(n)) * 2) {
            build(0, n, 1);
        }

        auto updateRange(i32 begin, i32 end, Flavor fla, i64 assign, i64 add, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].assignAdd(fla, assign, add);
                return;
            }
            pushDown(p);
            if (tree[lch(p)].end > begin) updateRange(begin, end, fla, assign, add, lch(p));
            if (tree[rch(p)].begin < end) updateRange(begin, end, fla, assign, add, rch(p));
            pushUp(p);
        }

        auto queryAlive() -> i64 {
            return tree[1].alive;
        }

        // 杀死范围内所有 max >= limit 的人机
        auto killRange(i32 begin, i32 end, i64 limit, i32 p = 1) -> void {
            if (tree[p].alive == 0) return;
            if (tree[p].max[0] < limit and tree[p].max[1] < limit) return;
            if (tree[p].end - tree[p].begin == 1) {
                tree[p].alive = 0;
                return;
            }
            pushDown(p);
            if (tree[lch(p)].end > begin) killRange(begin, end, limit, lch(p));
            if (tree[rch(p)].begin < end) killRange(begin, end, limit, rch(p));
            pushUp(p);
        }
    };
    auto solve() -> void {
        i32 n{}, q{};
        std::cin >> n >> q;

        SegTree sgt{n};

        for (auto _ = q; _ --> 0; ) {
            i32 op{}, l{}, r{}, k{};
            std::cin >> op;
            if (op == 3) {
                std::cout << sgt.queryAlive() << endl;
            } else {
                std::cin >> l >> r >> k;
                l--;

                if (op == 1) {
                    // 区间内喝 0，区间外喝 1
                    sgt.updateRange(l, r, 0, null, k);
                    sgt.updateRange(l, r, 1, 0, 0);
                    if (0 != l) sgt.updateRange(0, l, 0, 0, 0);
                    if (0 != l) sgt.updateRange(0, l, 1, null, k);
                    if (r != n) sgt.updateRange(r, n, 0, 0, 0);
                    if (r != n) sgt.updateRange(r, n, 1, null, k);
                } else {
                    sgt.killRange(l, r, k);
                }
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6974121065486076::solve();
    return 0;
}
