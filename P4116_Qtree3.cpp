/**
 * @link https://www.luogu.com.cn/problem/P4116
 */
#include "libs/debug_macros.hpp"
#include "lib"
using namespace lib;

namespace Solution_1787195618240513 {
    constexpr const int _N = 1e5+5;
    std::array<std::vector<int>, _N> graph;
    std::array<int, _N> depth, size, fa, son, top, index;
    std::vector<int> order;

    class SegTree {
        struct Node {
            int begin, end;
            int shallow;  // 最浅的点
        };
        std::array<Node, (_N << 2)> tr;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }
        auto pushUp(int p) -> void {
            tr[p].shallow = ranges::min({tr[lson(p)].shallow, tr[rson(p)].shallow}, std::less{}, lam(x, depth[x]));
        }
        auto build(int begin, int end, int p = 1) -> void {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N) { build(0, N, 1); }
        // auto 
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1787195618240513::solve();
    return 0;
}
