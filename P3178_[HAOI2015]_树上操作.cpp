/**
 * @link https://www.luogu.com.cn/problem/P3178
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1606595698450824 {
    class SegTree {
        struct Node {
            int begin, end;
            ll sum, add_tag;
        };
        std::vector<Node> tr;

        static constexpr auto lson(int x) -> int { return x << 1; }
        static constexpr auto rson(int x) -> int { return x << 1 | 1; }
        auto len(int p) const -> int { return tr[p].end - tr[p].begin; }
        auto addNode(int p, ll val) {
            tr[p].sum += val * len(p);
            tr[p].add_tag += val;
        }
        auto pushUp(int p) {
            tr[p].sum = tr[lson(p)].sum + tr[rson(p)].sum;
        }
        auto pushDown(int p) {
            if (tr[p].add_tag != 0) {
                for (auto s: {lson(p), rson(p)}) {
                    addNode(s, tr[p].add_tag);
                }
                tr[p].add_tag = 0;
            }
        }
        auto build(int begin, int end, auto const &init, int p) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].sum = init.at(begin);
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
        auto addRange(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  addRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  addRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto sumRange(int begin, int end, int p = 1) -> ll {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].sum;
            }
            pushDown(p);
            auto res = 0LL;
            if (tr[lson(p)].end > begin)  res += sumRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res += sumRange(begin, end, rson(p));
            return res;
        }
    };
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> size, depth, fa, son, top, order, index;
        int N;
        std::unique_ptr<SegTree> sgt;
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
        Tree(int N): forward(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), N(N) {}
        auto addNode(int p, int val) {
            sgt->addRange(index[p], index[p] + 1, val);
        }
        auto addChildren(int p, int val) {
            sgt->addRange(index[p], index[p] + size[p], val);
        }
        auto sumAncestor(int p) -> ll {
            auto res = 0LL;
            while (top[p] != top[1]) {
                res += sgt->sumRange(index[top[p]], index[p] + 1);
                p = fa[top[p]];
            }
            res += sgt->sumRange(index[1], index[p] + 1);
            return res;
        }
    
        friend auto operator>> (std::istream &s, Tree &t) -> std::istream & {
            auto N = t.N;
            std::vector<int> init(N+1);
            for (auto &x: init | views::drop(1))  s >> x;
            for (auto _: range(N-1)) {
                int x, y;  s >> x >> y;
                t.forward.at(x).push_back(y);
                t.forward.at(y).push_back(x);
            }
            t.dfs1(1, 0);
            t.dfs2(1, 1);
            std::vector<int> sgt_init(N);
            for (auto i: range(N))  sgt_init[i] = init[t.order.at(i)];
            t.sgt = std::make_unique<SegTree>(N, sgt_init);
            return s;
        }
    };
    void solve() {
        int N, M;  std::cin >> N >> M;
        Tree tree{N};  std::cin >> tree;
        for (auto _: range(M)) {
            int op, x;  std::cin >> op >> x;
            if (op == 1) {
                int y;  std::cin >> y;
                tree.addNode(x, y);
            } else if (op == 2) {
                int y;  std::cin >> y;
                tree.addChildren(x, y);
            } else {
                std::cout << tree.sumAncestor(x) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1606595698450824::solve();
    return 0;
}
