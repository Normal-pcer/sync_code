#include "testlib.h"
#include "./libs/fixed_int.hpp"

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
            std::find(hasParent.begin() + 1, hasParent.end(), false) - hasParent.begin());

        dfs(root, 0);
    }
    auto merge(i32 x, i32 y) -> void {
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

auto main(int argc, char *argv[]) -> int {
    registerTestlibCmd(argc, argv);

    auto &in = inf;  // 源输入
    auto &user = ouf;  // 选手输出

    i32 n = in.readInt();
    std::vector<std::pair<i32, i32>> edges(n - 1);
    for (auto &[x, y]: edges) x = in.readInt(), y = in.readInt();

    Graph g{n, edges};  // 听取用户操作的图
    i32 ansCount = user.readInt();
    std::vector<std::pair<i32, i32>> ops(ansCount);
    for (auto &[x, y]: ops) {
        x = user.readInt(), y = user.readInt();
        if (x > y) std::swap(x, y);
        g.merge(x, y);
        g.graph[g.parent[x]].erase(x);
    }

    i32 newN = in.readInt();
    std::vector<std::pair<i32, i32>> newEdges(newN - 1);
    for (auto &[x, y]: newEdges) x = in.readInt(), y = in.readInt();

    Graph ansGraph{n, newEdges};

    if (ansGraph.graph == g.graph) {
        quitf(_ok, "Accepted.");
    } else {
        if (ansCount != newN) {
            quitf(_wa, "Wrong Answer with wrong length.");
        } else {
            quitf(_wa, "Wrong Answer.");
        }
    }
    // assert(cp.graph == g.graph);
}