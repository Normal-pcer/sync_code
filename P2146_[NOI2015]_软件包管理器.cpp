/**
 * @link https://www.luogu.com.cn/problem/P2146
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_6446487739675902 {
    class SegTree {
        struct Node {
            int begin, end;
            int sum, assign_tag = -1;
        };
        std::vector<Node> tr;

        static constexpr int lson(int x) { return x << 1; }
        static constexpr int rson(int x) { return x << 1 | 1; }
        auto assignNode(int p, int val) -> void {
            tr[p].sum = val * (tr[p].end - tr[p].begin);
            tr[p].assign_tag = val;
        }
        auto pushUp(int p) -> void {
            tr[p].sum = tr[lson(p)].sum + tr[rson(p)].sum;
        }
        auto pushDown(int p) -> void {
            if (tr[p].assign_tag != -1) {
                for (auto s: {lson(p), rson(p)}) {
                    assignNode(s, tr[p].assign_tag);
                }
                tr[p].assign_tag = -1;
            }
        }
        auto build(int begin, int end, int p = 1) -> void {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
        }
    public:
        SegTree(int N): tr(N << 2) {
            build(0, N, 1);
        }
        auto assignRange(int begin, int end, int val, int p = 1) -> void {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                assignNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  assignRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  assignRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto sumRange(int begin, int end, int p = 1) -> int {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].sum;
            }
            pushDown(p);
            auto res = 0;
            if (tr[lson(p)].end > begin)  res += sumRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res += sumRange(begin, end, rson(p));
            return res;
        }
    };
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> size, depth, fa, son, top, order, index;
        int N;
        std::unique_ptr<SegTree> sgt = nullptr;

        void dfs1(int p, int prev) {
            size[p] = 1, fa[p] = prev, son[p] = N;
            depth[p] = depth[prev] + 1;
            for (auto x: forward.at(p))  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
                if (size[x] > size[son[p]]) {
                    son[p] = x;
                }
            }
        }
        void dfs2(int p, int tp) {
            top[p] = tp, index[p] = static_cast<int>(order.size()), order.push_back(p);
            if (son[p] != N)  dfs2(son[p], tp);
            for (auto x: forward.at(p)) {
                if (x != son[p] and x != fa[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N): forward(N+1), size(N+1), depth(N+1), fa(N+1), son(N+1), top(N+1), index(N+1), N(N) {}
        auto assignAncestor(int p, int val) -> void {
            while (top[p] != 0) {
                sgt->assignRange(index[top[p]], index[p] + 1, val);
                p = fa[top[p]];
            }
            sgt->assignRange(index[0], index[p] + 1, val);
        }
        auto assignChildren(int p, int val) -> void {
            sgt->assignRange(index[p], index[p] + size[p], val);
        }
        auto sum() -> int {
            return sgt->sumRange(0, N);
        }
        friend auto operator>> (std::istream &stream, Tree &tree) -> std::istream & {
            for (auto i = 1; i < tree.N; i++) {
                int x;  stream >> x;
                tree.forward.at(x).push_back(i);
                tree.forward.at(i).push_back(x);
            }
            tree.dfs1(0, tree.N);
            tree.dfs2(0, 0);
            tree.sgt = std::make_unique<SegTree>(tree.N);
            return stream;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        Tree tree{N};  std::cin >> tree;
        int Q;  std::cin >> Q;
        for (auto _ = 0; _ < Q; _++) {
            std::string op;  std::cin >> op;
            int x;  std::cin >> x;
            auto before = tree.sum();
            if (op[0] == 'u') {
                tree.assignChildren(x, 0);
            } else {
                tree.assignAncestor(x, 1);
            }
            auto after = tree.sum();
            auto del = after - before;
            std::cout << std::abs(del) << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6446487739675902::solve();
    return 0;
}
