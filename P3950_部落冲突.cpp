/**
 * @link https://www.luogu.com.cn/problem/P3950
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1357344625133186 {
    class BIT {
        std::vector<int> c;
        static constexpr auto lowbit(int x) -> int { return x & -x; }
    public:
        BIT(int N): c(N+1) {}
        auto addAt(int x, int val) -> void {
            x++;
            while (x < static_cast<int>(c.size())) {
                c.at(x) += val;
                x += lowbit(x);
            }
        }
        auto sumPrefix(int x) const -> int {
            if (x < 0)  return 0;
            x++;  auto res = 0;
            while (x) {
                res += c.at(x);
                x -= lowbit(x);
            }
            return res;
        }
        auto sumRange(int begin, int end) const -> int {
            return sumPrefix(end - 1) - sumPrefix(begin - 1);
        }
    };
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> depth, size, fa, son, top, order, index;
        int N;
        BIT bit;
        std::vector<std::pair<int, int>> wars;

        auto dfs1(int p, int prev) -> void {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;
            for (auto x: forward.at(p))  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[son[p]] < size[x]) {
                    son[p] = x;
                }
            }
        }
        auto dfs2(int p, int tp) -> void {
            top[p] = tp, index[p] = order.size(), order.push_back(p);
            if (son[p] != 0)  dfs2(son[p], tp);
            for (auto x: forward.at(p)) {
                if (x != fa[p] and x != son[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): forward(N+1), depth(N+1), size(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), N(N), bit(N) {}

        auto sumPath(int x, int y) const -> int {
            auto res = 0;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                res += bit.sumRange(index[top[x]], index[x] + 1);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            res += bit.sumRange(index[y] + 1, index[x] + 1);
            return res;
        }
        auto warBegin(int x, int y) -> void {
            if (depth[x] < depth[y])  std::swap(x, y);
            wars.push_back({x, y});
            assert(bit.sumRange(index[x], index[x] + 1) == 0);
            bit.addAt(index[x], +1);
        }
        auto warEnd(int i) -> void {
            auto [x, y] = wars.at(i - 1);
            assert(bit.sumRange(index[x], index[x] + 1) == 1);
            bit.addAt(index[x], -1);
        }
        friend auto operator>> (std::istream &s, Tree &tr) -> std::istream & {
            auto N = tr.N;
            for (auto _: range(N-1)) {
                int x, y;  s >> x >> y;
                tr.forward.at(x).push_back(y);
                tr.forward.at(y).push_back(x);
            }
            tr.dfs1(1, 0);
            tr.dfs2(1, 1);
            return s;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        Tree tree{N};  std::cin >> tree;

        for (auto _: range(M)) {
            char op;  std::cin >> op;
            int x;  std::cin >> x;
            if (op == 'Q') {
                int y;  std::cin >> y;
                std::cout << (tree.sumPath(x, y) == 0? "Yes": "No") << std::endl;
            } else if (op == 'C') {
                int y;  std::cin >> y;
                tree.warBegin(x, y);
            } else if (op == 'U') {
                tree.warEnd(x);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1357344625133186::solve();
    return 0;
}
