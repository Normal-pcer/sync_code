/**
 * @link https://www.luogu.com.cn/problem/P2486
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1543338549590594 {
    struct ColorNode {
        int cnt;  // 这一段的颜色数量
        int lc, rc;  // 左端点和右端点的颜色

        ColorNode(): cnt(0), lc(0), rc(0) {}
        ColorNode(int cnt, int lc, int rc): cnt(cnt), lc(lc), rc(rc) {}
        ColorNode(int color): cnt(1), lc(color), rc(color) {}

        ColorNode operator+ (const ColorNode &other) const {
            // 合并两个颜色节点
            assert(*this or other);  // 不可以合并两个空节点
            // 如果有一个为空，直接返回另一个
            if (not *this)  return other;
            if (not other)  return *this;
            ColorNode res {cnt + other.cnt, lc, other.rc};
            // 合并中间段
            if (rc == other.lc) {
                res.cnt--;
            }
            return res;
        }
        ColorNode &operator+= (const ColorNode &other) {
            *this = *this + other;
            return *this;
        }
        operator bool () const {
            return cnt != 0;
        }
        operator std::string () const {
            return std::format("ColorNode({}, {}, {})", cnt, lc, rc);
        }
    };
    class SegTree {
        struct Node {
            int begin, end;
            ColorNode color;
            int assign_tag = 0;
        };
        std::vector<Node> tr;
        static constexpr int lson(int p) { return p << 1; }
        static constexpr int rson(int p) { return p << 1 | 1; }
        void pushUp(int p) {
            tr[p].color = tr[lson(p)].color + tr[rson(p)].color;
        }
        void assignNode(int p, int val) {
            tr[p].color = {val};
            tr[p].assign_tag = val;
        }
        void pushDown(int p) {
            if (tr[p].assign_tag != 0) {
                for (auto s: {lson(p), rson(p)}) {
                    assignNode(s, tr[p].assign_tag);
                }
                tr[p].assign_tag = 0;
            }
        }
        void build(int begin, int end, std::vector<int> const &init_color, int p) {
            tr[p].begin = begin, tr[p].end = end;
            if (begin + 1 == end) {
                tr[p].color = {init_color[begin]};
                return;
            }
            auto mid = std::midpoint(begin, end);
            build(begin, mid, init_color, lson(p));
            build(mid, end, init_color, rson(p));
            pushUp(p);
        }
    public:
        SegTree(int N, std::vector<int> const &init_color): tr(N << 2) {
            build(0, N, init_color, 1);
        }
        void assignRange(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                assignNode(p, val);
                return;
            }
            pushDown(p);
            if (tr[lson(p)].end > begin)  assignRange(begin, end, val, lson(p));
            if (tr[rson(p)].begin < end)  assignRange(begin, end, val, rson(p));
            pushUp(p);
        }
        auto queryColorRange(int begin, int end, int p = 1) -> ColorNode {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                return tr[p].color;
            }
            ColorNode res{};
            pushDown(p);
            if (tr[lson(p)].end > begin)  res += queryColorRange(begin, end, lson(p));
            if (tr[rson(p)].begin < end)  res += queryColorRange(begin, end, rson(p));
            return res;
        }
    };
    class Tree {
        std::vector<std::vector<int>> nu4;
        std::vector<int> fa, son, size, depth, top, order, index;
        std::unique_ptr<SegTree> sgt = nullptr;
        void dfs1(int p, int prev = 0) {
            size[p] = 1, fa[p] = prev;
            depth[p] = depth[prev] + 1;
            for (auto x: nu4[p])  if (x != prev) {
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
            for (auto x: nu4[p]) {
                if (x != fa[p] and x != son[p]) {
                    dfs2(x, x);
                }
            }
        }
    public:
        Tree(int N, std::vector<int> const &w, std::vector<std::pair<int, int>> const &edges): nu4(N+1), fa(N+1), son(N+1), size(N+1), depth(N+1), top(N+1), index(N+1) {
            for (auto [x, y]: edges) {
                nu4.at(x).push_back(y);
                nu4.at(y).push_back(x);
            }
            dfs1(1, 0);
            dfs2(1, 1);
            std::vector<int> sgt_init(N);
            for (auto i = 0; i < N; i++) {
                sgt_init[i] = w.at(order[i]);
            }
            sgt = std::make_unique<SegTree>(N, sgt_init);

            debug {
                std::cout << "order: ";
                for (auto x: order) {
                    std::cout << x << ' ';
                }
                std::cout << std::endl;
            }
        }
        void assignPath(int x, int y, int val) {
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                debug  std::cout << std::format("assignRange {} {} {}", index[top[x]], index[x] + 1, val) << std::endl;
                sgt->assignRange(index[top[x]], index[x] + 1, val);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y);
            }
            debug  std::cout << std::format("assignRange {} {} {}", index[y], index[x] + 1, val) << std::endl;
            sgt->assignRange(index[y], index[x] + 1, val);
        }
        auto countPath(int x, int y) {
            ColorNode base_x, base_y;
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y), std::swap(base_x, base_y);
                }
                debug  std::cout << std::format("queryColorRange {} {} -> {}", index[top[x]], index[x] + 1, 
                    static_cast<std::string>(sgt->queryColorRange(index[top[x]], index[x] + 1))) << std::endl;
                base_x = sgt->queryColorRange(index[top[x]], index[x] + 1) + base_x;  // 合并到 base 的左边
                x = fa[top[x]];
            }
            if (depth[x] < depth[y]) {
                std::swap(x, y), std::swap(base_x, base_y);
            }
            debug  std::cout << std::format("queryColorRange {} {} -> {}", index[y], index[x] + 1, 
                static_cast<std::string>(sgt->queryColorRange(index[y], index[x] + 1))) << std::endl;
            auto mid = sgt->queryColorRange(index[y], index[x] + 1);
            // 翻转左区间（使得左区间的 lc 为较深端）
            std::swap(base_y.lc, base_y.rc);
            // 依次合并左中右三个区间
            return base_y + mid + base_x;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<int> w(N+1);
        for (auto &x: w | views::drop(1))  std::cin >> x;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges) {
            std::cin >> x >> y;
        }

        Tree tree {N, w, edges};
        for (auto _ = 0; _ < M; _++) {
            char op;  int x, y;
            std::cin >> op >> x >> y;
            if (op == 'C') {
                int z;  std::cin >> z;
                tree.assignPath(x, y, z);
            } else {
                std::cout << tree.countPath(x, y).cnt << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1543338549590594::solve();
    return 0;
}
