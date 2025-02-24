/**
 * @link https://www.luogu.com.cn/problem/P6145
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * 每条限制（a, b, x）：操作 b 在操作 a 之后至少 x 天
 * 看起来有点像拓扑排序
 * 没有任何限制的点可以排在第一天
 */
namespace Solution_2792320954737150 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M, C;  std::cin >> N >> M >> C;
        std::vector<i32> s(N+1);
        for (auto &x: s | views::drop(1))  std::cin >> x;

        struct Node {
            i32 next, val;
        };
        std::vector<std::vector<Node>> graph(N+1);
        std::vector<i32> in(N+1);
        for (i32 i = 0; i < C; i++) {
            i32 a, b, x;  std::cin >> a >> b >> x;
            graph[a].push_back({b, x});
            in[b]++;
        }

        std::vector<i32> limits(s);  // 最早的操作时间
        std::deque<i32> q;
        for (i32 i = 1; i <= N; i++) {
            if (in[i] == 0)  q.push_back(i);
        }

        while (not q.empty()) {
            auto x = q.front();  q.pop_front();

            for (auto [y, val]: graph[x]) {
                in[y]--;
                chkMax(limits[y], limits[x] + val);
                if (in[y] == 0) {
                    q.push_back(y);
                }
            }
        }

        for (auto x: limits | views::drop(1)) {
            std::cout << x << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2792320954737150::solve();
    return 0;
}
