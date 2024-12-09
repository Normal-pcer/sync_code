/**
 * @link https://www.luogu.com.cn/problem/P2627
 */

#include "./lib_v2.hpp"

#include "./libs/io.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1645533826939380 {
    void solve() {
        int N, K;  io >> N >> K;
        std::vector<int> eff(N+2);

        for (auto &i: eff | views::drop(1) | views::take(N))  io >> i;
        
        std::vector<ll> F(N+2);  // 上一次选在 i
        std::deque<int> q;  // 单调递增
        q.push_back(0);
        for (auto i: range(1, N+2)) {
            while (not q.empty() and i - q.front() - 1 > K)  q.pop_front();
            F.at(i) = F.at(q.front()) + eff.at(i);
            while (not q.empty() and F.at(q.back()) >= F.at(i))  q.pop_back();
            q.push_back(i);
        }

        io << std::accumulate(eff.begin(), eff.end(), 0LL) - F.at(N+1) << endl;
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    Solution_1645533826939380::solve();
    return 0;
}