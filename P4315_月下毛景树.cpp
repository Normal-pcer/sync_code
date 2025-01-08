/**
 * @link https://www.luogu.com.cn/problem/P4315
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9484761912585639 {
    std::vector<std::vector<int>> forward;
    std::vector<std::vector<int>> init;
    std::vector<int> fa, size, depth, son, order, index, top;

    void dfs1(int p, int prev = 0) {
        fa[p] = prev, size[p] = 1;
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
            if (x != fa[p] and x != son[p]) {
                dfs2(x, x);
            }
        }
    }

    class SegTree {
        struct Node {
            int begin, end;
            int max;
            int add_tag, assign_tag = -inf;
        };
        constexpr static int lson(int p) { return p << 1; }
        constexpr static int rson(int p) { return p << 1 | 1; }
        std::vector<Node> tr;

        void assignNode(int p, int val) {
            tr[p].max = val;
            tr[p].assign_tag = val;
            tr[p].add_tag = 0;
        }
        void addNode(int p, int val) {
            tr[p].max += val;
            tr[p].add_tag += val;
            assert(tr[p].assign_tag == -inf or tr[p].begin + 1 == tr[p].end);
        }
        void pushUp(int p) {
            tr[p].max = std::max(tr[lson(p)].max, tr[rson(p)].max);
        }
        void pushDown(int p) {
            if (tr[p].assign_tag != -inf) {
                for (auto s: {lson(p), rson(p)}) {
                    assignNode(s, tr[p].assign_tag);
                }
                tr[p].assign_tag = -inf;
            }
            if (tr[p].add_tag != 0) {
                for (auto s: {lson(p), rson(p)}) {
                    addNode(s, tr[p].add_tag);
                }
                tr[p].add_tag = 0;
            }
        }
        void build(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
        }
    public:
        SegTree(int N): tr((N + 1) << 2) {
            build(0, N, 1);
        }
        void assignAt(int pos, int val, int p = 1) {
            debug  std::cout << std::format("assignAt {} {} {}", pos, val, p) << std::endl;
            if (tr[p].begin >= pos and tr[p].end <= pos + 1) {
                assignNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > pos)  assignAt(pos, val, lson(p));
            else  assignAt(pos, val, rson(p));
            pushUp(p);
        }
        void assignRange(int begin, int end, int val, int p = 1) {
            debug  std::cout << std::format("assignRange {} {} {} {}", begin, end, val, p) << std::endl;
            if (tr[p].begin >= begin and tr[p].end <= end) {
                assignNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  assignRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  assignRange(begin, end, val, rson(p));
            pushUp(p);
        }
        int maxRange(int begin, int end, int p = 1) {
            debug  std::cout << std::format("maxRange {} {} {}", begin, end, p) << std::endl;
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].max;
            }
            auto res = -inf;
            pushDown(p);
            if (tr[lson(p)].end > begin)  chkMax(res, maxRange(begin, end, lson(p)));
            if (tr[rson(p)].begin < end)  chkMax(res, maxRange(begin, end, rson(p)));
            return res;
        }
        void addRange(int begin, int end, int val, int p = 1) {
            debug  std::cout << std::format("addRange {} {} {} {}", begin, end, val, p) << std::endl;
            if (tr[p].begin >= begin and tr[p].end <= end) {
                addNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  addRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  addRange(begin, end, val, rson(p));
            pushUp(p);
        }
    };

    std::unique_ptr<SegTree> sgt;

    void assignEdge(int x, int y, int val) {
        if (depth[x] < depth[y])  std::swap(x, y);
        sgt->assignAt(index[x], val);
    }
    void assignPath(int x, int y, int val) {
        while (top[x] != top[y]) {
            if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
            sgt->assignRange(index[top[x]], index[x] + 1, val);
            x = fa[top[x]];
        }
        if (depth[x] < depth[y])  std::swap(x, y);
        sgt->assignRange(index[y] + 1, index[x] + 1, val);
    }
    void addPath(int x, int y, int val) {
        while (top[x] != top[y]) {
            if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
            sgt->addRange(index[top[x]], index[x] + 1, val);
            x = fa[top[x]];
        }
        if (depth[x] < depth[y])  std::swap(x, y);
        sgt->addRange(index[y] + 1, index[x] + 1, val);
    }
    int maxPath(int x, int y) {
        auto res = -inf;
        while (top[x] != top[y]) {
            if (depth[top[x]] < depth[top[y]])  std::swap(x, y);
            chkMax(res, sgt->maxRange(index[top[x]], index[x] + 1));
            x = fa[top[x]];
        }
        if (depth[x] < depth[y])  std::swap(x, y);
        chkMax(res, sgt->maxRange(index[y] + 1, index[x] + 1));
        return res;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        forward.resize(N+1), fa.resize(N+1), size.resize(N+1), depth.resize(N+1);
        son.resize(N+1), index.resize(N+1), top.resize(N+1);
        std::vector<std::tuple<int, int, int>> edges(N-1);
        for (auto &[x, y, val]: edges)  std::cin >> x >> y >> val;
        for (auto [x, y, val]: edges) {
            forward.at(x).push_back(y);
            forward.at(y).push_back(x);
        }

        dfs1(1, 0);
        dfs2(1, 1);

        debug {
            for (auto x: order)  std::cout << x << ' ';
            std::cout << std::endl;
        }

        sgt = std::make_unique<SegTree>(N);
        std::string op;
        int x, y, z;
        for (auto [x, y, val]: edges) {
            assignEdge(x, y, val);
        }
        while (true) {
            std::cin >> op;
            if (op == "Stop")  break;
            std::cin >> x >> y;
            if (op == "Change") {
                assignEdge(std::get<0>(edges[x-1]), std::get<1>(edges[x-1]), y);
            } else if (op == "Cover") {
                std::cin >> z;
                assignPath(x, y, z);
            } else if (op == "Add") {
                std::cin >> z;
                addPath(x, y, z);
            } else {
                auto ans = maxPath(x, y);
                std::cout << ans << std::endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_9484761912585639::solve();
    return 0;
}
