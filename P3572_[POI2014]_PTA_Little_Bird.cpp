/**
 * @link https://www.luogu.com.cn/problem/P3572
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_8144686069145044 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> height(N+2);
        std::vector<int> F(N+2);  // 上一次经过点 i 的最大值

        for (auto i = 1; i < N+1; i++)  std::cin >> height.at(i);
        height.at(N+1) = 0;

        int Q;  std::cin >> Q;
        for (auto _: range(Q)) {
            int K;  std::cin >> K;
            std::fill(F.begin(), F.end(), 0);

            std::deque<int> q;  // F[i] 单调递增，次优先保证 H[i] 递减
            q.push_back(1);
            for (auto i: range(2, N+1)) {
                while (not q.empty() and q.front() + K < i)  q.pop_front();
                F.at(i) = F.at(q.front());
                if (height.at(q.front()) <= height.at(i))  F[i]++;
                while (
                    not q.empty() and 
                    (
                        F[q.back()] > F[i] or
                        (F[q.back()] == F[i] and height[q.back()] < height[i])
                    )
                )  q.pop_back();
                q.push_back(i);

                debug {
                    for (auto i: q)  std::cout << std::format("[{}]({},{}) ", i, F[i], height.at(i));
                    std::cout << std::endl;
                }
            }
            std::cout << F[N] << std::endl;

        }
    }
}

int main() {
    initDebug;
    Solution_8144686069145044::solve();
    return 0;
}
