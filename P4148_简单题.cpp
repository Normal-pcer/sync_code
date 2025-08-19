/**
 * @link https://www.luogu.com.cn/problem/P4148
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
namespace lib {
#if __cplusplus > 201703LL
    namespace ranges = std::ranges;
    namespace views = std::views;
#endif
}
char constexpr endl = '\n';
using namespace lib;
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_3494431784083846 {
namespace {
    i32 constexpr limit = 10000;

    auto is_sorted(auto const &) -> bool {
        return true;
    }

    template <typename ...Ts>
    auto is_sorted(auto const &x, auto const &y, Ts const &...args) -> bool {
        return (x <= y) && is_sorted(y, args...);
    }

    struct Point {
        std::array<i32, 2> pos{};
        i32 count{};

        auto operator[] (std::size_t index) -> i32 & {
            return pos[index];
        }
    };

    class KDTree {
        struct Node {
            i32 sum{};
            u32 lch{}, rch{};
            std::array<i32, 2> min{}, max{};

            Point point{};
        };

        std::deque<Node> nodes;
        std::deque<u32> collected;

        u32 root = 0;
        u32 size = 0;

        auto alloc(Point p) -> u32 {
            u32 res;
            if (not collected.empty()) {
                res = collected.back();
                collected.pop_back();
            } else {
                nodes.emplace_back();
                res = nodes.size() - 1;
            }
            nodes[res] = {}, nodes[res].point = p;
            nodes[res].sum = p.count;
            nodes[res].min = nodes[res].max = p.pos;
            return res;
        }
        auto free(u32 p) -> void {
            collected.emplace_back(p);
        }
        auto push_up(u32 p) -> void {
            for (auto ch: {nodes[p].lch, nodes[p].rch}) {
                if (ch == 0) continue;
                for (u32 dim: {0, 1}) {
                    chkMin(nodes[p].min[dim], nodes[ch].min[dim]);
                    chkMax(nodes[p].max[dim], nodes[ch].min[dim]);
                }
            }
            nodes[p].sum = nodes[nodes[p].lch].sum + nodes[nodes[p].rch].sum + nodes[p].point.count;
        }
        using iter_ = std::vector<Point>::iterator;
        auto build_(iter_ first, iter_ last, u32 dim) -> u32 {
            if (first == last) return 0;
            u32 mid = (last - first) / 2;

            ranges::nth_element(first, first + mid, last, ranges::less{}, lam(point, point[dim]));

            auto res = alloc(first[mid]);
            nodes[res].lch =  build_(first, first + mid, dim ^ 1);
            nodes[res].rch = build_(first + mid + 1, last, dim ^ 1);
            push_up(res);
            return res;
        }
        auto query_(std::array<i32, 2> min, std::array<i32, 2> max, u32 p) -> i32 {
            if (p == 0) return 0;

            auto this_res = [&] -> i32 {
                auto in_range = true;
                for (u32 dim: {0, 1}) {
                    in_range &= is_sorted(min[dim], nodes[p].point[dim], max[dim]);
                }
                if (in_range) return nodes[p].point.count;
                else return 0;
            }();

            auto contained = true, unique = false;
            for (u32 dim: {0, 1}) contained &= min[dim] <= nodes[p].min[dim] and nodes[p].max[dim] <= max[dim];
            for (u32 dim: {0, 1}) unique |= nodes[p].max[dim] < min[dim] or max[dim] < nodes[p].min[dim];

            if (contained) return nodes[p].sum;
            if (unique) return 0;
            return query_(min, max, nodes[p].lch) + query_(min, max, nodes[p].rch) + this_res;
        }
        auto insert_(Point point, u32 dim, u32 &p) -> void {
            if (p == 0) {
                p = alloc(point);
                return;
            }

            if (point[dim] < nodes[p].point[dim]) insert_(point, dim ^ 1, nodes[p].lch);
            else insert_(point, dim ^ 1, nodes[p].rch);
            push_up(p);
        }
        // bx chengyifan
        auto pia_(u32 p, std::vector<Point> &res) -> void {
            if (p == 0) return;
            pia_(nodes[p].lch, res);
            res.emplace_back(nodes[p].point);
            pia_(nodes[p].rch, res);
            free(p);
        }
    public:
        KDTree() {
            nodes.emplace_back();
        }
        KDTree(std::vector<Point> init) {
            nodes.emplace_back();  // 1-index
            root = build_(init.begin(), init.end(), 0);
        }
        auto query(i32 x1, i32 y1, i32 x2, i32 y2) -> i32 {
            return query_({x1, y1}, {x2, y2}, root);
        }
        auto insert(Point point) -> void {
            insert_(point, 0, root);
            ++size;
        }
        auto rebuild() -> void {
            std::vector<Point> seq;
            seq.reserve(size);

            pia_(root, seq);
            build_(seq.begin(), seq.end(), 0);
        }
    };
    auto solve() -> void {
        i32 n{}; std::cin >> n;

        KDTree kdt{};
        i32 last_ans{};

        while (true) {
            i32 op{}; std::cin >> op;
            if (op == 1) {
                i32 x{}, y{}, a{};
                std::cin >> x >> y >> a;
                x ^= last_ans, y ^= last_ans, a ^= last_ans;
                kdt.insert({x, y, a});
            } else if (op == 2) {
                i32 x1{}, y1{}, x2{}, y2{};
                std::cin >> x1 >> y1 >> x2 >> y2;
                x1 ^= last_ans, x2 ^= last_ans, y1 ^= last_ans, y2 ^= last_ans;
                std::cout << (last_ans = kdt.query(x1, y1, x2, y2)) << endl;
            } else {
                break;
            }
        }
    }
}
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_3494431784083846::solve();
    return 0;
}
