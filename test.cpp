/**
 * @link
 */
#include "./lib_v7.hpp"
#include "./libs/fixed_int.hpp"
#include "./randlib.hpp"

using namespace lib;
using namespace Random;

namespace Solution_6314180276493067 {
    class Graph {
    public:
        i32 root;
        std::vector<std::set<i32>> graph;
        std::vector<i32> parent;
        Graph(i32 n, std::vector<std::pair<i32, i32>> const &edges): graph(n + 1), parent(n + 1) {
            std::vector<char> hasParent(n + 1);
            for (auto [child, parent]: edges) {
                hasParent[child] = true;
                graph[child].insert(parent);
                graph[parent].insert(child);
            }

            root = static_cast<i32>(
                ranges::find(hasParent.begin() + 1, hasParent.end(), false) - hasParent.begin());

            dfs(root, 0);
        }
        auto merge(i32 x, i32 y) -> void {
            assert(x < y);

            // x 的儿子合并到 y 上
            for (auto v: graph[x]) {
                if (v == parent[x]) continue;
                graph[y].insert(v);
                graph[v].erase(x);
                graph[v].insert(y);
                parent[v] = y;
            }
            graph[x].clear();
        }
        auto getEdges() -> auto {
            std::vector<std::pair<i32, i32>> ans;
            getEdgesRecursion(root, 0, ans);
            return ans;
        }
    private:
        auto dfs(i32 u, i32 fa) -> void {
            parent[u] = fa;
            for (auto v: graph[u]) {
                if (v == fa) continue;
                dfs(v, u);
            }
        }
        auto getEdgesRecursion(i32 u, i32 prev, std::vector<std::pair<i32, i32>> &out) -> void {
            for (auto v: graph[u]) {
                if (v == prev) continue;
                out.push_back({v, u});
                getEdgesRecursion(v, u, out);
            }
        }
    };
    auto constexpr executable = u8"P9191_[USACO23OPEN]_Tree_Merging_G.exe";
    auto solve() -> void {
        i32 constexpr maxN = 10;
        std::fstream fout{std::filesystem::path{u8"2.in"}, std::ios::out | std::ios::binary};
        auto &out = fout;
        // out << 1 << endl;
        auto n = maxN;

        out << n << endl;
        std::vector<std::pair<i32, i32>> edges;
        for (i32 i = 2; i <= n; i++) {
            auto prev = randint(1, i - 1);
            edges.push_back({i, prev});
            out << i << ' ' << prev << endl;
        }

        Graph g{n, edges};
        auto cp = g;
        i32 m = 0;
        for (i32 _ = n; _ --> 0; ) {
            i32 p = randint(1, n);
            auto son = g.graph[p] | views::filter(lam(x, x != g.parent[p])) | ranges::to<std::vector<i32>>();
            if (son.size() >= 2) {
                auto tmp = sample(son, 2);
                auto x = tmp[0], y = tmp[1];
                assert(x != y);
                if (x > y) std::swap(x, y);
                m++;
                g.merge(x, y);
                g.graph[p].erase(x);
                // std::cerr << "merge: " << x << ", " << y << endl;
            }
        }

        out << n - m << endl;
        auto newEdges = g.getEdges();
        for (auto [x, y]: newEdges) {
            out << x << ' ' << y << endl;
        }
        
        out.close();
        // std::getchar();

        // std::system(std::format("{} < {} > {}", std::filesystem::path{executable}.string().c_str(), "1.in", "1.out").c_str());

        // std::fstream hint{std::filesystem::path{u8"hint.out"}, std::ios::out | std::ios::binary};
        // auto &a = g.graph;
        // hint << a.size() << endl;
        // for (auto const &line: a) {
        //     hint << line.size();
        //     for (auto x: line) {
        //         hint << " " << x;
        //     }
        //     hint << endl;
        // }
        // hint.close();

        return;
        std::fstream fin{std::filesystem::path{u8"1.out"}, std::ios::in | std::ios::binary};
        auto &in = fin;

        {
            i32 n;
            in >> n;

            std::vector<std::pair<i32, i32>> ops(n);
            for (auto &[x, y]: ops) {
                in >> x >> y;
                if (x > y) std::swap(x, y);
                cp.merge(x, y);
                cp.graph[cp.parent[x]].erase(x);
            }

            assert(cp.graph == g.graph);
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    // i32 i = 0; true; i++) {
        // std::cerr << i << std::endl;
    Solution_6314180276493067::solve();
        // std::cerr << std::flush;
   
    return 0;
}
