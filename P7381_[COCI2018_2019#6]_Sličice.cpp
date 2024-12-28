/**
 * @link https://www.luogu.com.cn/problem/P7381
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_8228166032927731 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M, K;  std::cin >> N >> M >> K;
        std::vector<int> P(N);
        for (auto &i: P)  std::cin >> i;
        std::vector<int> B(M+1);
        for (auto &i: B)  std::cin >> i;

        auto origin = [&]() {
            auto res = 0;
            for (auto i: P)  res += B[i];
            return res;
        }();

        struct Choice {
            int cost, value;
        };
        std::vector<std::vector<Choice>> groups(P.size());
        for (auto i: range(P.size())) {
            for (auto j: range(1, M - P[i] + 1)) {
                groups.at(i).push_back({.cost = j, .value = B[P[i]+j] - B[P[i]]});
            }
        }

        std::vector<int> F(K+1, -inf);
        F[0] = 0;
        for (auto i: range(groups.size())) {
            for (auto j: range(K+1) | views::reverse) {
                for (auto k: groups.at(i)) {
                    if (j + k.cost < K + 1) {
                        chkMax(F[j + k.cost], F[j] + k.value);
                    }
                }
            }
        }
        auto ans = ranges::max(F);
        ans += origin;
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_8228166032927731::solve();
    return 0;
}
