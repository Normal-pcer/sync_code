/**
 * @link https://www.luogu.com.cn/problem/P10193
 */
#include "./lib_v5.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * 首先发现 K 特别小（K <= 10），所以在图的所有节点上记录最近的 K 个充电站是可行的。
 * 一个比较暴力的方法是从每个充电站开始向外走，更新每个节点到自己的距离。
 * 一个优化是，如果不会对一个点造成任何贡献，那么就不需要向外走了。
 * 这是直接跑 C 次最短路。
 * 
 * 考虑优化。
 * 
 */
namespace Solution_4040868418093689 {
    i32 constexpr maxK = 10;
    i64 constexpr inf64 = infLL;
    struct MinItems {
        std::array<i64, maxK> data;

        MinItems() {
            data.fill(inf64);
        }
        auto update(i64 x) -> bool {
            auto it = ranges::upper_bound(data, x);
            if (it != data.end()) {
                ranges::rotate(it, data.end() - 1, data.end());
                *it = x;
                return true;
            } else {
                return false;
            }
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M, C, R, K;
        std::cin >> N >> M >> C >> R >> K;

        struct Node {
            i32 to = 0, val = 0;
        };
        std::vector<std::vector<Node>> graph(N + 1);
        for (i32 m = M; m --> 0; ) {
            i32 x, y, val;  std::cin >> x >> y >> val;
            graph[x].push_back({y, val});
            graph[y].push_back({x, val});
        }

        std::vector<MinItems> dis(N + 1);
        auto update_dis = [&](i32 s, std::vector<MinItems> &dis) -> void {
            std::vector<char> vis(N + 1);
            using QueueNode = std::pair<i32, i64>;
            std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<>> q;
            q.push({s, 0});
            dis[s].update(0);
            while (not q.empty()) {
                auto [x, val] = q.top();  q.pop();
                if (vis[x])  continue;
                vis[x] = true;

                for (auto next: graph[x]) {
                    if (vis[next.to])  continue;
                    if (dis[next.to].update(val + next.val)) {
                        q.push({next.to, val + next.val});
                    }
                }
            }
        };

        for (i32 i = 1; i <= C; i++) {
            update_dis(i, dis);
        }

        std::vector<i32> ans_collection;
        for (i32 i = C + 1; i <= N; i++) {
            if (dis[i].data[K - 1] <= R) {
                ans_collection.push_back(i);
            }
        }
        std::cout << ans_collection.size() << endl;
        for (auto x: ans_collection)  std::cout << x << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4040868418093689::solve();
    return 0;
}
