/**
 * @link https://www.luogu.com.cn/problem/P3522
 */

#include "./lib"

#include "./libs/range.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1183852871979620 {

    struct Node {
        int least, most;
    };

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<Node> accepted(N);  // 可接受的范围，闭区间
        for (auto &[x, y]: accepted)  std::cin >> x >> y;

        std::deque<int> q;  // least 递减
        auto ans = -inf;
        for (auto i: range((int)accepted.size())) {
            auto [least, most] = accepted[i];
            while (not q.empty() and accepted[q.front()].least > most)  q.pop_front();
            if (not q.empty()) {
                // 一个可能的段是 q.front() -> current
                chkMax(ans, i - q.front() + 1);
            }

            auto tmp = i;
            while (not q.empty() and accepted[q.back()].least < least)  tmp = q.back(), q.pop_back();
            accepted.at(tmp).least = accepted[i].least, q.push_back(tmp);

            debug {
                for (auto i: q)  std::cout << std::format("[{}]({}, {}) ", i, accepted.at(i).least, accepted.at(i).most);
                std::cout << std::endl;
            }
        }

        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1183852871979620::solve();
    return 0;
}
