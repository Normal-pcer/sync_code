/**
 * @link https://www.luogu.com.cn/problem/P2572
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/null_number.hpp"
using namespace lib;

/*
直接维护。
*/
namespace Solution_9459467120953669 {
    using i8 = std::int8_t;
    class SegTree {
        struct Node {
            i32 begin = 0, end = 0;
            
            using DataPair = std::array<i32, 2>;
            DataPair count{};       // 个数
            DataPair lmax{};        // 左侧最长连续段
            DataPair rmax{};        // 右侧最长
            DataPair max{};         // 总共最长

            // 先赋值，再反转
            i8 assignTag = null;
            bool invertTag = false;

            Node(i32 begin = 0, i32 end = 0): begin(begin), end(end) {
                count[0] = lmax[0] = rmax[0] = max[0] = 1;
            }

            auto len() const -> i32 { return end - begin; }
            auto empty() const -> bool { return begin == end; }

            auto friend operator| (Node const &a, Node const &b) -> Node {
                if (a.empty()) return b;
                if (b.empty()) return a;
                Node res{a.begin, b.end};
                for (i32 i: {0, 1}) {
                    res.count[i] = a.count[i] + b.count[i];
                    res.lmax[i] = a.lmax[i], res.rmax[i] = b.rmax[i];
                    if (a.lmax[i] == a.len()) res.lmax[i] += b.lmax[i];
                    if (b.rmax[i] == b.len()) res.rmax[i] += a.rmax[i];
                    res.max[i] = std::max({a.max[i], b.max[i], a.rmax[i] + b.lmax[i]});
                }
                return res;
            }
            auto assign(bool to) -> Node & {
                count[to] = lmax[to] = rmax[to] = max[to] = len();
                auto other = not to;
                count[other] = lmax[other] = rmax[other] = max[other] = 0;

                assignTag = to;
                invertTag = false;
                return *this;
            }
            auto invert() -> Node & {
                std::swap(count[0], count[1]);
                std::swap(lmax[0], lmax[1]);
                std::swap(rmax[0], rmax[1]);
                std::swap(max[0], max[1]);
                invertTag = not invertTag;
                return *this;
            }
        };
        std::vector<Node> tree;

        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }

        auto pushUp(i32 p) -> void {
            tree[p] = tree[lson(p)] | tree[rson(p)];
        }
        auto pushDown(i32 p) -> void {
            if (tree[p].assignTag != null) {
                tree[lson(p)].assign(tree[p].assignTag);
                tree[rson(p)].assign(tree[p].assignTag);
                tree[p].assignTag = null;
            }
            if (tree[p].invertTag) {
                tree[lson(p)].invert();
                tree[rson(p)].invert();
                tree[p].invertTag = false;
            }
        }
        auto build(i32 begin, i32 end, i32 p, std::vector<i8> const &init) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (end - begin == 1) {
                if (init[begin]) {
                    tree[p].invert();
                }
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p), init);
            build(mid, end, rson(p), init);
            pushUp(p);
        }
    public:
        SegTree(i32 n, std::vector<i8> const &init): tree(n << 2) {
            build(0, n, 1, init);
        }

        auto queryRange(i32 begin, i32 end, i32 p = 1) -> Node {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p];
            }
            Node res{};
            pushDown(p);
            if (tree[lson(p)].end > begin) res = res | queryRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end) res = res | queryRange(begin, end, rson(p));
            return res;
        }
        auto invertRange(i32 begin, i32 end, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].invert();
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin) invertRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end) invertRange(begin, end, rson(p));
            pushUp(p);
        }
        auto assignRange(i32 begin, i32 end, bool to, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].assign(to);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin) assignRange(begin, end, to, lson(p));
            if (tree[rson(p)].begin < end) assignRange(begin, end, to, rson(p));
            pushUp(p);
        }
    };
    auto solve() -> void {
        i32 n, m;
        std::cin >> n >> m;

        std::vector<i8> init(n);
        for (auto &x: init) {
            i32 tmp;
            std::cin >> tmp;
            x = tmp;
        }

        SegTree sgt{n, init};
        for (auto _ = m; _ --> 0; ) {
            i32 op, x, y;
            std::cin >> op >> x >> y;
            y++;
            if (op == 0) {
                sgt.assignRange(x, y, 0);
            } else if (op == 1) {
                sgt.assignRange(x, y, 1);
            } else if (op == 2) {
                sgt.invertRange(x, y);
            } else {
                auto ans = sgt.queryRange(x, y);
                if (op == 3) std::cout << ans.count[1] << endl;
                else std::cout << ans.max[1] << endl;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_9459467120953669::solve();
    return 0;
}
