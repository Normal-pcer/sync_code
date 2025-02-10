/**
 * @link https://www.luogu.com.cn/problem/P9912
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 每一个位置，可以设置成 1 表示没有被淹没，0 表示已经被淹没。
 * 岛屿数量就是 1 的连续段数量
 * 线段树维护即可
 * 修改，每次水位上涨都会对应若干个点的修改，总修改数是 O(N)。（一个 1 变成 0 就变不回来了）
 */
namespace Solution_1017317767519767 {
    class SegTree {
        struct Node {
            int begin = 0, end = 0;
            bool left = true, right = true;  // 左端点和右端点的颜色
            int count = 1;

            auto empty() const -> bool { return begin == end; }
            auto assign(bool val) -> void {
                left = right = val;
                count = val;
            }
            auto operator| (const Node &other) const -> Node {
                if (empty())  return other;
                if (other.empty())  return *this;
                Node res;
                res.begin = begin, res.end = other.end;
                res.left = left, res.right = other.right;
                res.count = count + other.count - (right and other.left);  // 可以拼在两块一起，相当于 -1
                return res;
            }
        };
        std::vector<Node> tree;

        static constexpr auto lson(int p) -> int { return p << 1; }
        static constexpr auto rson(int p) -> int { return p << 1 | 1; }

        auto build(int begin, int end, int p) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (begin + 1 == end)  return;
            auto mid = std::midpoint(begin, end);
            build(begin, mid, lson(p)), build(mid, end, rson(p));
            tree[p] = tree[lson(p)] | tree[rson(p)];
        }
    public:
        SegTree(int N): tree(N << 2) {
            build(0, N, 1);
        }
        auto queryRange(int begin, int end, int p = 1) -> Node {
            debug  std::cout << std::format("query {} {}", begin, end) << std::endl;
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p];
            }
            Node res;
            if (tree[lson(p)].end > begin)  res = res | queryRange(begin, end, lson(p));
            if (tree[rson(p)].begin < end)  res = res | queryRange(begin, end, rson(p));
            return res;
        }
        auto assignAt(int begin, int val, int p = 1) -> void {
            debug  std::cout << std::format("assign {} {}", begin, val) << std::endl;
            if (tree[p].begin >= begin and tree[p].end <= begin + 1) {
                tree[p].assign(val);
                return;
            }
            if (tree[lson(p)].end > begin)  assignAt(begin, val, lson(p));
            else  assignAt(begin, val, rson(p));
            tree[p] = tree[lson(p)] | tree[rson(p)];
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        std::vector<int> h(N);
        for (auto &x: h)  std::cin >> x;
        std::vector<std::tuple<int, int, int, int>> queries(Q);
        for (auto i = 0; auto &[x, y, z, index]: queries)  std::cin >> x >> y >> z, index = i++;
        ranges::sort(queries, lam(x, y, std::get<2>(x) < std::get<2>(y)));  // 高度排序

        // 接下来的每一次，处理出这一次上涨会掩盖哪些地面上的点
        std::vector<std::pair<int, int>> height_point(N);
        for (auto i: range(N))  height_point[i] = {h[i], i};
        ranges::sort(height_point);  // 高度排序
        
        SegTree sgt{N};
        auto it = height_point.begin();
        auto cur_h = 0;
        std::vector<int> answers(Q);
        for (auto [l, r, new_h, index]: queries) {
            assert(cur_h <= new_h);
            l--, r--;  // 从 0 开始编号
            cur_h = new_h;
            while (it != height_point.end() and it->first <= cur_h)  sgt.assignAt(it->second, false), it++;  // 被淹了
            auto ans = sgt.queryRange(l, r+1).count;
            answers[index] = ans;
        }

        for (auto x: answers)  std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1017317767519767::solve();
    return 0;
}
