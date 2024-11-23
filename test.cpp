/**
 * 
 */

#include "./lib"
#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1175088154992405 {

    const int _ST = 17;
    const int _N = 1e5+5;

    std::vector<std::vector<int>> graph;
    std::vector<int> depth;
    int F[_N][_ST];

    int LCA(int x, int y) {
        if (depth[x] < depth[y])  std::swap(x, y);

        auto distance = depth[x] - depth[y];
        for (auto i = 0; i < _ST; i++) {
            if (distance & (1<<i)) {
                distance ^= 1<<i;
                x = F[x][i];
            }
        }

        if (x == y)  return x;
        for (signed i = _ST - 1; i >= 0; i--) {
            if (F[x][i] != F[y][i]) {
                x = F[x][i], y = F[y][i];
            }
        }
        return F[x][0];
    }

    void dfs(int p, int prev) {
        depth[p] = depth[prev] + 1;
        F[p][0] = prev;
        for (auto i = 1; i < _ST; i++) {
            F[p][i] = F[F[p][i-1]][i-1];
        }
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs(dest, p);
        }
    }

    void solve() {

        int N, K;  io >> N >> K;
        graph.resize(N+1);

        std::vector<char> flag(N+1);
        for (auto i = 0; i < N-1; i++) {
            int x, y;  io >> x >> y;
            graph[x].push_back(y), graph[y].push_back(x);
            flag[y] = '\1';
        }

        auto find = std::find(flag.begin()+1, flag.end(), '\0');
        assert(find != flag.end());
        auto root = find - flag.begin();

        depth.resize(N+1);
        dfs(root, 0);

        for (auto i = 0; i < K; i++) {
            int x, y;  io >> x >> y;
            io << LCA(x, y) << endl;
        }
    }
}

int main() {
    initDebug;
    Solution_1175088154992405::solve();
    return 0;
}