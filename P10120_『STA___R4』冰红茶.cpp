/**
 * @link https://www.luogu.com.cn/problem/P10120
 */
#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if (DEBUG_MODE)
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
using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::uint16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
#ifndef LIB_NULL_NUMBER
#define LIB_NULL_NUMBER
#include <cmath>
#include <iostream>
#include <limits>
#include <type_traits>

namespace lib {
    /**
     * 提供“可空”的数字类型。
     * 具体地，会在每个类型中去除一些值作为空值，判断是否为指定的空值。
     * 有符号整数：最小值
     * 无符号整数：最大值
     * 浮点数：NaN
     * 
     * 如同从值域中删除了对应值。如果有效值和保留的空值相同，则行为未定义。
     */
    namespace NullNumber {
        // 主模板触发编译错误
        template <typename T, typename Enable = void>
        struct InvalidValueOf {
            static_assert(sizeof(T) == 0, "Unsupported type");
        };

        // 浮点特化：返回 NaN
        template <typename T>
        struct InvalidValueOf<T, std::enable_if_t<std::is_floating_point<T>::value>> {
            static constexpr T value = std::numeric_limits<T>::quiet_NaN();
        };

        // 有符号整数特化：返回最小值
        template <typename T>
        struct InvalidValueOf<T, std::enable_if_t<
            std::is_integral<T>::value && 
            std::is_signed<T>::value
        >> {
            T static constexpr value = std::numeric_limits<T>::min();
        };

        // 无符号整数特化（排除 bool）：返回最大值
        template<typename T>
        struct InvalidValueOf<T, std::enable_if_t<
            std::is_integral<T>::value && 
            std::is_unsigned<T>::value && 
            !std::is_same<T, bool>::value
        >> {
            T static constexpr value = std::numeric_limits<T>::max();
        };

        // NullType 类支持隐式转换
        class NullType {
        public:
            template <typename T>
            constexpr operator T() const {
                return InvalidValueOf<T>::value;
            }
        };

        // 全局 null 常量
        NullType static constexpr null;

        // 判断类型是否支持空值
        template<typename T>
        struct IsNullable: std::integral_constant<bool,
            std::is_arithmetic<T>::value && 
            !std::is_same<T, bool>::value
        > {};

        // 重载 == 运算符（支持浮点 NaN 比较）
        template <typename T>
        auto operator== (T val, NullType) -> std::enable_if_t<IsNullable<T>::value && std::is_floating_point<T>::value, bool> {
            return std::isnan(val);
        }
        template <typename T>
        auto operator== (T val, NullType) -> std::enable_if_t<IsNullable<T>::value && std::is_integral<T>::value, bool> {
            return val == InvalidValueOf<T>::value;
        }
        // 重载 != 运算符
        template <typename T> auto operator!= (T val, NullType) -> std::enable_if_t<IsNullable<T>::value, bool> {
            return !(val == null);
        }
        // 删除不可用类型的运算符
        template <typename T> auto operator== (T, NullType) -> std::enable_if_t<!IsNullable<T>::value, void> = delete;
        template <typename T> auto operator!= (T, NullType) -> std::enable_if_t<!IsNullable<T>::value, void> = delete;
    }
    
    using NullNumber::null;
}
#endif  // not def LIB_NULL_NUMBER
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
