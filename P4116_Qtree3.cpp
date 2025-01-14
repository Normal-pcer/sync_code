/**
 * @link https://www.luogu.com.cn/problem/P4116
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_1787195618240513 {
    constexpr const int _N = 1e5+5;
    std::array<std::vector<int>, _N> graph;
    std::array<int, _N> depth, size, fa, son, top, index;
    std::vector<int> order;

    auto depth_cmp = [](int x, int y) { return depth[x] < depth[y]; };
    std::array<std::set<int, decltype(depth_cmp)>, _N> sets;
    
    auto dfs1(int p, int prev) -> void {
        size[p] = 1, fa[p] = prev;
        depth[p] = depth[prev] + 1;
        for (auto x: graph[p])  if (x != prev) {
            dfs1(x, p);
            size[p] += size[x];
            if (size[son[p]] < size[x]) {
                son[p] = x;
            }
        }
    }
    auto dfs2(int p, int tp) -> void {
        top[p] = tp, index[p] = order.size(), order.push_back(p);
        if (son[p] != 0)  dfs2(son[p], tp);
        for (auto x: graph[p]) {
            if (x != fa[p] and x != son[p]) {
                dfs2(x, x);
            }
        }
    }
    auto updatePoint(int p) -> void {
        auto &set = sets[top[p]];
        if (set.contains(p)) {
            set.erase(p);
        } else {
            set.insert(p);
        }
    }
    auto queryAncestor(int p) -> int {
        auto ans = 0;
        while (top[p] != top[1]) {
            if (not sets[top[p]].empty()) {
                auto it = sets[top[p]].begin();
                if (depth[*it] <= depth[p]) {
                    ans = *it;
                }
            }
            p = fa[top[p]];
        }
        if (not sets[top[p]].empty()) {
            auto it = sets[top[p]].begin();
            if (depth[*it] <= depth[p]) {
                ans = *it;
            }
        }
        return ans;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, Q;  std::cin >> N >> Q;
        for (auto _ = 0; _ < N - 1; _++) {
            int x, y;  std::cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x); 
        }
        dfs1(1, 1), dfs2(1, 0);
        depth[0] = inf;
        for (auto _ = 0; _ < Q; _++) {
            int op, x;  std::cin >> op >> x;
            if (op == 1) {
                auto ans = queryAncestor(x);
                if (ans == 0)  std::cout << -1 << endl;
                else  std::cout << ans << endl;
            } else {
                updatePoint(x);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1787195618240513::solve();
    return 0;
}
