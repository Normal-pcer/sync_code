/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3397
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_2769381802995354 {
    template <typename T>
    auto midpoint(T begin, T end) -> T {
        return begin + ((end - begin) >> 1);
    }
    class SegTree {
        struct Node {
            i32 begin = 0, end = 0;
            // 左/右/总共最大的连续 i 数量
            std::array<i32, 2> lmax{};
            std::array<i32, 2> rmax{};
            std::array<i32, 2> max{};
            bool invertTag = false;

            Node(i32 begin = 0, i32 end = 0): begin(begin), end(end) {
                lmax[0] = rmax[0] = max[0] = 1;
                lmax[1] = rmax[1] = max[1] = 0;
            }

            auto empty() const -> bool { return begin == end; }
            auto len() const -> i32 { return end - begin; }
            auto friend operator| (Node const &lhs, Node const &rhs) -> Node {
                if (lhs.empty()) return rhs;
                if (rhs.empty()) return lhs;
                Node res{lhs.begin, rhs.end};
                for (i32 i: {0, 1}) {
                    res.lmax[i] = lhs.lmax[i];
                    if (lhs.lmax[i] == lhs.len()) res.lmax[i] += rhs.lmax[i];
                    res.rmax[i] = rhs.rmax[i];
                    if (rhs.rmax[i] == rhs.len()) res.rmax[i] += lhs.rmax[i];
                    res.max[i] = std::max({lhs.max[i], rhs.max[i], lhs.rmax[i] + rhs.lmax[i]});
                }
                return res;
            }
            auto invert() -> Node & {
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
            if (tree[p].invertTag) {
                tree[lson(p)].invert();
                tree[rson(p)].invert();
                tree[p].invertTag = false;
            }
        }
        auto build(i32 begin, i32 end, i32 p, std::vector<char> const &init) {
            tree[p].begin = begin, tree[p].end = end;
            if (end - begin == 1) {
                if (init[begin]) {
                    tree[p].invert();
                }
                return;
            }
            auto mid = midpoint(begin, end);
            build(begin, mid, lson(p), init);
            build(mid, end, rson(p), init);
            pushUp(p);
        }
    public:
        SegTree(i32 n, std::vector<char> const &init): tree(n << 2) {
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
    };
    auto solve() -> void {
        i32 n, q;
        std::cin >> n >> q;

        std::vector<char> init(n);
        for (auto &x: init) {
            char ch;
            std::cin >> ch;
            x = (ch == '1');
        }

        SegTree sgt{n, init};
        for (auto _ = q; _ --> 0; ) {
            i32 op, x, y;
            std::cin >> op >> x >> y;
            x--;
            
            if (op == 1) {
                sgt.invertRange(x, y);
            } else {
                auto ans = sgt.queryRange(x, y);
                std::cout << ans.max[1] << endl;
            }
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_2769381802995354::solve();
    return 0;
}
