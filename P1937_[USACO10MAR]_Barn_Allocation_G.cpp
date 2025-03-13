/**
 * @link https://www.luogu.com.cn/problem/P1937
 */
#ifndef ONLINE_JUDGE 
#define GNU_DEBUG 
#define _GLIBCXX_DEBUG 1
#define _GLIBCXX_DEBUG_PEDANTIC 1
#define _GLIBCXX_SANITIZE_VECTOR 1
#endif
#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 感觉可以用一个类似 P11232 的贪心策略：
 * 按照区间右端点排序，每次放一个区间右端点最靠左（且不会冲突）的。
 * 
 * 感觉这样是合理的。例如现在策略中相邻排放的两个区间，如果想要变成三个，必然有一个的右端点在当前选择的左侧。
 * 
 * 接下来考虑怎么实现。维护一个格子还有多少剩余空间。每次放置一头牛相当于区间减少 1。
 * 如果这个区间有一个格子已经是 0，那么这头牛必然会发生冲突。
 * 使用线段树维护操作：区间加，区间求最小值。
 * 
 * 可爱。没有标记就下传标记。
 */
namespace Solution_3475458810815399 {
    class SegTree {
        struct Node {
            i32 begin, end;
            i32 min = inf, add_tag = 0;
        };

        std::vector<Node> tree;
        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }

        auto pushUp(i32 p) -> void {
            tree[p].min = std::min(tree[lson(p)].min, tree[rson(p)].min);
        }

        auto pushDown(i32 p) -> void {
            // 这里原先写的 tree[p].add_tag == 0。
            if (tree[p].add_tag != 0) {
                for (auto s: {lson(p), rson(p)}) {
                    tree[s].min += tree[p].add_tag;
                    tree[s].add_tag += tree[p].add_tag;
                }
                tree[p].add_tag = 0;
            }
        }
        template <typename T>
        auto build(i32 begin, i32 end, T const &init, i32 p) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end) {
                tree[p].min = init.at(begin);
                return;
            }
            auto mid = (begin + end) >> 1;
            build(begin, mid, init, lson(p));
            build(mid, end, init, rson(p));
            pushUp(p);
        }
    public:
        template <typename T>
        SegTree(i32 N, T const &init): tree((N + 1) << 2) {
            build(0, N, init, 1);
        }
        auto minRange(i32 begin, i32 end, i32 p = 1) -> i32 {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p].min;
            }
            pushDown(p);
            i32 res = inf;
            if (tree[lson(p)].end > begin)  chkMin(res, minRange(begin, end, lson(p)));
            if (tree[rson(p)].begin < end)  chkMin(res, minRange(begin, end, rson(p)));
            return res;
        }
        auto addRange(i32 begin, i32 end, i32 val, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].min += val, tree[p].add_tag += val;
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin)  addRange(begin, end, val, lson(p));
            if (tree[rson(p)].begin < end)  addRange(begin, end, val, rson(p));
            pushUp(p);
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<i32> init(N);
        for (auto &x: init)  std::cin >> x;

        SegTree sgt{N, init};
        
        std::vector<std::pair<i32, i32>> edges(M);
        for (auto it = edges.begin(); it != edges.end(); it++) {
            std::cin >> it->first >> it->second, it->first--;
        }
        std::sort(edges.begin(), edges.end(), lam(x, y, x.second < y.second));

        i32 ans = 0;
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            i32 x, y;  std::tie(x, y) = *it;
            // 检查是否会冲突
            auto conflict = sgt.minRange(x, y) == 0;
            if (not conflict) {
                sgt.addRange(x, y, -1);
                ans++;
            }
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3475458810815399::solve();
    return 0;
}
