/**
 * @link https://www.luogu.com.cn/problem/P4114
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_4939238518059489 {
    class SegTree {
        struct Node {
            int begin, end;
            int max = 0;
        };
        std::vector<Node> tr;
        
        static constexpr auto lson(int x) -> int { return x << 1; }
        static constexpr auto rson(int x) -> int { return x << 1 | 1; }
        auto pushUp(int p) -> void {
            tr[p].max = std::max(tr[lson(p)].max, tr[rson(p)].max);
        }
        auto build(int begin, int end, auto const &init, int p = 1) -> void {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].max = init.at(begin);
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, init, lson(p)), build(mid, end, init, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N, auto const &init): tr(N << 2) {
            build(0, N, init, 1);
        }
        auto assignAt(int begin, int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= begin + 1) {
                tr[p].max = val;
                return;
            }
            if (tr[lson(p)].end > begin)  assignAt(begin, val, lson(p));
            else  assignAt(begin, val, rson(p));
            pushUp(p);
        }
        auto maxRange(int begin, int end, int p = 1) -> int {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            auto res = 0;
            if (tr[lson(p)].end > begin)  chkMax(res, maxRange(begin, end, lson(p)));
            if (tr[rson(p)].begin < end)  chkMax(res, maxRange(begin, end, rson(p)));
            return res;
        }
    };
    class Tree {
        std::vector<std::vector<int>> graph;
        std::vector<int> size, depth, fa, son, top, order, index;
        std::unique_ptr<SegTree> sgt = nullptr;
        int N;
        std::vector<std::tuple<int, int, int>> edges;
        auto dfs1(int p, int prev) -> void {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;
            for (auto x: graph.at(p))  if (x != prev) {
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
            for (auto x: graph.at(p)) {
                if (x != fa[p] and x != son[p]) {
                    dfs2(x, x);
                }
            }
        }
    public: 
        Tree(int N): graph(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), N(N), edges(N-1) {}
        
        auto assignEdge(int e, int val) -> void {
            auto [x, y, z] = edges.at(e-1);
            if (depth[x] < depth[y])  std::swap(x, y);
            sgt->assignAt(index[x], val);
        }
        auto maxPath(int x, int y) -> int {
            auto res = 0;
            if (x == y)  return 0;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                chkMax(res, sgt->maxRange(index[top[x]], index[x] + 1));
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            chkMax(res, sgt->maxRange(index[y] + 1, index[x] + 1));
            return res;
        }
        friend auto operator>> (std::istream &s, Tree &tr) -> std::istream & {
            auto N = tr.N;
            std::vector<int> init(N + 1);
            for (auto i = 0; i < N - 1; i++) {
                auto &[x, y, z] = tr.edges.at(i);
                s >> x >> y >> z;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            tr.dfs1(1, 0);
            tr.dfs2(1, 1);
            for (auto &[x, y, z]: tr.edges) {
                init[ranges::max({x, y}, std::less{}, lam(x, tr.depth[x]))] = z;
            }
            std::vector<int> sgt_init(N);
            for (auto i = 0; i < N; i++) {
                sgt_init[i] = init[tr.order[i]];
            }
            tr.sgt = std::make_unique<SegTree>(N, sgt_init);
            return s;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        Tree tree{N};  std::cin >> tree;
        while (true) {
            std::string op;  std::cin >> op;
            if (op == "DONE")  return;
            int x, y;  std::cin >> x >> y;
            if (op == "CHANGE") {
                tree.assignEdge(x, y);
            } else {
                std::cout << tree.maxPath(x, y) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4939238518059489::solve();
    return 0;
}
