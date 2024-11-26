/**
 * @link https://www.luogu.com.cn/problem/P3459
 */

#include "./lib"
#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1275913551310634 {

    std::vector<std::vector<int>> graph;

    std::vector<int> order, size;
    void dfs(int p, int prev) {
        order.push_back(p);
        size.at(p) = 1;
        for (auto dest: graph.at(p))  if (dest != prev) {
            dfs(dest, p);
            size.at(p) += size.at(dest);
        }
    }

    std::vector<int> c;
    inline int lowbit(int x) { return x & -x; }
    auto query(int x) -> int {
        x++;
        never std::cout << std::format("query({})", x) << std::endl;
        auto res = 0;
        while (x) {
            res += c.at(x);
            x -= lowbit(x);
        }
        return res;
    }

    auto update(int x, int val) -> void {
        x++;
        never std::cout << std::format("update({}, {})", x, val) << std::endl;
        while (x < (int)c.size()) {
            c.at(x) += val;
            x += lowbit(x);
        }
    }

    auto update(int begin, int end, int val) {
        update(begin, +val);
        update(end, -val);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N;  std::cin >> N;

        graph.resize(N+1), size.resize(N+1);
        for (auto _: range(N-1)) {
            int x, y;  std::cin >> x >> y;
            graph.at(x).push_back(y);
            graph.at(y).push_back(x);
        }

        dfs(1, 0);

        std::vector<int> firstIndex(N+1);  // 第一次出现在 order 的下标
        for (auto i: range(order.size())) {
            firstIndex.at(order.at(i)) = i;
        }

        std::vector<std::pair<int, int>> subTreeRanges(N+1);
        
        c.resize(N+5);
        for (auto i: range(1, N+1)) {
            auto begin = firstIndex.at(i);
            auto end = begin + size.at(i);

            never std::cout << std::format("Point {}: begin = {}, end = {}", i, begin, end) << std::endl;
            subTreeRanges.at(i) = {begin, end};
            update(begin, end, 1);
        }

        update(0, N, -1);

        for (auto i: range(1, N+1)) {
            never std::cout << std::format("Point {}: val = {}", i, query(firstIndex.at(i))) << std::endl;
        }

        int Q;  std::cin >> Q;
        Q = Q + N - 1;
        for (auto _: range(Q)) {
            char op;  int x;
            std::cin >> op >> x;
            if (op == 'A') {
                int y;  std::cin >> y;
                if (x > y)  std::swap(x, y);
                auto [begin, end] = subTreeRanges.at(y);
                update(begin, end, -1);
            } else {
                std::cout << query(firstIndex.at(x)) << '\n';
            }
        }
    }
}

int main() {
    initDebug;
    Solution_1275913551310634::solve();
    return 0;
}
