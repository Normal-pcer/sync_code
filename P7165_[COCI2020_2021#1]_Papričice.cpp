/**
 * @link https://www.luogu.com.cn/problem/P7165
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 遍历整棵树，对于节点 P，将 P 与父亲之间的边删除
 * 将 P 的子树作为一块，其余的分成两块
 * 考虑第二个断点 Q 的位置：
 * 如果另一处断点在 P 的祖先节点，此时 Q 的子树包含 P 的子树，将 Q 的子树以外视为一块。
 * 否则，将 Q 的子树本身以外视为一块。
 * 
 * 希望：
 * - Q 是 P 的祖先，size[Q] = (N + size[P]) / 2
 * - Q 不是 P 的祖先，size[Q] = (N - size[P]) / 2
 * 分别记录当前 P 祖先的 size 和其他点的 size，二分查找最近的元素即可
 */
namespace Solution_1742502056898965 {
    class Tree {
        int N;
        std::vector<std::vector<int>> graph;
        std::vector<int> size;
    public:
        Tree(int N): N(N), graph(N+1), size(N+1) {}

        auto dfs1(int p, int prev) -> void {
            size[p] = 1;
            for (auto x: graph.at(p))  if (x != prev) {
                dfs1(x, p);
                size[p] += size[x];
            }
        }
        
        /**
         * @param min 当前找到的最小答案
         * @param ancestor 所有祖先的 size 组成的栈
         * @param other 除了祖先以外，所有的走过的点的 size
         */
        auto _getAnswer(int &min, std::vector<int> &ancestor, std::multiset<int> &other, int p, int prev, int dep) -> void {
            debug {
                std::cout << "min: " << min << std::endl;
                std::cout << "ancestor: ";
                for (auto x: ancestor)  std::cout << x << " ";
                std::cout << std::endl;
                std::cout << "other: ";
                for (auto x: other)  std::cout << x << " ";
                std::cout << std::endl;
                std::cout << std::format("p: {}, prev: {}, depth: {}", p, prev, dep) << std::endl;
                std::cout << "----" << std::endl;
            }
            // 在祖先中查找
            // 保证 ancestor 单调递减
            {
                auto f = [&](int sz_q) -> void {
                    debug  std::cout << "f_ancestor " << sz_q << std::endl;
                    auto a = size[p], b = sz_q - size[p], c = N - sz_q;
                    auto [cur_min, cur_max] = std::minmax({a, b, c});
                    auto cur = cur_max - cur_min;
                    chkMin(min, cur);
                };
                auto it = ranges::lower_bound(ancestor, static_cast<double>(N + size[p]) / 2, std::greater{});
                if (it != ancestor.end())  f(*it);
                if (it != ancestor.begin())  f(*std::prev(it));
            }
            {
                auto f = [&](int sz_q) -> void {
                    debug  std::cout << "f_other " << sz_q << std::endl;
                    auto a = size[p], b = sz_q, c = N - sz_q - size[p];
                    auto [cur_min, cur_max] = std::minmax({a, b, c});
                    auto cur = cur_max - cur_min;
                    chkMin(min, cur);
                };
                auto it = other.lower_bound(static_cast<double>(N - size[p]) / 2);
                if (it != other.end())  f(*it);
                if (it != other.begin())  f(*std::prev(it));
            }
            ancestor.push_back(size[p]);
            for (auto x: graph.at(p))  if (x != prev) {
                _getAnswer(min, ancestor, other, x, p, dep + 1);
            }
            other.insert(size[p]);
            ancestor.pop_back();
        }
        auto getAnswer() -> int {
            int min = inf;
            std::vector<int> ancestor;
            std::multiset<int> other {N};
            _getAnswer(min, ancestor, other, 1, 0, 0);
            return min;
        }
        friend auto operator>> (std::istream &st, Tree &tr) -> std::istream & {
            auto N = tr.N;
            for (auto _: range(N-1)) {
                int x, y;  std::cin >> x >> y;
                tr.graph.at(x).push_back(y);
                tr.graph.at(y).push_back(x);
            }
            tr.dfs1(1, 0);
            return st;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        Tree tree{N};  std::cin >> tree;
        std::cout << tree.getAnswer() << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1742502056898965::solve();
    return 0;
}
