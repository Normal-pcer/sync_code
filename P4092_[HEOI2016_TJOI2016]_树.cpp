/**
 * @link https://www.luogu.com.cn/problem/P4092
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_4174522909328179 {
    class Tree {
        std::vector<std::vector<int>> forward;
        std::vector<int> depth, size, order, index;
        struct SegTreeNode {
            int begin, end;
            int deep, tag;
        };
        std::vector<SegTreeNode> tr;
    public:
        Tree(int N, std::vector<std::pair<int, int>> const &edges): forward(N+1), depth(N+1), size(N+1), index(N+1), tr((N+1) << 2) {
            for (auto [x, y]: edges)  forward.at(x).push_back(y), forward.at(y).push_back(x);
            dfs(1, 0);
            segTreeBuild(0, N, 1);
        }
        void dfs(int p, int prev = 0) {
            size[p] = 1;
            depth[p] = depth[prev] + 1;
            index.at(p) = order.size(), order.push_back(p);
            for (auto x: forward.at(p))  if (x != prev) {
                dfs(x, p);
                size[p] += size[x];
            }
        }
#define ls (p << 1)
#define rs (p << 1 | 1)
        void checkDeep(int &base, int cmp) {
            if (depth[base] < depth[cmp]) {
                base = cmp;
            }
        }
        void segTreeUpdateNode(int p, int val) {
            checkDeep(tr[p].deep, val);
            checkDeep(tr[p].tag, val);
        }
        void segTreePushDown(int p) {
            if (tr[p].tag != 0) {
                for (auto s: {ls, rs}) {
                    segTreeUpdateNode(s, tr[p].tag);
                }
                tr[p].tag = 0;
            }
        }
        void segTreeBuild(int begin, int end, int p = 1) {
            tr[p].begin = begin, tr[p].end = end;
            if (tr[p].begin + 1 == tr[p].end) {
                tr[p].deep = 1;
                return;
            }
            auto mid = std::midpoint(begin, end);
            segTreeBuild(begin, mid, ls);
            segTreeBuild(mid, end, rs);
        }
        void segTreeUpdateRange(int begin, int end, int val, int p = 1) {
            if (tr[p].begin >= begin and tr[p].end <= end) {
                segTreeUpdateNode(p, val);
                return;
            }
            segTreePushDown(p);
            if (tr[ls].end > begin)  segTreeUpdateRange(begin, end, val, ls);
            if (tr[rs].begin < end)  segTreeUpdateRange(begin, end, val, rs);
        }
        int segTreeDeepPoint(int pos, int p = 1) {
            if (tr[p].begin >= pos and tr[p].end <= pos + 1) {
                return tr[p].deep;
            }
            segTreePushDown(p);
            if (tr[ls].end > pos)  return segTreeDeepPoint(pos, ls);
            else  return segTreeDeepPoint(pos, rs);
        }
        void mark(int p) {
            auto begin = index[p];
            auto end = begin + size[p];
            segTreeUpdateRange(begin, end, p);
        }
        int query(int p) {
            return segTreeDeepPoint(index[p]);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        std::vector<std::pair<int, int>> edges(N-1);
        for (auto &[x, y]: edges)  std::cin >> x >> y;
        Tree tree(N, edges);
        for (auto _ = 0; _ < Q; _++) {
            char op;  int x;
            std::cin >> op >> x;
            if (op == 'Q') {
                std::cout << tree.query(x) << endl;
            } else {
                tree.mark(x);
            }
        }
    }
#undef ls
#undef rs
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4174522909328179::solve();
    return 0;
}
