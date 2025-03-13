/**
 * @link https://www.luogu.com.cn/problem/P2894
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 每次查询最左侧的、长度大于等于 x 的连续段。
 * 可以假设用一个数组来维护从 x 开始向右最长的连续段长度。
 * 可以发现，对这个数组的修改是非常规律的。
 * 入住房间：部分房间变为 0，部分房间减去一个数。
 * 离开房间：部分房间变为连续自然数，部分房间加上一个数。
 * 于是可以使用线段树维护这个数组。
 * 为了查询第一个足够大的值，还需要维护区间最大值。
 * 
 * 实际上我是令这个数组表示从 x 向左的连续段，本质相同。
 * 因为线段树的 iota 操作写的是填充上升自然数。
 * 
 * 还有一个问题是需要找一个点之后的第一个 0。
 * 再写一个二分即可。
 */
namespace Solution_4455511570693051 {
    class SegTree {
        struct Node {
            i32 begin, end;
            i32 max = -inf;         // 区间最大值
            i32 min = inf;          // 区间最小值
            i32 assign_tag = -inf;  // 如果不是 -inf 则为区间赋值为 tag
            i32 iota_tag = -inf;    // 如果不是 -inf 则让下标为 i 的数变为 tag + i
            i32 add_tag = 0;        // 加上 tag
            // 需要保证 iota_tag 和 assign_tag 不共存。
            // 下传标记时，对子区间先 iota / assign 再 add

            auto assign(i32 x) {
                assign_tag = max = min = x;
                iota_tag = -inf;
                add_tag = 0;
            }
            auto iota(i32 x) {
                iota_tag = x;
                max = x + end - 1;
                min = x + begin;
                assign_tag = -inf;
                add_tag = 0;
            }
            auto add(i32 x) {
                add_tag += x;
                max += x;
                min += x;
            }
        };
        std::vector<Node> tree;
        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }
        
        auto pushUp(i32 p) -> void {
            tree[p].max = std::max(tree[lson(p)].max, tree[rson(p)].max);
            tree[p].min = std::min(tree[lson(p)].min, tree[rson(p)].min);
        }
        auto pushDown(i32 p) -> void {
            assert(tree[p].assign_tag == -inf or tree[p].iota_tag == -inf);
            for (auto s: {lson(p), rson(p)}) {
                if (tree[p].assign_tag != -inf) {
                    tree[s].assign(tree[p].assign_tag);
                }
                if (tree[p].iota_tag != -inf) {
                    tree[s].iota(tree[p].iota_tag);
                }
                if (tree[p].add_tag != 0) {
                    tree[s].add(tree[p].add_tag);
                }
            }
            tree[p].assign_tag = -inf;
            tree[p].iota_tag = -inf;
            tree[p].add_tag = 0;
        }
        auto build(i32 begin, i32 end, i32 p) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end) {
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p));
            build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(i32 N): tree(N << 2) {
            build(0, N, 1);
        }
        auto end() const -> i32 {
            return tree[1].end;
        }
        auto assignRange(i32 begin, i32 end, i32 val, i32 p = 1) -> void {
            if (begin == end)  return;
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].assign(val);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  assignRange(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  assignRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto iotaRange(i32 begin, i32 end, i32 val, i32 p = 1) -> void {
            if (begin == end)  return;
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].iota(val);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  iotaRange(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  iotaRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto addRange(i32 begin, i32 end, i32 val, i32 p = 1) -> void {
            if (begin == end)  return;
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].add(val);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  addRange(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  addRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto maxRange(i32 begin, i32 end, i32 p = 1) -> i32 {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p].max;
            }
            i32 res = -inf;
            pushDown(p);
            if (tree[lson(p)].end > begin)  chkMax(res, maxRange(begin, end, lson(p)));
            if (tree[rson(p)].begin < end)  chkMax(res, maxRange(begin, end, rson(p)));
            return res;
        }

        /**
         * 设 x 点的对应值为 f(x)
         * 在节点 p 上查询第一个 x，满足 f(x) >= val。
         * 如果不存在，返回这个节点的 end 值。
         */
        auto findFirstGreaterEqual(i32 val, i32 p = 1) -> i32 {
            if (tree[p].begin + 1 == tree[p].end) {
                if (tree[p].max >= val)  return tree[p].begin;
                else  return tree[p].end;
            }
            pushDown(p);
            if (tree[lson(p)].max >= val) {
                auto res = findFirstGreaterEqual(val, lson(p));
                if (res != tree[lson(p)].end)  return res;
            }
            if (tree[rson(p)].max >= val) {
                auto res = findFirstGreaterEqual(val, rson(p));
                if (res != tree[rson(p)].end)  return res;
            }
            return tree[p].end;
        }
        /**
         * 找到第一个 x 满足：
         * x > x0
         * f(x) <= val
         */
        auto findFirstLessEqualAfter(i32 x0, i32 val, i32 p = 1) -> i32 {
            if (tree[p].begin + 1 == tree[p].end) {
                if (tree[p].min <= val)  return tree[p].begin;
                else  return tree[p].end;
            }
            pushDown(p);
            if (tree[lson(p)].min <= val and tree[lson(p)].end > x0) {
                auto res = findFirstLessEqualAfter(x0, val, lson(p));
                if (res != tree[lson(p)].end)  return res;
            }
            if (tree[rson(p)].min <= val) {
                auto res = findFirstLessEqualAfter(x0, val, rson(p));
                if (res != tree[rson(p)].end)  return res;
            } 
            return tree[p].end;
        }
        auto friend operator<< (std::ostream &os, SegTree &sgt) -> std::ostream & {
            for (i32 i = 0; i != sgt.end(); i++) {
                if (i != 0)  os << " ";
                os << sgt.maxRange(i, i + 1);
            }
            return os;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        // 想象维护一个 space 数组表示从 i 向左的最大空闲。
        // 在 space 数组上架设一棵线段树。
        SegTree sgt{N};

        // 最开始的空间是 1, 2, 3, ...
        sgt.iotaRange(0, N, 1);

        debug  std::cout << sgt << endl;
        for (auto m = M; m --> 0; ) {
            i32 op;  std::cin >> op;
            if (op == 1) {
                i32 x;  std::cin >> x;
                // 查询房间
                auto pos = sgt.findFirstGreaterEqual(x);
                if (pos == sgt.end()) {
                    std::cout << 0 << endl;
                } else {
                    std::cout << (pos + 1 - x + 1) << endl;  // 转成 1-index 额外 +1

                    auto next_zero = sgt.findFirstLessEqualAfter(pos + 1, 0);
                    auto decrease_begin = pos + 1;
                    sgt.iotaRange(decrease_begin, next_zero, 1 - decrease_begin);
                    sgt.assignRange(pos + 1 - x, pos + 1, 0);
                }
            } else {
                i32 x, y;  std::cin >> x >> y;
                x--;  // 转换为 0-index
                y = x + y;  // 转换为 end 位置而不是长度
                // 清空房间
                // 找到这个区间的第一个 0 以及之后的第一个 0
                auto first_zero = sgt.findFirstLessEqualAfter(x, 0);
                auto last_zero = sgt.findFirstLessEqualAfter(y, 0);

                // 这一段区间可以被批量修改
                auto first_val = sgt.maxRange(first_zero - 1, first_zero) + 1;
                sgt.iotaRange(first_zero, last_zero, first_val - first_zero);
            }
            debug  std::cout << sgt << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4455511570693051::solve();
    return 0;
}
