/**
 * @link https://www.luogu.com.cn/problem/P3313
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_4828700238829491 {
    struct Node {
        Node *left;
        Node *right;
        int sum = 0, max = -inf;

        Node();
        Node(int);
    };
    std::deque<Node> nodes{Node{0}};
    Node::Node(): left(&nodes.front()), right(&nodes.front()) {}
    Node::Node(int): left(nullptr), right(nullptr) {}

    class SegTree {
        Node *root = &nodes.front();
        int root_begin, root_end;

        auto pushUp(Node *node) -> void {
            node->sum = node->left->sum + node->right->sum;
            node->max = std::max(node->left->max, node->right->max);
        }
    public:
        SegTree(int begin, int end): root_begin(begin), root_end(end) {}
        auto assignAt(int pos, int val, Node *&p, int node_begin, int node_end) -> void {
            if (p == &nodes.front())  nodes.push_back({}), p = &nodes.back();
            if (node_begin + 1 == node_end) {
                p->max = p->sum = val;
                return;
            }
            auto node_mid = std::midpoint(node_begin, node_end);
            if (pos < node_mid)  assignAt(pos, val, p->left, node_begin, node_mid);
            else  assignAt(pos, val, p->right, node_mid, node_end);
            pushUp(p);
        }
        auto assignAt(int pos, int val) {
            assignAt(pos, val, root, root_begin, root_end);
        }
        auto sumRange(int begin, int end, Node *p, int node_begin, int node_end) const -> int {
            if (p == &nodes.front())  return 0;
            if (node_begin >= begin and node_end <= end)  return p->sum;
            auto node_mid = std::midpoint(node_begin, node_end);
            auto res = 0;
            if (begin < node_mid)  res += sumRange(begin, end, p->left, node_begin, node_mid);
            if (node_mid < end)  res += sumRange(begin, end, p->right, node_mid, node_end);
            return res;
        }
        auto sumRange(int begin, int end) const -> int {
            return sumRange(begin, end, root, root_begin, root_end);
        }
        auto maxRange(int begin, int end, Node *p, int node_begin, int node_end) const -> int {
            if (p == &nodes.front())  return 0;
            if (node_begin >= begin and node_end <= end)  return p->max;
            auto node_mid = std::midpoint(node_begin, node_end);
            auto res = -inf;
            if (begin < node_mid)  chkMax(res, maxRange(begin, end, p->left, node_begin, node_mid));
            if (node_mid < end)  chkMax(res, maxRange(begin, end, p->right, node_mid, node_end));
            return res;
        }
        auto maxRange(int begin, int end) const -> int {
            return maxRange(begin, end, root, root_begin, root_end);
        }
    };
    class Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> depth, size, fa, son, top, order, index;
        std::vector<SegTree> sgts;
        std::vector<int> keys;  // 第 i 个城市的宗教

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
        Tree(int N, int C): N(N), graph(N+1), depth(N+1), size(N+1), fa(N+1), son(N+1), 
                top(N+1), index(N+1), sgts(C+1, {0, N}), keys(N+1) {}
        auto assignPointKey(int p, int new_key) -> void {
            auto cur_key = keys[p];
            auto cur_val = sgts[cur_key].sumRange(index[p], index[p] + 1);
            sgts[cur_key].assignAt(index[p], 0);
            sgts[new_key].assignAt(index[p], cur_val);
            keys[p] = new_key;
        }
        auto assignPointValue(int p, int val) -> void {
            sgts[keys[p]].assignAt(index[p], val);
        }
        // 路径上所有 key 相同的点权求和
        auto sumPathValue(int x, int y) -> int {
            auto res = 0;
            auto key = keys[x];
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                res += sgts[key].sumRange(index[top[x]], index[x] + 1);
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            res += sgts[key].sumRange(index[y], index[x] + 1);
            return res;
        }
        // 路径上所有 key 相同的点权求最大值
        auto maxPathValue(int x, int y) -> int {
            auto res = -inf;
            auto key = keys[x];
            while (top[x] != top[y]) {
                if (depth[top[x]] < depth[top[y]]) {
                    std::swap(x, y);
                }
                chkMax(res, sgts[key].maxRange(index[top[x]], index[x] + 1));
                x = fa[top[x]];
            }
            if (depth[x] < depth[y])  std::swap(x, y);
            chkMax(res, sgts[key].maxRange(index[y], index[x] + 1));
            return res;
        }
        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            std::vector<std::pair<int, int>> vector(N);
            for (auto &[x, y]: vector)  std::cin >> x >> y;
            for (auto _: range(N-1)) {
                int x, y;  std::cin >> x >> y;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            tr.dfs1(1, 0);
            tr.dfs2(1, 1);
            debug {
                std::cout << "order: ";
                for (auto x: tr.order)  std::cout << x << " ";
                std::cout << std::endl;
            }
            for (auto i: range(vector.size())) {
                auto [value, key] = vector[i];
                tr.assignPointKey(i + 1, key);
                tr.assignPointValue(i + 1, value);
            }
            return st;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        Tree tree{N, 100001};  std::cin >> tree;
        for (auto _: range(M)) {
            std::string op;  std::cin >> op;
            int x, y;  std::cin >> x >> y;
            if (op == "CC") {
                tree.assignPointKey(x, y);
            } else if (op == "CW") {
                tree.assignPointValue(x, y);
            } else if (op == "QS") {
                std::cout << tree.sumPathValue(x, y) << endl;
            } else {
                std::cout << tree.maxPathValue(x, y) << endl;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4828700238829491::solve();
    return 0;
}
