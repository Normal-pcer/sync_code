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

        std::vector<i32> leafCount(N + 1, 0);
        auto countRecursion = [&](i32 p, i32 prev, auto &&self) -> void {
            bool hasChild = false;
            leafCount[p] = 0;
            for (auto next: graph[p]) {
                if (next != prev) {
                    hasChild = true;
                    self(next, p, self);
                    leafCount[p] += leafCount[next];
                }
            }
            if (not hasChild) leafCount[p] = 1;
        };

        std::vector<f64> F(N + 1, 0);
        auto getExpectRecursion = [&](i32 p, i32 prev, auto &&self) -> f64 {
            f64 ans = 0;
            f64 probPerLeaf = static_cast<f64>(1) / leafCount[p];
            bool hasChild = false;
            for (auto next: graph[p]) {
                if (next != prev) {
                    hasChild = true;
                    ans += self(next, p, self) * probPerLeaf * leafCount[next];
                }
            }
            if (not hasChild) return F[p] = val[p];
            ans += val[p];
            return F[p] = ans;
        };
        auto getExpect = lam(x, getExpectRecursion(x, 0, getExpectRecursion));

        f64 max = -inf;
        for (i32 i = 1; i <= N; i++) {
            countRecursion(i, 0, countRecursion);
            auto cur = getExpect(i);
            std::cout << "Root = " << i << ", cur = " << cur << endl;
            chkMax(max, cur);
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

        std::vector<i32> leafCount(N + 1, 0);
        auto countRecursion = [&](i32 p, i32 prev, auto &&self) -> void {
            bool hasChild = false;
            for (auto next: graph[p]) {
                if (next != prev) {
                    hasChild = true;
                    self(next, p, self);
                    leafCount[p] += leafCount[next];
                }
            }
            if (not hasChild) leafCount[p] = 1;
        };
        countRecursion(1, 0, countRecursion);

        std::vector<f64> F(N + 1, 0);
        auto getExpectRecursion = [&](i32 p, i32 prev, auto &&self) -> f64 {
            f64 ans = 0;
            f64 probPerLeaf = static_cast<f64>(1) / leafCount[p];
            bool hasChild = false;
            for (auto next: graph[p]) {
                if (next != prev) {
                    hasChild = true;
                    ans += self(next, p, self) * probPerLeaf * leafCount[next];
                }
            }
            if (not hasChild) return F[p] = val[p];
            ans += val[p];
            return F[p] = ans;
        };
        auto getExpect = lam(x, getExpectRecursion(x, 0, getExpectRecursion));

        getExpect(1);  // 初始以 1 为根

        f64 ans = -inf;
        auto modifyRootRecursion = [&](i32 p, i32 prev, f64 Fp, i32 countP, auto &&self) -> void {
            debug std::cout << "F[" << p << "] = " << Fp << endl;
            chkMax(ans, Fp);
            for (auto next: graph[p]) {
                if (next != prev) {
                    auto newCountNext = countP;
                    if (graph[p].size() == 1) newCountNext++;  // p 成为一个新的叶子节点
                    auto newCountP = countP - leafCount[next];
                    if (graph[p].size() == 1) newCountP++;
                    if (graph[next].size() == 1) newCountP++;
                    auto newFp = [&]() -> f64 {
                        // 所有子节点的 F 值之和
                        auto sumChildren = (Fp - val[p]) * countP;
                        auto sumOtherChildren = sumChildren - F[next] * leafCount[next];
                        auto expect = newCountP == 0
                            ? 0
                            : sumOtherChildren / newCountP;
                        return expect + val[p];
                    }();
                    auto newFnext = [&]() -> f64 {
                        auto sumChildren = (F[next] - val[next]) * leafCount[next];
                        auto sumNewChildren = sumChildren + newFp * newCountP;
                        return sumNewChildren / newCountNext + val[next];
                    }();

                    self(next, p, newFnext, newCountNext, self);
                }
            }
        };
        modifyRootRecursion(1, 0, F[1], leafCount[1], modifyRootRecursion);

        std::cout << std::fixed << std::setprecision(2) << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_9254495447996924::solve();
    debug Solution_9254495447996924::solveForce();
    return 0;
}
