/**
 * @link
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

namespace Solution_6314180276493067 {
    template <typename T>
    auto constexpr bit_width(T x) -> i32 {
        return std::__lg(x) + 1;
    }
    template <typename Comp = std::less<>>
    class SparseTable {
        i32 n, logN;
        std::vector<std::vector<i32>> f;
        Comp cmp;
    public:
        SparseTable(i32 n, std::vector<i32> const &a, Comp cmp = {}): 
                n(n), logN(bit_width<u32>(n)), f(logN, std::vector<i32>(n)), cmp(std::move(cmp)) {
            if (n == 0) return;
            std::copy(a.begin(), a.end(), f[0].begin());
            for (i32 t = 1; t != logN; t++) {
                for (i32 i = 0; i < n; i++) {
                    auto j = i + (1 << t);
                    if (j > n) break;

                    f[t][i] = std::min(f[t - 1][i], f[t - 1][i + (1 << (t - 1))], cmp);
                }
            }
        }

        auto minRange(i32 begin, i32 end) {
            auto log = bit_width<u32>(end - begin) - 1;
            return std::min(f[log][begin], f[log][end - (1 << log)], cmp);
        }
    };
    class Graph {
        i32 n;
        std::vector<std::vector<i32>> graph;
        std::vector<i32> depth, order, index, fa;
        i32 root;

        struct SparseTableCompare {
            std::vector<i32> const *depth = nullptr;
            SparseTableCompare() {}
            explicit SparseTableCompare(std::vector<i32> const *depth): depth(depth) {}
            auto operator() (i32 x, i32 y) const -> bool {
                assert(depth != nullptr);
                return (*depth)[x] < (*depth)[y];
            }
        };

        SparseTable<SparseTableCompare> minST;

        auto dfs(i32 u, i32 prev) -> void {
            depth[u] = depth[prev] + 1;
            index[u] = static_cast<i32>(order.size());
            fa[u] = prev;
            order.push_back(u);
            for (auto v: graph[u]) {
                if (v == prev) continue;
                dfs(v, u);
            }
        }
    public:
        Graph(i32 n, std::vector<std::pair<i32, i32>> const &edges, i32 root): 
                n(n), graph(n + 1), depth(n + 1), order(), index(n + 1), fa(n + 1),
                root(root), minST(0, {}, {}) {
            for (auto [x, y]: edges) {
                graph[x].push_back(y);
                graph[y].push_back(x);
            }
            dfs(root, 0);

            minST = SparseTable<SparseTableCompare>(n, order, SparseTableCompare{&depth});
        }
        auto lca(i32 u, i32 v) -> i32 {
            if (u == v) return u;
            if (index[u] > index[v]) std::swap(u, v);
            auto min = minST.minRange(index[u] + 1, index[v] + 1);
            return fa[min];
        }
    };
    auto solve() -> void {
        i32 n, k;
        std::cin >> n >> k;

        std::vector<std::pair<i32, i32>> edges(n - 1);
        std::vector<char> isChild(n + 1, false); 
        isChild[0] = true;
        for (auto &[x, y]: edges) std::cin >> x >> y, isChild[y] = true;

        i32 root = std::find(isChild.begin(), isChild.end(), false) - isChild.begin();

        Graph graph(n, edges, root);
        for (auto _ = k; _ --> 0; ) {
            i32 u, v;
            std::cin >> u >> v;

            std::cout << graph.lca(u, v) << endl;
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_6314180276493067::solve();
    return 0;
}
