/**
 * @link https://www.luogu.com.cn/problem/P3834
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_1557509260852697 {
    class SegTree {
        struct Node {
            i32 sum = 0;
            i32 lson = 0, rson = 0;
        };

        std::deque<Node> tree;
        std::vector<i32> root;  // 所有版本的根节点
        i32 root_begin, root_end;

        auto pushUp(i32 p) -> void {
            tree[p].sum = tree[tree[p].lson].sum + tree[tree[p].rson].sum;
        }
        auto addAt(i32 pos, i32 val, i32 &p, i32 node_begin, i32 node_end) -> void {
            tree.push_back(tree[p]), p = static_cast<i32>(tree.size()) - 1;
            if (node_begin + 1 == node_end) {
                tree[p].sum += val;
                return;
            }
            auto node_mid = std::midpoint(node_begin, node_end);
            if (pos < node_mid)  addAt(pos, val, tree[p].lson, node_begin, node_mid);
            else  addAt(pos, val, tree[p].rson, node_mid, node_end);
            pushUp(p);
        }
        auto lowerBound(i32 val, i32 p, i32 pMinus, i32 node_begin, i32 node_end) -> i32 {
            debug  std::cout << std::format("lowerBound {} {} {} {} {}", val, p, pMinus, node_begin, node_end) << std::endl;
            if (node_begin + 1 == node_end) {
                auto node_val = tree[p].sum - tree[pMinus].sum;
                if (node_val >= val)  return node_begin;
                else  return node_end;
            }
            auto node_mid = std::midpoint(node_begin, node_end);
            auto left_val = tree[tree[p].lson].sum - tree[tree[pMinus].lson].sum;
            if (left_val >= val)  return lowerBound(val, tree[p].lson, tree[pMinus].lson, node_begin, node_mid);
            else  return lowerBound(val - left_val, tree[p].rson, tree[pMinus].rson, node_mid, node_end);
        }
    public:
        SegTree(i32 N): root_begin(0), root_end(N + 1) {
            tree.push_back({});
        }
        auto addAt(i32 pos, i32 val) -> i32 {
            root.push_back(root.empty()? 0: root.back());
            addAt(pos, val, root.back(), root_begin, root_end);
            return static_cast<i32>(root.size()) - 1;
        }
        // (vMinus, v] 这个区间内所有版本的差异
        // 在这上面查找第一个 val
        auto lowerBound(i32 val, i32 v, i32 vMinus) -> i32 {
            return lowerBound(val, root[v], root[vMinus], root_begin, root_end);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<i32> init(N+1);
        for (auto &x: init | views::drop(1))  std::cin >> x;

        i32 constexpr maxValue = 1e9;
        SegTree sgt{maxValue};
        for (auto x: init)  sgt.addAt(x, +1);

        for (auto _ = M; _ --> 0; ) {
            i32 l, r, k;  std::cin >> l >> r >> k;
            // 第 i 个版本表示第 i 个数被插入之后
            // [l, r] 即 (l - 1, r] 的差异
            auto ans = sgt.lowerBound(k, r, l - 1);
            std::cout << ans << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1557509260852697::solve();
    return 0;
}
