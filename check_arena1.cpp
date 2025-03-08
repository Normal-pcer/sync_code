/**
 * @link https://www.luogu.com.cn/problem/P11833
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
#include "./libs/fixed_int.hpp"
#include "./libs/io.hpp"

using namespace lib;

struct Box {
    i32 start, target;
    i64 time;
};

/**
 * 箱子到达目标地点的时间按照 t 排序。
 * 
 * 推动一个箱子，需要把若干个碍事的箱子挪走。
 * 这一次操作会使得所有被直接或间接移动的箱子位置变为连续自然数。
 * 
 * 考虑使用线段树维护，将 i 处的点赋值为 i，可以求出所有箱子移动的总距离。
 * 然后需要支持区间赋值成连续自然数，有点奇怪，但也不是不能做。
 * 值域有点大，动态开点即可。
 * 
 * 先只考虑箱子向右移动
 * 希望将一个箱子从 start 推到 target
 * 如果这个区间内有其他的箱子，把它们一起挤过去。
 * 最后这些受到影响箱子的位置一定会从 target 开始形成一个连续的自然数列。
 * [start, target] 内部有 cnt0 个箱子，它们推完之后移动到 [target, target + cnt0)。
 * 但是如果目标区间还存在箱子，需要继续向后移动。
 * 寻找一个 cnt1，使得 [target + cnt1) 恰好还能容纳 cnt0 个箱子。
 * 观察这 cnt1 个箱子的初末状态可以获取总位移。
 * 移动完成的需要在线段树中删除。为什么呢？例如：
 * ......###
 * .....|----->|
 * 第一个箱子是固定的，箭头为第二个箱子希望移动的轨迹。如果希望推动第二个箱子，并不用在乎第一个箱子。
 * 如果这个区间内没有箱子，说明一定有其他的箱子把自己推跑了，无解。
 * 
 * 等一下，为什么非要这么维护？
 * 所有箱子之间的相对位置关系一定是不变的。第 i 项表示从左到右第 i 个箱子的当前位置。
 * 这样就不需要那些乱七八槽的删除操作，只需要弄一个区间赋值成等差数列就行。
 * 同时只有 N 项也不需要动态开点。
 * 
 * 
 * 总结一下，线段树需要的操作是区间求和以及区间赋值为连续自然数，还有单点赋值为指定值。
 */
