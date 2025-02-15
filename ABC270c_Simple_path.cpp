/**
 * @link https://www.luogu.com.cn/problem/AT_abc270_c
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

using namespace lib;

#include "./libs/fixed_int.hpp"

namespace Solution_1166737442538429 {
    void solve() {
        i32 N, X, Y;  std::cin >> N >> X >> Y;

        std::vector<std::vector<int>> graph(N+1);
        for (i32 i = 0; i < N - 1; i++) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        std::vector<i32> prev_of(N+1);
        struct Node { int x, prev = 0; };
        std::deque<Node> q{Node{X, 0}};
        std::vector<char> vis(N+1);

        while (not q.empty()) {
            auto [x, prev] = q.front();  q.pop_front();
            if (vis[x])  continue;
            vis[x] = true;
            prev_of[x] = prev;
            if (x == Y)  break;
            
            for (auto p: graph.at(x)) {
                q.push_back({p, x});
            }
        }

        std::vector<i32> path;
        auto cur = Y;
        while (cur != X) {
            path.push_back(cur);
            cur = prev_of[cur];
        }
        path.push_back(X);

        ranges::reverse(path);

        for (auto x: path)  std::cout << x << " ";
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1166737442538429::solve();
    return 0;
}
