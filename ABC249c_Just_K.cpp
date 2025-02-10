/**
 * @link https://www.luogu.com.cn/problem/AT_abc249_c
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_1030025425235152 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, K;  std::cin >> N >> K;
        std::vector<std::string> s(N);
        for (auto &x: s)  std::cin >> x;

        auto ans = 0;
        for (auto i = 0U; i < (1U << N); i++) {
            std::vector<int> counter(26);
            for (auto j = 0; j < N; j++) {
                if (i & (1U << j)) {
                    for (auto ch: s[j])  counter[static_cast<int>(ch - 'a')]++;
                }
            }

            auto k_count = 0;
            for (auto x: counter)  k_count += (x == K);
            chkMax(ans, k_count);
        }

        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1030025425235152::solve();
    return 0;
}
