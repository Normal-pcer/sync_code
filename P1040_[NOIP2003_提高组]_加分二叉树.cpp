/**
 * @link https://www.luogu.com.cn/problem/P1040
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1761065362129408 {
    struct MaxItem {
        ll value = 0;
        int source = -1;

        std::weak_ordering operator<=> (const MaxItem &other) const {
            return value <=> other.value;
        }
        operator ll () const {
            return value;
        }
    };
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<int> val(N);
        for (auto &x: val)  std::cin >> x;

        // F[i][j]: [i, j) 段的最大分数
        std::vector F(N+1, std::vector(N+1, MaxItem{}));
        for (auto i: range(N+1))  F[i][i] = {1, -1};
        for (auto i: range(N))  F[i][i+1] = {val[i], i};
        for (auto len: range(2, N+1)) {
            for (auto i: range(N)) {
                auto j = i + len;
                if (j > N)  break;
                for (auto k: range(i, j)) {
                    chkMax(F[i][j], {F[i][k] * F[k+1][j] + val[k], k});
                }
            }
        }
        auto ans = F[0][N];
        std::vector<int> steps;
        std::function<void(int, int)> dfs = [&](int begin, int end) {
            if (F[begin][end].source == -1)  return;
            auto mid = F[begin][end].source;
            steps.push_back(mid);
            dfs(begin, mid), dfs(mid+1, end);
        };
        dfs(0, N);
        std::cout << ans.value << endl;
        for (auto x: steps)  std::cout << x + 1 << ' ';
        std::cout << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1761065362129408::solve();
    return 0;
}
