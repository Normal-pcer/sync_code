/**
 * @link https://www.luogu.com.cn/problem/AT_abc261_d
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * F[i][j] 表示 第 i 次抛硬币，计数器为 j 之后的最大收益
 */
namespace Solution_4630774456267733 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N, M;  std::cin >> N >> M;
        std::vector<i32> value(N + 1);
        for (auto &x: value | views::drop(1))  std::cin >> x;
        std::map<i32, i32> extra;
        for (i32 i = 0; i < M; i++) {
            i32 x, y;  std::cin >> x >> y;
            extra[x] = y;
        }

        std::vector F(N + 1, std::vector<i64>(N + 1, -infLL));
        F[0][0] = 0;

        for (i32 i = 0; i < N; i++) {
            for (i32 j = 0; j < N; j++) {
                auto get_ex = [&](int x) -> i32 {
                    auto it = extra.find(x);
                    if (it != extra.end())  return it->second;
                    else  return 0;
                };
                chkMax(F[i + 1][j + 1], F[i][j] + value[i + 1] + get_ex(j + 1));
                chkMax(F[i + 1][0], F[i][j]);
            }
        }

        i64 ans = -infLL;
        for (i32 i = 0; i <= N; i++) {
            for (i32 j = 0; j <= N; j++) {
                // debug  std::cout << std::format("{:3}", F[i][j]);
                chkMax(ans, F[i][j]);
            }
            // debug  std::cout << std::endl;
        }

        std::cout << ans << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_4630774456267733::solve();
    return 0;
}
