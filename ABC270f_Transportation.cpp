/**
 * @link https://www.luogu.com.cn/problem/AT_abc270_f
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

#include "./libs/fixed_int.hpp"

/**
 * 两个点之间可以选择：
 * 两个机场，两个港口，一条边
 * 
 * 机场和港口可以认为是连向一个虚拟的点
 */
namespace Solution_1554821428817021 {
    class DSU {
    public:
        std::vector<i32> fa;
        std::vector<i32> size;
        DSU(int N): fa(N+1), size(N+1, 1) {
            std::iota(fa.begin(), fa.end(), 0);
        }

        auto find(int x) -> int {
            if (fa[x] == x)  return x;
            else  return (fa[x] = find(fa[x]));
        }

        auto connect(int x, int y) -> void {
            auto a = find(x), b = find(y);
            if (a == b)  return;
            if (size[a] < size[b])  std::swap(a, b), std::swap(x, y);
            size[a] += size[b], fa[b] = a;
        }
    };
    template <typename T>
    auto operator+ (std::vector<T> const &a, std::vector<T> const &b) -> std::vector<T> {
        auto res = a;
        for (auto x: b)  res.push_back(x);
        return res;
    }
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        struct Edge { int x, y, val; };
        std::vector<i32> airport_cost(N);
        std::vector<i32> port_cost(N);
        std::vector<Edge> bridges(M);

        for (auto &x: airport_cost)  std::cin >> x;
        for (auto &x: port_cost)  std::cin >> x;
        for (auto &[x, y, val]: bridges)  std::cin >> x >> y >> val;

        auto airport_vp = N + 1, port_vp = N + 2;
        
        std::vector<Edge> edges(bridges);
        std::vector<Edge> edges1;
        std::vector<Edge> edges2;
        for (auto i = 0; i < N; i++)  edges1.push_back({i+1, airport_vp, airport_cost[i]});
        for (auto i = 0; i < N; i++)  edges2.push_back({i+1, port_vp, port_cost[i]});
        
        auto get = [&](auto &&edges, i32 expected_size) -> i64 {
            DSU dsu{N+2};
            ranges::sort(edges, std::less{}, lam(x, x.val));
            i64 ans = 0;
            for (auto [x, y, val]: edges) {
                if (dsu.find(x) == dsu.find(y))  continue;
                dsu.connect(x, y), ans += val;
            }
            if (ranges::max(dsu.size) < expected_size)  return infLL;
            else  return ans;
        };

        auto ans = std::min({get(edges, N), get(edges + edges1, N+1), get(edges + edges2, N+1), get(edges + edges1 + edges2, N+2)});
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1554821428817021::solve();
    return 0;
}
