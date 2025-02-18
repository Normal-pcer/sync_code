/**
 * @link https://www.luogu.com.cn/problem/AT_abc276_b
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

namespace Solution_1687994568685032 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<std::set<i32>> graph(N + 1);
        for (i32 _ = 0; _ < M; _++) {
            i32 x, y;  std::cin >> x >> y;
            graph[x].insert(y);
            graph[y].insert(x);
        }
        for (i32 i = 1; i <= N; i++) {
            std::cout << graph[i].size() << " ";
            for (auto const &x: graph[i]) {
                std::cout << x << " ";
            }
            std::cout << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1687994568685032::solve();
    return 0;
}
