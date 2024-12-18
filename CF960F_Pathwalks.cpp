/**
 * @link https://www.luogu.com.cn/problem/CF960F
 * @link https://codeforces.com/problemset/problem/960/F
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_3216920462628721 {
    // 动态开点线段树
    class SegTree {
        struct Node {
            int max;
            int ls = 0, rs = 0;
        };
        static std::deque<Node> nodes;

        int root, root_begin, root_end;

        void pushUp(int p) {
            auto ls = nodes[p].ls, rs = nodes[p].rs;
            nodes[p].max = std::max(nodes[ls].max, nodes[rs].max);
        }
        int max(int begin, int end, int n_begin, int n_end, int &p) {
            if (p == 0)  return 0;
            if (n_begin >= begin and n_end <= end) {
                return nodes[p].max;
            }
            auto n_mid = std::midpoint(n_begin, n_end);
            auto res = 0;
            if (begin < n_mid)  chkMax(res, max(begin, end, n_begin, n_mid, nodes[p].ls));
            if (n_mid < end)  chkMax(res, max(begin, end, n_mid, n_end, nodes[p].rs));
            return res;
        }
        void update(int pos, int val, int n_begin, int n_end, int &p) {
            if (p == 0)  p = nodes.size(), nodes.push_back({val});
            if (n_begin == pos and pos + 1 == n_end) {
                chkMax(nodes[p].max, val);
                return;
            }
            auto n_mid = std::midpoint(n_begin, n_end);
            if (pos < n_mid)  update(pos, val, n_begin, n_mid, nodes[p].ls);
            else  update(pos, val, n_mid, n_end, nodes[p].rs);
            pushUp(p);
        }
    public:
        SegTree(int begin, int end): root(0), root_begin(begin), root_end(end) {}
        int max(int begin, int end) {
            return max(begin, end, root_begin, root_end, root);
        }
        void update(int pos, int val) {
            update(pos, val, root_begin, root_end, root);
        }
    };
    std::deque<SegTree::Node> SegTree::nodes {{0}};

    struct Edge {
        int tail, head;
        int val;
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        auto max_v = 0;
        std::vector<Edge> edges(M);
        for (auto &[a, b, v]: edges)  std::cin >> a >> b >> v, chkMax(max_v, v);

        std::vector trees(N+1, SegTree(0, max_v+1));  // 维护 head 为 i 的点
        auto ans = 0;
#if false
        std::vector<int> F(M);
        for (auto i: range(M)) {
            F[i] = 1;
            for (auto j: range(i)) {
                if (edges[j].val < edges[i].val and edges[j].head == edges[i].tail) {
                    chkMax(F[i], F[j]+1);
                }
            }
            chkMax(ans, F[i]);
        }
#endif
        for (auto i: range(M)) {
            auto max = 1;
            auto &tree = trees.at(edges[i].tail);
            chkMax(max, tree.max(0, edges[i].val)+1);
            trees.at(edges[i].head).update(edges[i].val, max);
            debug  std::cout << std::format("F[{}] = {}", i, max) << std::endl;
            chkMax(ans, max);
        }

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3216920462628721::solve();
    return 0;
}
