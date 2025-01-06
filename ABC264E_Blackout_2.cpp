/**
 * @link https://www.luogu.com.cn/problem/AT_abc264_e
 * @link https://atcoder.jp/contests/abc264/tasks/abc264_e
 */
#ifdef ONLINE_JUDGE
#pragma GCC optimize(2)
#endif
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_3357545288551450 {
    class DSU {
        std::vector<int> fa;
        std::vector<int> size;
        std::vector<char> flag;
    public:
        DSU(int N): fa(N+1), size(N+1, 1), flag(N+1) {
            std::iota(fa.begin(), fa.end(), 0);
        }
        void active(int x) {
            flag[x] = true;
        }
        int find(int x) {
            if (fa[x] == x)  return x;
            else  return (fa[x] = find(fa[x]));
        }
        int combine(int x, int y) {  // 返回有多少个点被新染色
            auto a = find(x), b = find(y);
            if (a == b)  return 0;
            if (size[a] > size[b])  std::swap(a, b), std::swap(x, y);
            fa[a] = b, size[b] += size[a];
            if (not flag[b] and flag[a])  return flag[b] = true, size[b] - size[a];
            if (not flag[a] and flag[b])  return flag[a] = true, size[a];
            return 0;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, E;  std::cin >> N >> M >> E;
        std::vector<std::pair<int, int>> edges(E);
        for (auto &[x, y]: edges)  std::cin >> x >> y;
        int Q;  std::cin >> Q;
        std::vector<int> queries(Q);
        for (auto &x: queries)  std::cin >> x, x--;
        std::set<std::pair<int, int>> deleted;
        for (auto x: queries)  deleted.insert(edges.at(x));
        std::vector<std::pair<int, int>> init(edges);
        // std::erase_if(init, lam(x, deleted.contains(x)? (deleted.erase(x), true): false));
        std::erase_if(init, lam(x, deleted.contains(x)));
        DSU dsu(N + M);
        for (auto i: range(N+1, N+M+1))  dsu.active(i);
        // 初始的插入并查集
        auto cur = 0;  // 没有城市有供电
        for (auto [x, y]: init) {
            cur += dsu.combine(x, y);
        }
        // 倒序遍历查询，加边
        std::vector<int> ans;
        for (auto q: queries | views::reverse) {
            auto [x, y] = edges.at(q);
            ans.push_back(cur);
            cur += dsu.combine(x, y);
        }
        for (auto x: ans | views::reverse)  std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3357545288551450::solve();
    return 0;
}
