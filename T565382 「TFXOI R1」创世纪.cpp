/**
 * 
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
using ll = long long; using ull = unsigned long long;
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

using namespace lib;

using f32 = float;
using f64 = double;
using f80 = long double;

/**
 * 
 */
namespace Solution_8381487202141610 {
    f80 constexpr inf = __builtin_infl();
    f80 constexpr eps = 1e-5;
    class SegTreeValue {
        struct Node {
            i32 begin = -1, end = -1;
            f80 sum = 0, min = +inf, max = -inf;
            f80 mul_tag = 1, add_tag = 0;  // 先乘再加

            auto empty() const -> bool { return begin == -1 and end == -1; }
            auto modified() const -> bool {
                return mul_tag != 1 or add_tag != 0;
            }
            auto len() const -> i32 { return end - begin; }
            auto mul_add(f80 mul, f80 add) -> void {
                sum = sum * mul + add;
                min = min * mul + add;
                max = max * mul + add;
                if (mul < 0)  std::swap(min, max);
                mul_tag = mul_tag * mul;
                add_tag = add_tag * mul + add;
            }
            auto operator| (Node const &other) const -> Node {
                if (empty())  return other;
                if (other.empty())  return *this;
                Node res{begin, other.end};
                res.sum = sum + other.sum;
                res.min = std::min(min, other.min);
                res.max = std::max(max, other.max);
                return res;
            }
        };

        std::vector<Node> tree;

        auto constexpr static lson(i32 p) -> i32 { return p << 1; }
        auto constexpr static rson(i32 p) -> i32 { return p << 1 | 1; }

