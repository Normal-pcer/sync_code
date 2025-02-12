/**
 * @link https://www.luogu.com.cn/problem/AT_abc269_g
 */
#pragma GCC optimize(3)
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

using namespace lib;
#include "./libs/rolling_container.hpp"

/**
 * 一张卡片翻开从 a[i] 到 b[i]，增量为 c[i] = b[i] - a[i]
 * a[i] + b[i] 之和小于等于 2e5
 * c[i] 的种类不会很多（根号级别）
 * 选择若干个 c[i] 凑出 K，由于 c[i] 的种类较少，可以看成分组背包。
 * 可以使用二进制优化。
 */
namespace Solution_3049054151772910 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N, M;  std::cin >> N >> M;
        std::vector<std::pair<int, int>> cards(N);
        auto sigma_a = 0;
        for (auto &[x, y]: cards)  std::cin >> x >> y, sigma_a += x;

        std::vector<int> c(N);
        std::map<int, int> groups;  // c[i] 的种类和个数
        for (auto i = 0; i < N; i++) {
            auto [x, y] = cards[i];
            c[i] = y - x;
            groups[c[i]]++;
        }

        struct Object {
            ll value = 0, weight = 0;
        };
        std::vector<Object> obj;
        for (auto [x, cnt]: groups) {
            auto lg = std::__lg(cnt);
#if false  // 在写什么？
            for (auto i = 0; i < lg; i++) {
                if (cnt & (1U << i)) {
                    obj.push_back({static_cast<ll>(x) << i, 1LL << i});
                }
            }
#endif
            for (auto i = 0; i < lg; i++) {
                obj.push_back({static_cast<ll>(x) << i, 1LL << i});
            }
            auto placed = (1 << lg) - 1;
            auto remains = cnt - placed;
            if (remains != 0) {
                obj.push_back({static_cast<ll>(x) * remains, remains});
            }
        }

        // 只有 -M + sigma_a 到 2M + sigma_a 的范围内有效
        // F[i][j + offset] 表示前 i 个物品，价值为 j 的最小重量
        auto offset = M / 2 + sigma_a;
        auto min_j = -(M / 2) - sigma_a, max_j = M * 3 / 2 - sigma_a;

        RollingContainer<std::vector<std::vector<ll>>, 2> F(std::vector(M * 5 / 2 + 1, infLL));  // 滚动数组优化空间
        F[0][0 + offset] = 0;
        for (auto i = 1; i <= static_cast<int>(obj.size()); i++) {
            for (auto j = min_j; j <= max_j; j++) {
                F[i][j + offset] = F[i-1][j + offset];
                auto from = j - obj[i-1].value;
                if (from < min_j or from > max_j)  continue;
                chkMin(F[i][j + offset], F[i-1][from + offset] + obj[i-1].weight);
            }
        }

        auto const &ans = F[obj.size()];
        for (auto k = 0; k <= M; k++) {
            auto cur_ans = ans[k - sigma_a + offset];
            if (cur_ans >= infLL)  cur_ans = -1;
            std::cout << cur_ans << std::endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3049054151772910::solve();
    return 0;
}
