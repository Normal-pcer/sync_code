/**
 * 
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace Solution_2935048438418098 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, L, R;  std::cin >> N >> L >> R;
        std::vector<int> val(N+1);
        for (auto &i: val)  std::cin >> i;

        std::vector<int> F(N+1, -inf);
        std::deque<int> q;  // F[i] 单调递减

        F[0] = 0;
        for (auto i = 1; i < N + 1; i++) {
            // x < i - R 位置失效
            while (not q.empty() and q.front() < i - R)  q.pop_front();
            // x == i - L 位置新增
            if (i - L >= 0) {
                while (not q.empty() and F[q.back()] <= F[i-L])  q.pop_back();
                q.push_back(i-L);
            }
            if (q.empty())  continue;
            debug  std::cout << std::format("F[{}] <- F[{}]", i, q.front()) << std::endl;
            auto cur = F[q.front()] + val[i];
            F[i] = cur;
        }

        auto ans = -inf;
        for (auto i = N - R + 1; i < N + 1; i++) {
            chkMax(ans, F[i]);
        }
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_2935048438418098::solve();
    return 0;
}
