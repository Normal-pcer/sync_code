/**
 * 
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1001217313665246 {
    
    std::vector<std::vector<int>> graph;
    std::vector<int> val;
    std::vector<int> size;
    std::vector<int> order = {0};  // dfs Â∫è
    std::vector<int> pos;

    void dfs(int p, int prev) {
        pos.at(p) = order.size(), order.push_back(p);
        size.at(p) = 1;
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs(dest, p);
            size.at(p) += size.at(dest);
        }
    }

    std::vector<ll> c;
    inline int lowbit(int x) { return x & -x; }
    void update(int x, int val) {
        while (x < (int)c.size()) {
            c.at(x) += val;
            x += lowbit(x);
        }
    }

    auto query(int x) {
        auto res = 0LL;
        while (x) {
            res += c.at(x);
            x -= lowbit(x);
        }
        return res;
    }
    
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, R;  std::cin >> N >> M >> R;
        val.resize(N);
        for (auto &i: val)  std::cin >> i;

        graph.resize(N+1);
        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y), graph.at(y).push_back(x);
        }

        size.resize(N+1), pos.resize(N+1);
        dfs(R, 0);
        debug {
            for (auto item: order)  std::cout << item << ' ';
            std::cout << std::endl;
            for (auto sz: size)  std::cout << sz << ' ';
            std::cout << std::endl;
        }

        c.resize(N+1);
        for (auto i: range(1, N+1)) {
            update(i, val.at(i-1));
        }
        for (auto _: range(M)) {
            int op, p;  std::cin >> op >> p;
            if (op == 1) {
                int val;  std::cin >> val;
                update(pos.at(p), val);
            } else {
                int left = pos.at(p);
                int right = left + size.at(p) - 1;
                auto ans = query(right) - query(left - 1);
                std::cout << ans << std::endl;
            }
        }
    }
}

int main() {
    initDebug;
    Solution_1001217313665246::solve();
    return 0;
}
