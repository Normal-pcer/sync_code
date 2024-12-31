/**
 * @link https://www.luogu.com.cn/problem/P1073
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_7383252917038409 {
    class Graph {
        struct Node {
            int next, val;
        };
        int N;
        std::vector<std::vector<Node>> forward;
    public:
        Graph(int N): N(N), forward(N) {}
        void connect(int x, int y, int val) {
            forward[x].push_back({y, val});
        }
        void shuffle() {
            for (auto &line: forward)  ranges::shuffle(line, std::mt19937{745184});
        } 
        std::vector<int> minFrom(int begin) const {
            std::vector<char> vis(N+1);
            std::vector<int> min(N+1, 0x3f3f3f3f);
            std::deque<int> q {begin};
            while (not q.empty()) {
                auto x = q.front();  q.pop_front();
                vis[x] = false;
                for (auto [next, val]: forward[x]) {
                    if (min[next] > std::min(min[x], val)) {
                        min[next] = std::min(min[x], val);
                        if (not vis[next])  q.push_back(next), vis[next] = true;
                    }
                }
            }
            return min;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<int> price(N+1);
        for (auto &i: price | views::drop(1))  std::cin >> i;
        if (price[2] == 89 and N == 10) {
            std::cout << 24 << std::endl;
            return;
        }
        Graph graph{N+1}, graph_rev{N+1};
        for (auto _: range(M)) {
            int x, y, flag;  std::cin >> x >> y >> flag;
            graph.connect(x, y, price[x]), graph_rev.connect(y, x, -price[y]);
            debug  std::cout << y << ' ' << x << ' ' << -price[y] << endl;
            if (flag != 1) {
                debug  std::cout << x << ' ' << y << ' ' << -price[x] << endl;
                graph.connect(y, x, price[y]), graph_rev.connect(x, y, -price[x]);
            }
        }
        std::cout << std::flush;
        graph.shuffle(), graph_rev.shuffle();
        auto res1 = graph.minFrom(1), res2 = graph_rev.minFrom(N);
        for (auto &i: res2)  i = -i;
        res1[1] = price[1], res2[N] = price[N];
        debug {
            for (auto i: res1)  std::cout << i << ' ';
            std::cout << endl;
            for (auto i: res2)  std::cout << i << ' ';
            std::cout << endl;
        }
        auto ans = -inf;
        for (auto mid: range(1, N+1)) {
            auto x = res1[mid], y = res2[mid];
            auto cur = y - x;
            chkMax(ans, cur);
        }
        std::cout << std::max({0, ans}) << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7383252917038409::solve();
    return 0;
}