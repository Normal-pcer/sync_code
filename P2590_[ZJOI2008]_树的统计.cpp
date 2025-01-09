/**
 * @link https://www.luogu.com.cn/problem/P2590
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/io.hpp"

using namespace lib;
namespace Solution_1245402782909691 {
    // constexpr const int _N = 3e4+5;
    // std::array<std::vector<int>, _N> forward;
    // std::array<int, _N> size, fa, depth, son, top, index;
    // std::vector<int> order;
    std::vector<std::vector<int>> forward;
    std::vector<int> size, fa, depth, son, top, index;
    std::vector<int> order;
    class SegTree {
        struct Node {
            int begin, end;
            int sum = 0, max = -inf;
        };
        std::vector<Node> tr;
        static constexpr int lson(int p) { return p << 1; }
        static constexpr int rson(int p) { return p << 1 | 1; }
        void assignNode(int, int) {}
        void pushUp(int p) {
            tr[p].sum = tr[lson(p)].sum + tr[rson(p)].sum;
            tr[p].max = std::max(tr[lson(p)].max, tr[rson(p)].max);
        }
        void pushDown(int) {}
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = (begin + end) >> 1;
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N): tr((N+1) << 2) {
            build(0, N, 1);
        }
        void assignAt(int pos, int val, int p = 1) {
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].sum = val;
                tr[p].max = val;
                return;
            }
            if (tr[lson(p)].end > pos)  assignAt(pos, val, lson(p));
            else  assignAt(pos, val, rson(p));
            pushUp(p);
        }
        int maxRange(int begin, int end, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            pushDown(p);
            auto res = -inf;
            if (tr[lson(p)].end > begin)  chkMax(res, maxRange(begin, end, lson(p)));
            if (tr[rson(p)].begin < end)  chkMax(res, maxRange(begin, end, rson(p)));
            return res;
        }
        int sumRange(int begin, int end, int p = 1) {
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
    std::unique_ptr<SegTree> sgt;
    void dfs1(int p, int prev) {
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
    void dfs2(int p, int tp) {
        top[p] = tp, index[p] = order.size(), order.push_back(p);
        if (son[p] != 0)  dfs2(son[p], tp);
        for (auto x: forward.at(p)) {
            if (x != son[p] and x != fa[p]) {
                dfs2(x, x);
            }
        }
    }
    void assignAt(int p, int val) {
        sgt->assignAt(index[p], val);
    }
    int maxPath(int x, int y) {
        auto res = -inf;
        while (top[x] != top[y]) {
            if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
            chkMax(res, sgt->maxRange(index[top[x]], index[x] + 1));
            x = fa[top[x]];
        }
        if (depth[x] < depth[y])  std::swap(x, y);
        chkMax(res, sgt->maxRange(index[y], index[x] + 1));
        return res;
    }
    int sumPath(int x, int y) {
        auto res = 0;
        while (top[x] != top[y]) {
            if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
            res += sgt->sumRange(index[top[x]], index[x] + 1);
            x = fa[top[x]];
        }
        if (depth[x] < depth[y])  std::swap(x, y);
        res += sgt->sumRange(index[y], index[x] + 1);
        return res;
    }
    void solve() {
#define cin 0
        int N;  io >> N;
        forward.resize(N+1);
        size.resize(N+1), fa.resize(N+1), depth.resize(N+1), son.resize(N+1);
        top.resize(N+1), index.resize(N+1);
        for (auto _ = 0; _ < N-1; _++) {
            int x, y;  io >> x >> y;
            forward.at(x).push_back(y);
            forward.at(y).push_back(x);
        }
        dfs1(1, 0);
        dfs2(1, 1);

        sgt = std::make_unique<SegTree>(N);
        for (auto i = 1; i < N+1; i++) {
            int x;  io >> x;
            assignAt(i, x);
        }

        std::string op;  int x, y;
        int Q;  io >> Q;
        for (auto _ = 0; _ < Q; _++) {
            io >> op >> x >> y;
            if (op == "CHANGE") {
                assignAt(x, y);
            } else if (op == "QMAX") {
                io << maxPath(x, y) << endl;
            } else {
                io << sumPath(x, y) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1245402782909691::solve();
    return 0;
}
