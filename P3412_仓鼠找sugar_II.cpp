/**
 * @link https://www.luogu.com.cn/problem/P3412
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

/*
令 up[u] 表示：从 u 节点移动到它父亲的期望步数。
down[u] 表示：从 u 子树的一个点 v，到 u 点的期望步数为 f[v]。所有的 f[v] 之和。

于是有如下转移：
    up[u] = sum up[v] + deg[u]。
    此处的 up[root] 没有实际意义，只是出于完备给出。

    down[u] = sum(down[v] + size[v] * up[v])

答案为：
    ans = sum dis(i, j) / (n * n)

    其中对于一个特定的 i， dis(i, j) 求和即为以 i 为根的 down[i]。

考虑换根。
根节点从 fa 变成 u，令 w 为 u 的兄弟节点，v 为 u 的子节点。
观察这个过程中的变化量。
down[fa]: -down[v] - size[v]*up[v]
up[fa]: -up[u]
down[u]: +down'[fa] + size'[fa]*up'[fa]
up[u]: up'[fa]
*/
namespace Solution_4653086382387967 {
    i32 constexpr mod = 998244353;
    auto addMod(i32 x, i32 y) -> i32 {
        x += y;
        if (x >= mod) x -= mod;
        return x;
    }
    auto subMod(i32 x, i32 y) -> i32 {
        x -= y;
        if (x < 0) x += mod;
        return x;
    }
    auto mulMod(i32 x, i32 y) -> i32 {
        return static_cast<i32>(static_cast<i64>(x) * y % mod);
    }
    auto powMod(i32 x, i32 y) -> i32 {
        i32 res = 1;
        for (; y != 0; y >>= 1, x = mulMod(x, x)) {
            if (y & 1) res = mulMod(res, x);
        }
        return res;
    }
    auto divMod(i32 x, i32 y) -> i32 {
        return mulMod(x, powMod(y, mod - 2));
    }
    auto solve() -> void {
        i32 n;
        std::cin >> n;

        std::vector<std::vector<i32>> graph(n + 1);
        std::vector<i32> deg(n + 1, 0);
        for (auto _ = n - 1; _ --> 0; ) {
            i32 x, y;
            std::cin >> x >> y;
            graph[x].push_back(y);
            graph[y].push_back(x);
            deg[x]++, deg[y]++;
        }

        
        std::vector<i32> up(n + 1);
        std::vector<i32> down(n + 1);
        std::vector<i32> size(n + 1);
        auto getF = [&](auto &&getF, i32 u, i32 fa) -> void {
            up[u] = deg[u];
            down[u] = 0;
            size[u] = 1;

            for (auto v: graph[u]) {
                if (v == fa) continue;
                getF(getF, v, u);
                size[u] += size[v];
                up[u] = addMod(up[u], up[v]);
                down[u] = addMod(down[u], addMod(down[v], mulMod(size[v], up[v])));
            }
        };

        auto moveRoot = [&](auto &&moveRoot, 
            i32 fa,     // 旧的根节点
            i32 prev,   // 再上一个根节点
            i32 up_fa, i32 down_fa, i32 size_fa
        ) -> void {
            for (auto u: graph[fa]) {
                if (u == prev) continue;

                auto new_down_fa = subMod(down_fa, addMod(down[u], mulMod(size[u], up[u])));
                auto new_up_fa = subMod(up_fa, up[u]);
                auto new_size_fa = subMod(size_fa, size[u]);
                auto new_down_u = addMod(down[u], addMod(new_down_fa, mulMod(new_size_fa, new_up_fa)));
                auto new_up_u = addMod(up[u], new_up_fa);
                auto new_size_u = addMod(size[u], new_size_fa);

                up[u] = new_up_u;
                down[u] = new_down_u;
                moveRoot(moveRoot, u, fa, new_up_u, new_down_u, new_size_u);
            }
        };

        getF(getF, 1, 0);
        moveRoot(moveRoot, 1, 0, up[1], down[1], size[1]);

        i32 ans = 0;
        for (i32 i = 1; i <= n; i++) {
            ans = addMod(ans, down[i]);
        }
        ans = divMod(ans, mulMod(n, n));

        std::cout << ans << endl;
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution_4653086382387967::solve();
    return 0;
}