        auto pushUp(i32 p) -> void {
            tree[p] = tree[lson(p)] | tree[rson(p)];
        }
        auto pushDown(i32 p) -> void {
            if (tree[p].modified()) {
                for (auto s: {lson(p), rson(p)}) {
                    tree[s].mul_add(tree[p].mul_tag, tree[p].add_tag);
                }
                tree[p].add_tag = 0;
                tree[p].mul_tag = 1;
            }
        }
        auto build(i32 begin, i32 end, i32 p, auto const &init) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end) {
                f80 val = init[begin];
                tree[p].min = tree[p].max = tree[p].sum = val;
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p), init), build(mid, end, rson(p), init);
            pushUp(p);
        }
    public:
        SegTreeValue(i32 N, auto const &init): tree((N+1) << 2) {
            build(1, N+1, 1, init);
        }

        auto addRange(i32 begin, i32 end, f80 val, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].mul_add(1, val);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  addRange(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  addRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto mulRange(i32 begin, i32 end, f80 val, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].mul_add(val, 0);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  mulRange(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  mulRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto assignRange(i32 begin, i32 end, f80 val) -> void {
            mulRange(begin, end, 0);
            addRange(begin, end, val);
        }
        auto queryRange(i32 begin, i32 end, i32 p = 1) -> Node {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p];
            }
            pushDown(p);
            Node res;
            if (tree[lson(p)].end > begin)  res = res | queryRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  res = res | queryRange(begin, end, rson(p));
            return res;
        }
    };
    /**
     * 维护序列的升降
     * 对于当前序列的 a[i] 和 a[i+1]，b[i] = 
     * 1, a[i] < a[i+1]
     * 0, a[i] = a[i+1]
     * -1, a[i] > a[i+1]
     * 
     * 峰和谷：形如 +++--- / ---+++
     * 合并？
     * 似乎需要维护每个区间两侧的：连续 +，连续 +- 连续 -，连续 -+ 长度
     * 以及各自的峰谷数量。
     * 
     * cnt 需要使用 int64
     * 单点赋值、区间反转
     */
    class SegTreeOther {
        struct Node {
            i32 begin = -1, end = -1;
            struct Lens {
                i32 len1 = 0, len2 = 0;  // 第一段和第二段连续段的长度（第二连续段不为 0）
                i32 value = 0;  // 第一段连续段的类型
            } left {}, right {};

            i64 top = 0, bottom = 0;
            bool neg_tag = false;
            bool zero_tag = false;

            auto empty() const -> bool { return begin == -1 and end == -1; }
            auto len() const -> i32 { return end - begin; }
            auto operator| (Node const &other) const -> Node {
                if (empty())  return other;
                if (other.empty())  return *this;
                Node res{begin, other.end};
                
                // 拼接两个区间
                // 先考虑 ---++++ +++---- 这样
                if (right.value == other.left.value and right.value != 0) {
                    std::array<i64, 3> segs;
                    segs[0] = right.len2, segs[1] = right.len1 + other.left.len1, segs[2] = other.left.len2;
                    i64 append_b = segs[0] * segs[1];  // 这次新增的谷
                    i64 append_t = segs[1] * segs[2];  // 新增的峰
                    i64 extra_b = static_cast<i64>(right.len2) * right.len1;
                    i64 extra_t = static_cast<i64>(other.left.len1) * other.left.len2;
                    if (right.value == -1)  std::swap(append_b, append_t), std::swap(extra_b, extra_t);

                    // 计算新增的贡献
                    res.top = top + other.top + append_t - extra_t;
                    res.bottom = bottom + other.bottom + append_b - extra_b;
                }
                // +++++ --- 这样
                if (right.value != other.left.value and right.value + other.left.value == 0) {  // 恰好相反
                    i64 append_t = static_cast<i64>(right.len1) * other.left.len1;
                    i64 append_b = 0;

                    if (right.value == -1)  std::swap(append_b, append_t);
                    res.top = top + other.top + append_t;
                    res.bottom = bottom + other.bottom + append_b;
                }

                // 统计连续段
                res.left = left, res.right = other.right;
                if (left.len1 == len()) {  // 左侧贯穿区间
                    assert(res.left.len2 == 0);
                    if (res.left.value == other.left.value) {
                        res.left.len1 += other.left.len1;
                        res.left.len2 += other.left.len2;
                    } else {
                        res.left.len2 += other.left.len1;
                    }
                } 
                if (other.right.len1 == other.len()) {  // 右侧贯穿区间
                    assert(res.right.len2 == 0);
                    if (res.right.value == right.value) {
                        res.right.len1 += right.len1;
                        res.right.len2 += right.len2;
                    } else {
                        res.right.len2 += right.len1;
                    }
                }
                if (left.len1 + left.len2 == len() and left.len2 != 0) {  // 左侧第二段贯穿区间
                    assert(left.value != 0);
                    if (res.left.value + other.left.value == 0) {  // 和第一段的反色相同
                        res.left.len2 += other.left.len1;
                    }
                }
                if (other.right.len1 + other.right.len2 == other.len() and other.right.len2 != 0) {  // 右侧第二段贯穿区间
                    assert(other.right.value != 0);
                    if (res.right.value + right.value == 0) {
                        res.right.len2 += right.len1;
                    }
                }
                return res;
            }

            auto negate() -> void {
                std::swap(top, bottom);
                left.value = -left.value;
                right.value = -right.value;
                neg_tag = not neg_tag;
            }
            auto zero() -> void {
                top = bottom = 0;
                left.value = right.value = 0;
                left.len1 = right.len1 = 0;
                left.len2 = right.len2 = 0;
                zero_tag = true;
            }
        };
        std::vector<Node> tree;
        auto constexpr static lson(i32 p) -> i32 { return p << 1; }
        auto constexpr static rson(i32 p) -> i32 { return p << 1 | 1; }
        auto pushUp(i32 p) -> void {
            tree[p] = tree[lson(p)] | tree[rson(p)];
        }
        auto pushDown(i32 p) -> void {
            if (tree[p].neg_tag) {
                for (auto s: {lson(p), rson(p)}) {
                    tree[s].negate();
                }
                tree[p].neg_tag = false;
            }
            if (tree[p].zero_tag) {
                for (auto s: {lson(p), rson(p)}) {
                    tree[s].negate();
                }
                tree[p].zero_tag = false;
            }
        }
        auto build(i32 begin, i32 end, i32 p, auto const &init) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end) {
                auto val = init[begin];
                if (val != 0) {
                    tree[p].left.value = tree[p].right.value = val;
                    tree[p].left.len1 = tree[p].right.len1 = 1;
                }
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p), init), build(mid, end, rson(p), init);
            pushUp(p);
        }
    public:
        SegTreeOther(i32 N, auto const &init): tree((N+1) << 2) {
            build(1, N+1, 1, init);
        }
        auto assignAt(i32 pos, i32 val, i32 p = 1) -> void {
            if (tree[p].begin + 1 == tree[p].end) {
                tree[p].left.value = tree[p].right.value = val;
                tree[p].left.len1 = tree[p].right.len1 = (val != 0);
                assert(tree[p].left.len2 == 0);
                assert(tree[p].right.len2 == 0);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > pos)  assignAt(pos, val, lson(p));
            else  assignAt(pos, val, rson(p));
            pushUp(p);
        }
        auto negateRange(i32 begin, i32 end, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].negate();
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  negateRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  negateRange(begin, end, rson(p));
            pushUp(p);
        }
        auto zeroRange(i32 begin, i32 end, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].zero();
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  zeroRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  zeroRange(begin, end, rson(p));
            pushUp(p);
        }
        auto queryRange(i32 begin, i32 end, i32 p = 1) -> Node {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p];
            }
            pushDown(p);
            Node res;
            if (tree[lson(p)].end > begin)  res = res | queryRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  res = res | queryRange(begin, end, rson(p));
            return res;
        }
        auto print() -> void {
            for (auto x: tree) {
                if (not x.empty()) {
                    std::cout << std::format("[{}, {}) left = {{{}, {}, {}}} right = {{{}, {}, {}}} t = {} b = {} neg = {} zero = {}", x.begin, x.end, x.left.len1, x.left.len2, x.left.value, x.right.len1, x.right.len2, x.right.value, x.top, x.bottom, x.neg_tag, x.zero_tag) << std::endl;
                }
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<f80> init(N+1);
        for (auto &x: init | views::drop(1))  std::cin >> x;

        auto sign = [](f80 x) -> i32 {
            if (std::abs(x) < eps)  return 0;
            if (x > 0)  return 1;
            return -1;
        };

        std::vector<i32> diff(N+1);
        for (i32 i = 1; i < N; i++) {
            diff[i] = sign(init[i + 1] - init[i]);
        }

        SegTreeValue sgt_v{N, init};
        SegTreeOther sgt_diff{N, diff};
        while (M --> 0) {
            auto add = [&](i32 l, i32 r, f80 v) -> void {
                auto cur = sgt_v.queryRange(l, r + 1);
                if (cur.max + v > 1e9 or cur.min + v < -1e9)  return;
                sgt_v.addRange(l, r + 1, v);
                if (l > 1) {
                    auto l_sign = sign(sgt_v.queryRange(l, l + 1).sum - sgt_v.queryRange(l - 1, l).sum);
                    sgt_diff.assignAt(l - 1, l_sign);
                }
                if (r < N) {
                    auto r_sign = sign(sgt_v.queryRange(r + 1, r + 2).sum - sgt_v.queryRange(r, r + 1).sum);
                    sgt_diff.assignAt(r, r_sign);
                }
            };
            auto mul = [&](i32 l, i32 r, f80 v) -> void {
                auto cur = sgt_v.queryRange(l, r + 1);
                if (std::abs(cur.max * v) > 1e9 or std::abs(cur.min * v) > 1e9)  return;
                sgt_v.mulRange(l, r + 1, v);
                if (l != r) {
                    if (std::abs(v) < eps)  sgt_diff.zeroRange(l, r);
                    else if (v < 0)  sgt_diff.negateRange(l, r);
                }
                if (l > 1) {
                    auto l_sign = sign(sgt_v.queryRange(l, l + 1).sum - sgt_v.queryRange(l - 1, l).sum);
                    sgt_diff.assignAt(l - 1, l_sign);
                }
                if (r < N) {
                    auto r_sign = sign(sgt_v.queryRange(r + 1, r + 2).sum - sgt_v.queryRange(r, r + 1).sum);
                    sgt_diff.assignAt(r, r_sign);
                }
            };
            i32 op, l, r;  std::cin >> op >> l >> r;
            if (op == 0) {
                if (l == r) {
                    std::cout << 0 << " " << 0 << std::endl;
                } else {
                    auto diff_res = sgt_diff.queryRange(l, r);
    
                    auto ans_t = diff_res.top, ans_b = diff_res.bottom;
                    std::cout << ans_t << " " << ans_b << std::endl;
                    debug  sgt_diff.print();
                }
            } else if (op == 1) {
                f80 v;  std::cin >> v;
                add(l, r, v);
            } else if (op == 2) {
                f80 v;  std::cin >> v;
                mul(l, r, v);
            } else if (op == 3) {
                f80 v;  std::cin >> v;
                mul(l, r, 0);
                add(l, r, v);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8381487202141610::solve();
    return 0;
}
