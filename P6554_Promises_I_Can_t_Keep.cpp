/**
 * @link https://www.luogu.com.cn/problem/P6554
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/less_inf_number.hpp"
using namespace lib;

/*
考虑换根。
根从 A 换到 B，观察各个点的增量。
B 的子树下，所有点的期望都不会变。
A 的所有子树中，除了 B 所在的，其他的也都不会变。
A 的期望需要减去 B 的贡献。
然而 A 和 B 两个点的 size 都会改变。
观察暴力 dp 的式子：
F[i] <- val[i] + sigma(F[j]) / children

可以通过 F[i] 反推出 F[j] 之和，然后再加上或减去另一个点的 F 值即可。
*/
namespace Solution_9254495447996924 {
    using f64 = double;
    auto solveForce() -> void {
        i32 N; std::cin >> N;
        std::vector<std::vector<i32>> graph(N + 1);
        for (auto _ = N - 1; _ --> 0; ) {
            i32 x, y;
            std::cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x);
        }
        std::vector<i32> val(N + 1);
        for (i32 i = 1; i <= N; i++) std::cin >> val[i];

        auto getExpectRecursion = [&](i32 p, i32 prev, auto &&self) -> f64 {
            i32 childrenCount = static_cast<i32>(graph[p].size()) - (prev != 0);
            if (childrenCount == 0) return val[p];

            auto branchProb = static_cast<f64>(1) / childrenCount;
            f64 ans = 0;
            for (auto next: graph[p]) {
                if (next != prev) {
                    ans += self(next, p, self);
                }
            }
            ans *= branchProb;
            ans += val[p];
            return ans;
        };
        auto getExpect = lam(x, getExpectRecursion(x, 0, getExpectRecursion));

        f64 max = -inf;
        for (i32 i = 1; i <= N; i++) {
            chkMax(max, getExpect(i));
        }
        std::cout << std::fixed << std::setprecision(6) << max << endl;
    }
    auto solve() -> void {
        i32 N; std::cin >> N;
        std::vector<std::vector<i32>> graph(N + 1);
        for (auto _ = N - 1; _ --> 0; ) {
            i32 x, y;
            std::cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x);
        }
        std::vector<i32> val(N + 1);
        for (i32 i = 1; i <= N; i++) std::cin >> val[i];

        std::vector<f64> F(N + 1, 0);
        auto getExpectRecursion = [&](i32 p, i32 prev, auto &&self) -> f64 {
            i32 childrenCount = static_cast<i32>(graph[p].size()) - (prev != 0);
            if (childrenCount == 0) return F[p] = val[p];

            auto branchProb = static_cast<f64>(1) / childrenCount;
            f64 ans = 0;
            for (auto next: graph[p]) {
                if (next != prev) {
                    ans += self(next, p, self);
                }
            }
            ans *= branchProb;
            ans += val[p];
            return F[p] = ans;
        };
        auto getExpect = lam(x, getExpectRecursion(x, 0, getExpectRecursion));

        getExpect(1);  // 初始以 1 为根

        auto modifyRootRecursion = [&](i32 p, i32 prev, f64 F_fa, auto &&self) -> void {
            
        };
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_9254495447996924::solveForce();
    return 0;
}
