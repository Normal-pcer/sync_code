/**
 * @link https://www.luogu.com.cn/problem/P7162
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 感性理解，希望最先把点权大的点分离出来。
 * 猜测：优先断 点权最大的点上的边。
 * 考虑将删边改为加边，可以使用并查集维护左右两个连续块的最大值。
 * 先找到点权最小的点，然后按照点权排序，依次加边（只加上点权小于自己的）
 */
namespace Solution_1678264762810399 {
    class DSU {
        std::vector<int> F;
        std::vector<int> size;
    public:
        DSU(int N): F(N+1), size(N+1, 1) {
            std::iota(F.begin(), F.end(), 0);
        }
        auto find(int x) -> int {
            if (F[x] == x)  return x;
            else  return (F[x] = find(F[x]));
        }
        auto merge(int x, int y) -> void {
            auto a = find(x), b = find(y);
            if (a == b)  return;
            if (size[a] < size[b])  std::swap(x, y), std::swap(a, b);
            size[a] += size[b], F[b] = a;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> val(N+1);
        for (auto &x: val | views::drop(1))  std::cin >> x;
        std::vector<std::vector<int>> graph(N+1);
        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        DSU dsu{N};
        std::vector<int> order(N);
        std::vector<char> vis(N+1);
        std::iota(order.begin(), order.end(), 1);
        ranges::sort(order, std::less{}, lam(x, val[x]));
        
        std::vector<int> max(val);  // 连通块中的最大点权
        auto ans = 0LL;
        for (auto x: order) {
            vis[x] = true;
            ranges::sort(graph[x], std::less{}, lam(x, val[x]));
            for (auto y: graph[x]) {
                if (not vis[y])  continue;
                assert(dsu.find(x) != dsu.find(y));
                auto left = max.at(dsu.find(x));
                auto right = max.at(dsu.find(y));
                auto cur = left + right;  // 这一次加边的代价
                ans += cur;
                dsu.merge(x, y);
                max[dsu.find(x)] = std::max(left, right);
            }
        }
        std::cout << ans << endl;
    }
}

int main(int dyy_forever, char const *lmy_forever[]) {
    DEBUG_MODE = (dyy_forever-1) and not strcmp("-d", lmy_forever[1]);
    Solution_1678264762810399::solve();
    return 0;
}
