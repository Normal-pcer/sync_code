/**
 * @link https://www.luogu.com.cn/problem/P3120
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * F[i][j] 表示到达点 (i, j) 的方案数。
 * 
 * 直接转移是直接把所有左上角的点求和。
 * 要求不等于这个点，可以拆分成所有点减去等于这个点。
 * 前者可以直接维护前缀和，后者可能可以开 C 棵动态开点线段树维护。（否则直接开前缀和空间会炸）
 * 
 * 循环遍历到第 i 行，sgt[c] 线段树的第 j 项表示 F[i][j]（color = c）或 0。
 */
namespace Solution_8987557215580481 {
    i32 constexpr mod = 1'000'000'007;
    class SegTree {
    public:
        struct Node {
            i32 sum = 0;
            i32 lson = 0, rson = 0;
        };
    private:
        std::deque<Node> static nodes;

        i32 root = 0;
        i32 root_begin = 0, root_end = 0;
        auto pushUp(i32 p) -> void {
            nodes[p].sum = (nodes[nodes[p].lson].sum + nodes[nodes[p].rson].sum) % mod;
        }
        auto sumRange(i32 begin, i32 end, i32 p, i32 node_begin, i32 node_end) -> i32 {
            if (p == 0)  return 0;
            if (node_begin >= begin and node_end <= end) {
                return nodes[p].sum;
            }
            i32 res = 0;
            auto node_mid = std::midpoint(node_begin, node_end);
            if (node_mid > begin)  res = (res + sumRange(begin, end, nodes[p].lson, node_begin, node_mid)) % mod;
            if (node_mid < end)  res = (res + sumRange(begin, end, nodes[p].rson, node_mid, node_end)) % mod;
            return res;
        }
        auto addAt(i32 pos, i32 val, i32 &p, i32 node_begin, i32 node_end) -> void {
            if (p == 0)  p = nodes.size(), nodes.push_back({});
            if (node_begin + 1 == node_end) {
                assert(pos == node_begin);
                nodes[p].sum = (nodes[p].sum + val) % mod;
                return;
            }
            auto node_mid = std::midpoint(node_begin, node_end);
            if (node_mid > pos)  addAt(pos, val, nodes[p].lson, node_begin, node_mid);
            else  addAt(pos, val, nodes[p].rson, node_mid, node_end);
            pushUp(p);
        }
    public:
        SegTree(i32 N): root_begin(0), root_end(N) {}

        auto sumRange(i32 begin, i32 end) -> i32 {
            return sumRange(begin, end, root, root_begin, root_end);
        }
        auto addAt(i32 pos, i32 val) -> void {
            addAt(pos, val, root, root_begin, root_end);
        }
    };
    std::deque<SegTree::Node> SegTree::nodes = {{}};
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 R, C, K;  std::cin >> R >> C >> K;
        std::vector mat(R, std::vector<i32>(C));

        i32 max_color = K;
        for (auto &line: mat) {
            for (auto &item: line)  std::cin >> item, chkMax(max_color, item);
        }

        std::vector F(R, std::vector<i32>(C));
        F[0][0] = 1;

        std::vector colors(max_color + 1, SegTree{C});
        SegTree all{C};
        // all.addAt(0, 1);
        // colors[mat[0][0]].addAt(0, 1);
        for (i32 i = 1; i < R; i++) {
            for (i32 j = 0; j < C; j++) {
                auto total = all.sumRange(0, j);
                auto extra = colors[mat[i][j]].sumRange(0, j);
                F[i][j] = (total - extra + mod) % mod;
                colors[mat[i - 1][j]].addAt(j, F[i - 1][j]);
                all.addAt(j, F[i - 1][j]);
            }
            debug {
                for (i32 j = 0; j < C; j++) {
                    std::cout << all.sumRange(j, j + 1) << " ";
                }
                std::cout << std::endl;
            }
        }

        auto ans = F[R - 1][C - 1];
        debug {
            for (i32 i = 0; i < R; i++) {
                for (i32 j = 0; j < C; j++) {
                    std::cout << "F[" << i << "][" << j << "] = " << F[i][j] << std::endl;
                }
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8987557215580481::solve();
    return 0;
}
