/**
 * @link https://www.luogu.com.cn/problem/AT_abc369_f
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1778585983417334 {

    const int _N = 262'144;
    struct Node {
        int val;
        int owner;

        auto operator<=> (const Node&) const = default;
    } c[_N];

    inline int lowbit(int x) { return x & -x; }
    void update(int x, Node val) {
        while (x < _N) {
            chkMax(c[x], val);
            x += lowbit(x);
        }
    }
    auto query(int x) {
        auto res = (Node){0, 0};
        while (x) {
            chkMax(res, c[x]);
            x -= lowbit(x);
        }
        return res;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int H, W, N;
        std::cin >> H >> W >> N;
        std::vector<std::pair<int, int>> coins(N+2);

        *coins.begin() = std::make_pair(1, 1);
        for (auto &[x, y]: coins | rgs::drop(1) | rgs::take(N))  std::cin >> x >> y;
        *coins.rbegin() = std::make_pair(H, W);
        rgs::sort(coins);

        // for (auto [x, y]: coins)  std::cout << x << ' ' << y << std::endl;
        // std::cout << "------" << std::endl;

        std::vector<int> prev(N+2);
        for (auto i: range(1, (int)coins.size())) {
            auto y = coins[i].second;
            auto [max, owner] = query(y);
            // debug std::cout << std::format("{} <- ({}, {})", i, max, owner) << std::endl;
            prev[i] = owner;
            update(y, {max+1, i});
        }

        std::vector<int> steps;
        auto cur = coins.size() - 1;
        while (cur)  steps.push_back(cur), cur = prev[cur];
        steps.push_back(0);

        std::reverse(steps.begin(), steps.end());
        // for (auto i: steps)  std::cout << std::format("({}, {})", coins[i].first, coins[i].second) << ' ';
        // std::cout << std::endl;

        std::string res;
        res.reserve(H+W+1);
        for (auto i: range(1, (int)steps.size())) {
            auto [x0, y0] = coins[steps[i-1]];
            auto [x1, y1] = coins[steps[i]];

            auto down = x1 - x0;
            auto right = y1 - y0;
            for (auto _: range(down))  res += 'D';
            for (auto _: range(right))  res += 'R';
        }

        std::cout << query(W).val - 1 << std::endl;
        std::cout << res << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1778585983417334::solve();
    return 0;
}