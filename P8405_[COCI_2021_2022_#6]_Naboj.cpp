/**
 * @link https://www.luogu.com.cn/problem/P8405
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1499991513624592 {
    struct Graph {
        int N;
        std::vector<std::pair<int, int>> edges;
        std::vector<std::vector<int>> graph;

    public:
        Graph(int N): N(N), graph(N+1) {}
        auto add(int x, int y) -> void {
            graph.at(x).push_back(y);
            edges.push_back({x, y});
        }
        auto topo() const -> std::optional<std::vector<int>> {
            std::vector<int> in(N+1);  // 入度
            for (auto [_, y]: edges)  in.at(y)++;

            std::deque<int> q;
            for (auto i: range(1, N+1)) {
                if (in[i] == 0)  q.push_back(i);
            }

            std::vector<int> order;
            auto cnt = 0;
            while (not q.empty()) {
                auto x = q.front();  q.pop_front();
                cnt++;
                
                for (auto y: graph.at(x)) {
                    in[y]--;
                    if (in[y] == 0)  q.push_back(y), order.push_back(y);
                }
            }

            if (cnt == N)  return order;
            else  return std::nullopt;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        int M;  std::cin >> M;
        Graph graph{N};
        for (auto _: range(M)) {
            int x, y;  std::cin >> y >> x;
            graph.add(x, y);
        }

        auto order_or_null = graph.topo();
        if (order_or_null == std::nullopt) {
            std::cout << -1 << std::endl;
            return;
        }
        auto &order = *order_or_null;
        std::cout << order.size() << endl;
        for (auto x: order) {
            std::cout << x << " " << 1 << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1499991513624592::solve();
    return 0;
}
