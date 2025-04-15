/**
 * @link https://www.luogu.com.cn/problem/P3380
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"

#ifndef INCLUDED_INPLACE_VECTOR
#define INCLUDED_INPLACE_VECTOR
template <typename T, std::size_t MaxSize>
class InplaceVector {
    alignas(alignof(T)) std::array<std::byte, MaxSize * sizeof(T)> data_;
    std::size_t size_ = 0;

public:
    using Iterator = T *;
    using ConstIterator = T const *;

    InplaceVector() {}
    explicit InplaceVector(std::size_t n): size_(n) {
        auto ptr = reinterpret_cast<T *>(data_.data());
        for (std::size_t i = 0; i != n; ++i, ++ptr) {
            new(ptr) T;
        }
    }
    InplaceVector(std::size_t n, T const &value): size_(n) {
        auto ptr = reinterpret_cast<T *>(data_.data());
        for (std::size_t i = 0; i != n; ++i, ++ptr) {
            new(ptr) T(value);
        }
    }
    ~InplaceVector() {
        auto ptr = reinterpret_cast<T *>(data_.data());
        for (std::size_t i = 0; i != size_; ++i, ++ptr) {
            ptr->~T();
        }
    }
    
    auto size() const -> std::size_t { return size_; }
    auto data() const -> T * { return reinterpret_cast<T *>(data_.data()); }
    auto begin() -> Iterator { return reinterpret_cast<T *>(data_.data()); }
    auto begin() const -> ConstIterator { return reinterpret_cast<T *>(data_.data()); }
    auto end() -> Iterator { return reinterpret_cast<T *>(data_.data()) + size(); }
    auto end() const -> ConstIterator { return reinterpret_cast<T *>(data_.data()) + size(); }

    auto at(std::size_t index) const -> T const & {
        if (index >= size()) {
            throw std::out_of_range{
                std::format("InplaceVector access out of range. Accessing index {} (size = {}).", index, size())
            };
        }
        return *(reinterpret_cast<T const *>(data_.data()) + index);
    }
    auto at(std::size_t index) -> T & {
        if (index >= size()) {
            throw std::out_of_range{
                std::format("InplaceVector access out of range. Accessing index {} (size = {}).", index, size())
            };
        }
        return *(reinterpret_cast<T *>(data_.data()) + index);
    }

    auto pushBack(T const &x) -> void {
        if (size() >= MaxSize) {
            throw std::logic_error{
                std::format("InplaceVector failed to allocate memory.")
            };
        }

        T *pos = reinterpret_cast<T *>(data_.data()) + size();
        new(pos) T(x);
        ++size_;
    }
    auto pushBack(T &&x) -> void {
        if (size() >= MaxSize) {
            throw std::logic_error{
                std::format("InplaceVector failed to allocate memory.")
            };
        }

        T *pos = reinterpret_cast<T *>(data_.data()) + size();
        new(pos) T(std::move(x));
        ++size_;
    }
    auto friend operator== (InplaceVector const &lhs, InplaceVector const &rhs) -> bool {
        return lhs.size() == rhs.size() and std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
    auto friend operator<=> (InplaceVector const &lhs, InplaceVector const &rhs) -> std::strong_ordering {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    auto operator[] (std::size_t index) const -> T const & { return at(index); }
    auto operator[] (std::size_t index) -> T & { return at(index); }
};
#endif  // def INCLUDED_INPLACE_VECTOR
using namespace lib;

namespace Solution_7578310299915525 {
    // 上界为 log n 的数组
    template <typename T>
    using LogVector = InplaceVector<T, 20>;

    // 和树状数组相关的工具
    class BITUtils {
    public:
        static auto constexpr lowbit(i32 x) -> i32 { return x & -x; }
        // 获取树状数组单次查询涉及到的节点
        static auto queried(i32 x) -> LogVector<i32> {
            LogVector<i32> res;
            while (x != 0) {
                res.pushBack(x);
                x -= lowbit(x);
            }
            return res;
        }
        // 获取树状数组单次修改涉及到的节点
        static auto modified(i32 x, i32 n) -> LogVector<i32> {
            LogVector<i32> res;
            while (x <= n) {
                res.pushBack(x);
                x += lowbit(x);
            }
            return res;
        }
    };

    // 树状数组套线段树
    class BITSegTree {
        struct Node {
            i32 sum = 0;
            i32 left = 0, right = 0;
        };
        std::deque<Node> nodes;
        i32 n;
        i32 rootBegin = 0, rootEnd = 0;
        i32 root = 0;

        std::vector<i32> rootOfTree;

        auto pushUp(i32 p) -> void {
            nodes[p].sum = nodes[nodes[p].left].sum + nodes[nodes[p].right].sum;
        }
        auto addPointOnSingleTree(i32 pos, i32 val, i32 &p, i32 nodeBegin, i32 nodeEnd) -> void {
            if (p == 0) {
                p = static_cast<i32>(nodes.size());
                nodes.push_back({});
            }
            if (nodeEnd - nodeBegin == 1) {
                assert(nodeBegin == pos);
                nodes[p].sum += val;
                return;
            }
            auto nodeMid = std::midpoint(nodeBegin, nodeEnd);
            if (pos < nodeMid) addPointOnSingleTree(pos, val, nodes[p].left, nodeBegin, nodeMid);
            else addPointOnSingleTree(pos, val, nodes[p].right, nodeMid, nodeEnd);
            pushUp(p);
        }
        auto findFirstOnTrees(i32 val, LogVector<i32> add, LogVector<i32> sub, i32 nodeBegin, i32 nodeEnd) -> i32 {
            if (nodeEnd - nodeBegin == 1) {
                i32 x = 0;
                for (auto i: add) x += nodes[i].sum;
                for (auto i: sub) x -= nodes[i].sum;
                if (x >= val) return nodeBegin;
                else return nodeEnd;
            }

            i32 leftX = 0;
            for (auto i: add) leftX += nodes[nodes[i].left].sum;
            for (auto i: sub) leftX -= nodes[nodes[i].left].sum;

            i32 nodeMid = std::midpoint(nodeBegin, nodeEnd);
            if (leftX >= val) {
                for (auto &x: add) x = nodes[x].left;
                for (auto &x: sub) x = nodes[x].left;
                return findFirstOnTrees(val, add, sub, nodeBegin, nodeMid);
            } else {
                for (auto &x: add) x = nodes[x].right;
                for (auto &x: sub) x = nodes[x].right;
                return findFirstOnTrees(val - leftX, add, sub, nodeMid, nodeEnd);
            }
        }
        auto sumRangeOnTrees(i32 begin, i32 end, LogVector<i32> add, LogVector<i32> sub, i32 nodeBegin, i32 nodeEnd) -> i32 {
            if (nodeBegin >= begin and nodeEnd <= end) {
                i32 res = 0;
                for (auto x: add) res += nodes[x].sum;
                for (auto x: sub) res -= nodes[x].sum;
                return res;
            }
            auto nodeMid = std::midpoint(nodeBegin, nodeEnd);
            i32 ans = 0;
            if (begin < nodeMid) {
                auto newAdd = add, newSub = sub;
                for (auto &x: newAdd) x = nodes[x].left;
                for (auto &x: newSub) x = nodes[x].left;
                ans += sumRangeOnTrees(begin, end, newAdd, newSub, nodeBegin, nodeMid);
            }
            if (end > nodeMid) {
                auto newAdd = add, newSub = sub;
                for (auto &x: newAdd) x = nodes[x].right;
                for (auto &x: newSub) x = nodes[x].right;
                ans += sumRangeOnTrees(begin, end, newAdd, newSub, nodeMid, nodeEnd);
            }
            return ans;
        }
    public:
        BITSegTree(i32 n, i32 rootBegin, i32 rootEnd): n(n), rootBegin(rootBegin), rootEnd(rootEnd), rootOfTree(n + 1) {
            nodes.push_back({});
        }
        // 在第 layer 层上把 pos 处的值增加 val
        auto addPoint(i32 layer, i32 pos, i32 val) -> void {
            // 使用类似树状数组的方式处理，一共需要修改 log n 棵树
            // 每棵线段树做一次单点修改
            for (auto treeIndex: BITUtils::modified(layer, n)) {
                addPointOnSingleTree(pos, val, rootOfTree[treeIndex], rootBegin, rootEnd);
            }
        }
        // [l1, l2] 层之间，寻找第一个前缀和为 val 的 x。
        auto findFirstBetweenLayers(i32 l1, i32 l2, i32 val) -> i32 {
            auto add = BITUtils::queried(l2);
            for (auto &x: add) x = rootOfTree[x];

            auto sub = BITUtils::queried(l1 - 1);  // 能够保证 l1 == 1 时为空
            for (auto &x: sub) x = rootOfTree[x];

            return findFirstOnTrees(val, add, sub, rootBegin, rootEnd);
        }
        // [l1, l2] 层之间的区间和
        auto sumRangeBetweenLayers(i32 l1, i32 l2, i32 begin, i32 end) -> i32 {
            auto add = BITUtils::queried(l2);
            for (auto &x: add) x = rootOfTree[x];

            auto sub = BITUtils::queried(l1 - 1);
            for (auto &x: sub) x = rootOfTree[x];

            return sumRangeOnTrees(begin, end, add, sub, rootBegin, rootEnd);
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<i32> a(n + 1);
        for (i32 i = 1; i <= n; i++) std::cin >> a[i];

        i32 constexpr maxVal = 1e8;
        BITSegTree sgt{n, 0, maxVal + 1};

        for (i32 i = 1; i <= n; i++) {
            sgt.addPoint(i, a[i], 1);
        }

        for (auto _ = m; _ --> 0; ) {
            i32 op;
            std::cin >> op;

            if (op == 1) {
                // 查询 k 在 [l, r] 排名
                i32 l, r, k;
                std::cin >> l >> r >> k;
                auto ans = sgt.sumRangeBetweenLayers(l, r, 0, k) + 1;
                std::cout << ans << endl;
            } else if (op == 2) {
                // 查询排名为 k 的数
                i32 l, r, k;
                std::cin >> l >> r >> k;
                auto ans = sgt.findFirstBetweenLayers(l, r, k);
                std::cout << ans << endl;
            } else if (op == 3) {
                // 单点修改
                i32 pos, val;
                std::cin >> pos >> val;
                if (a[pos] == val) continue;
                sgt.addPoint(pos, a[pos], -1);
                sgt.addPoint(pos, val, 1);
                a[pos] = val;
            } else if (op == 4) {
                // [l, r] 最大的小于 x 的数
                i32 l, r, x;
                std::cin >> l >> r >> x;
                auto less = sgt.sumRangeBetweenLayers(l, r, 0, x);
                if (less == 0) {
                    std::cout << -2147483647 << endl;
                } else {
                    auto ans = sgt.findFirstBetweenLayers(l, r, less);
                    std::cout << ans << endl;
                }
            } else {
                // [l, r] 最小的大于 x 的数
                i32 l, r, x;
                std::cin >> l >> r >> x;
                auto lessEqual = sgt.sumRangeBetweenLayers(l, r, 0, x + 1);
                if (lessEqual == r - l + 1) {
                    std::cout << 2147483647 << endl;
                } else {
                    auto ans = sgt.findFirstBetweenLayers(l, r, lessEqual + 1);
                    std::cout << ans << endl;
                }
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_7578310299915525::solve();
    return 0;
}