namespace Solution_2812031851177643_1 {
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lowerBoundValue(T begin, T end, U x, Func &&f, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (cmp(f(mid), x))  begin = mid + 1;
            else  end = mid;
        }
        return begin;
    }
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto upperBoundValue(T begin, T end, U x, Func &&f, Comp &&cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = std::midpoint(begin, end);
            if (not cmp(x, f(mid)))  begin = mid + 1;
            else  end = mid;
        }
        return begin;
    }

    class SegTree {
        struct Node {
            // 记 begin = x 的叶子节点上对应的原值为 f(x)
            i32 begin = 0, end = 0;
            i64 sum = 0;
            i32 max = 0;  // f(x) 的最大值
            i32 max_minus_index = 0;  // g(x) = f(x) - x 的最大值
            i32 iota_tag = -inf;  // 不为负无穷：赋值为 tag 到 tag + len - 1 的连续自然数

            auto constexpr len() const -> i32 { return end - begin; }
            auto constexpr iota(i32 x) -> void {
                iota_tag = x;
                sum = static_cast<i64>(x + (x + len() - 1)) * len() / 2;
                max = x + len() - 1;
                max_minus_index = x - begin;
            }
        };
        i32 N;
        std::vector<Node> tree;
        auto static constexpr lson(i32 x) -> i32 { return x << 1; }
        auto static constexpr rson(i32 x) -> i32 { return x << 1 | 1; }

        auto pushUp(i32 p) -> void {
            tree[p].sum = tree[lson(p)].sum + tree[rson(p)].sum;
            tree[p].max = std::max(tree[lson(p)].max, tree[rson(p)].max);
            tree[p].max_minus_index = std::max(tree[lson(p)].max_minus_index, tree[rson(p)].max_minus_index);
        }
        auto pushDown(i32 p) -> void {
            if (tree[p].iota_tag != -inf) {
                tree[lson(p)].iota(tree[p].iota_tag);
                tree[rson(p)].iota(tree[p].iota_tag + tree[lson(p)].len());
                tree[p].iota_tag = -inf;
            }
        }
        auto build(i32 begin, i32 end, i32 p) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(i32 N): N(N), tree((N + 1) << 2) {
            build(0, N + 1, 1);
        }

        auto sumRange(i32 begin, i32 end, i32 p = 1) -> i64 {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p].sum;
            }
            pushDown(p);
            i64 res = 0;
            if (tree[lson(p)].end > begin)  res += sumRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  res += sumRange(begin, end, rson(p));
            return res;
        }
        auto iotaRange(i32 begin, i32 end, i32 val, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].iota(val + tree[p].begin - begin);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  iotaRange(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  iotaRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto assignAt(i32 pos, i32 val, i32 p = 1) -> void {
            if (tree[p].begin + 1 == tree[p].end) {
                tree[p].sum = val;
                tree[p].max = val;
                tree[p].max_minus_index = val - pos;
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > pos)  assignAt(pos, val, lson(p));
            else  assignAt(pos, val, rson(p));
            pushUp(p);
        }
        auto string() -> std::string {
            std::stringstream ss;
            for (i32 i = 0; i < N; i++) {
                ss << "[" << i << "]" << sumRange(i, i + 1) << " ";
            }
            return ss.str();
        }
        // 寻找 [begin, end) 区间内的第一个 x 使得 f(x) >= val
        // 不存在这样的 x，返回 end
        auto lowerBound(i32 begin, i32 end, i32 val, i32 p = 1) -> i32 {
            debug  std::cout << std::format("lowerBound {} {} {}", begin, end, val) << std::endl;
            if (tree[p].begin + 1 == tree[p].end) {
                if (tree[p].max >= val)  return tree[p].begin;
                else  return end;
            }

            pushDown(p);
            if (tree[lson(p)].end > begin and tree[lson(p)].max >= val) {
                auto find = lowerBound(begin, end, val, lson(p));
                if (find != end)  return find;
            }
            if (tree[rson(p)].begin < end and tree[rson(p)].max >= val) {
                auto find = lowerBound(begin, end, val, rson(p));
                if (find != end)  return find;
            }
            return end;
        }

        // 寻找 [begin, end) 区间内的第一个 x 使得 f(x) - x >= val
        // 不存在这样的 x，返回 end
        auto lowerBoundMinusIndex(i32 begin, i32 end, i32 val, i32 p = 1) -> i32 {
            if (tree[p].begin + 1 == tree[p].end) {
                if (tree[p].max_minus_index >= val)  return tree[p].begin;
                else  return end;
            }

            pushDown(p);
            if (tree[lson(p)].end > begin and tree[lson(p)].max_minus_index >= val) {
                auto find = lowerBoundMinusIndex(begin, end, val, lson(p));
                if (find != end)  return find;
            }
            if (tree[rson(p)].begin < end and tree[rson(p)].max_minus_index >= val) {
                auto find = lowerBoundMinusIndex(begin, end, val, rson(p));
                if (find != end)  return find;
            }
            return end;
        }
    };
    
    auto solve() -> void {
        i32 N;  io >> N;
        std::vector<Box> boxes(N);
        for (auto &[x, y, z]: boxes) {
            io >> x >> y >> z;
        }
        ranges::sort(boxes, std::less{}, lam(x, x.time));

        std::vector box_pos_collection(boxes);
        auto box_pos_cmp = lam(x, y, x.start < y.start);
        ranges::sort(box_pos_collection, box_pos_cmp);  // 按照初始位置排序
        std::vector<i32> index(N);
        for (i32 i = 0; i < N; i++) {
            auto it = ranges::lower_bound(box_pos_collection, boxes[i], box_pos_cmp);
            auto this_index = std::distance(box_pos_collection.begin(), it);
            index[i] = this_index;
        }
#define __POS sgt.string()
        SegTree sgt{N};
        void(sgt.string());
        for (i32 i = 0; i < N; i++) {
            sgt.assignAt(i, box_pos_collection[i].start);
        }

        auto ans = [&]() -> bool {
            i64 cur = 0;
            for (i32 i = 0; i < N; i++) {
                auto &box = boxes[i];
                auto right = [&]() -> bool {
                    auto cur_pos = sgt.sumRange(index[i], index[i] + 1);  // 当前箱子的位置
                    if (cur_pos > box.target)  return false;  // 已经被移动走
                    
                    // 感觉似乎可以保证 cur_pos 到 target 没有固定的箱子
                    // auto _after_target = upperBoundValue(index[i], N, box.target, lam(x, sgt.sumRange(x, x + 1)));
                    auto after_target = sgt.lowerBound(index[i], N, box.target + 1);
                    // assert(_after_target == after_target);

                    auto cnt0 = after_target - index[i];
                    // 寻找合适的地方安置这些箱子
                    // auto _bind_until = upperBoundValue(after_target, N + 1, cnt0, [&](i32 x) {  // [index[i], bind_until) 需要绑定到一起赋值
                    //     auto occupied = x - after_target;  // 已经被占用
                    //     auto total = (x == N? infLL: sgt.sumRange(x, x + 1)) - box.target;
                    //     return total - occupied;
                    // });
                    auto bind_until = sgt.lowerBoundMinusIndex(after_target, N, cnt0 - after_target + box.target + 1);
                    // assert(_bind_until == bind_until);
                    auto old_pos_sum = sgt.sumRange(index[i], bind_until);
                    sgt.iotaRange(index[i], bind_until, box.target);
                    auto new_pos_sum = sgt.sumRange(index[i], bind_until);
                    cur += new_pos_sum - old_pos_sum;
                    return true;
                };
                auto left = [&]() -> bool {
                    auto cur_pos = sgt.sumRange(index[i], index[i] + 1);  // 当前箱子
                    if (cur_pos < box.target)  return false;

                    // auto before_target = lowerBoundValue(0, index[i], box.target, lam(x, sgt.sumRange(x, x + 1))) - 1;
                    auto before_target = sgt.lowerBound(0, index[i], box.target) - 1;
                    auto cnt0 = index[i] - before_target;

                    // (bind_after, index[i]]
                    auto bind_after = lowerBoundValue(0, before_target + 1, cnt0, [&](i32 x) {
                        auto occupied = before_target - x;
                        auto total = box.target - sgt.sumRange(x, x + 1);
                        return total - occupied;
                    }, std::greater{}) - 1;

                    auto old_pos_sum = sgt.sumRange(bind_after + 1, index[i] + 1);
                    sgt.iotaRange(bind_after + 1, index[i] + 1, box.target + 1 - index[i] + bind_after);
                    auto new_pos_sum = sgt.sumRange(bind_after + 1, index[i] + 1);
                    cur -= new_pos_sum - old_pos_sum;
                    return true;
                };
                bool can_continue = true;
                if (box.start <= box.target)  can_continue = right();
                else  can_continue = left();
                if (not can_continue)  return false;
                if (cur > box.time)  return false;
            }
            return true;
        }();

        io << (ans? "Yes": "No") << endl;
    }
}

int main(int dyy_forever, char const *lmy_forever[]) {
    DEBUG_MODE = (dyy_forever-1) and not strcmp("-d", lmy_forever[1]);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 C, T;  io >> C >> T;
    while (T --> 0) {
        Solution_2812031851177643_1::solve();
    }
    return 0;
}

