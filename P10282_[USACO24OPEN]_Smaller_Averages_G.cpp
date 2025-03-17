/**
 * @link https://www.luogu.com.cn/problem/P10282
 */
#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"
using namespace lib;

/**
 * F[i][j][k][l] 表示：
 * 当前遍历到点 i，数组 A 的最后一段是 [j, i]，数组 B 的最后一段是 [k, i]。两者的段数相差 l。
 * 
 * 转移：
 * F[i][j][k][l] -> F[i + 1][i + 1][i + 1][l]
 * F[i][j][k][l] -> F[i + 1][i + 1][k][l + 1]
 * F[i][j][k][l] -> F[i + 1][j][k + 1][l - 1]
 * F[i][j][k][l] -> F[i + 1][j][k][l]
 * 
 * 复杂度 N**4。
 * 换一种状态。G[i][j] 表示：
 * A 数组和 B 数组恰好划分到 [1, i] 和 [1, j]，此时两个区域内的段数相同。
 * 在 G[i][j] 的基础上如何转移？假设 A 从 i 后面拿了 s 项作为一段，必然存在一个 t，B 从 j 后面拿 >= t 项都是合法的。
 * 枚举这个 s，随着 s 递增，t 从 0 增长到 N - j。
 * 对于向区间转移，使用差分即可。最后求一下前缀和，单次转移复杂度是 O(N)。
 * 对每一行 G[i] 求差分。在循环的过程中自然可以求出 G[i][j] 的实际值，同时可以快速地进行区间加法。
 * 
 * 想假了。随着 s 递增，平均数是可以减少的。
 * 可以把对于每个 i， s 递增能取到的所有平均数预处理出来，然后排序。接下来就能正常使用双指针了。
 * 对于每个 j，t 递增的结果同理。
 * 
 * 不妨换一个方向。从谁可以转移到 F[s][t]？
 * s 和 t 各自向前缩减一点。
 * reduce_s 和 reduce_t 表示所有可能砍掉的区间，按照区间内的平均值排序。
 * 可以从一个状态转移过来，要求 avg(reduce_s[i]) <= avg(reduce_t[j])
 * 使用双指针法。选定一个 i，必然存在一个 j，reduce_t[j] 及以后的元素都可以转移过来。
 * 设这个元素的长度为 x。F[s][t] <- sum F[i][x]。
 * 维护一个前缀和之类的东西即可。
 * 
 * 咋被这玩意硬控了这么多个小时啊。
 */
namespace Solution_7752660436032105 {
    struct Frac {
        i64 nume = 0, deno = 1;

        auto operator<=> (Frac const &other) const -> std::strong_ordering {
            return nume * other.deno <=> deno * other.nume;
        }
        auto operator== (Frac const &other) const -> bool {
            return *this <=> other == 0;
        }
    };
    i32 constexpr mod = 1e9 + 7;
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;
        std::vector<i32> a(N + 1), b(N + 1);
        for (auto &x: a | views::drop(1))  std::cin >> x;
        for (auto &x: b | views::drop(1))  std::cin >> x;
        
        // 从 a 或者 b 的对应点向前
        std::vector<std::vector<std::pair<Frac, i32>>> cuttable_at_a(N + 1), cuttable_at_b(N + 1);
        for (i32 i = 1; i <= N; i++) {
            i32 sum = 0;
            for (i32 back = i, front = i; front > 0; front--) {
                sum += a[front], cuttable_at_a[i].emplace_back(Frac{sum, back - front + 1}, front);
            }
            ranges::sort(cuttable_at_a[i]);
        }
        for (i32 j = 1; j <= N; j++) {
            i32 sum = 0;
            for (i32 back = j, front = j; front > 0; front--) {
                sum += b[front], cuttable_at_b[j].emplace_back(Frac{sum, back - front + 1}, front);
            }
            ranges::sort(cuttable_at_b[j]);
        }

        std::vector G(N + 1, std::vector<i32>(N + 1));
        G[0][0] = 1;
        for (i32 s = 1; s <= N; s++) {
            // 定义前缀和 G_ps[i][j] = sum G[k - 1][j], cuttable_at_a[s].index(k) <= cuttable_at_a[s].index(i)。
            // -1 是为了从新段第一个点变为旧段最后一个点。
            // t 左侧的 j 固定在一个位置。i 可以选择使得平均值更小的。
            // 假设 i0 使得这段的平均值恰好大于 j 到 t 段的平均值。
            // cuttable_at_a[s] 在 i0 以前的所有 i 均可以统计贡献。
            std::vector G_ps(N + 1, std::vector<i32>(N + 1));
            for (i32 j = 0; j <= N; j++) {
                i32 sum = 0;
                for (auto [_, k]: cuttable_at_a[s]) {
                    assert(k - 1 >= 0);
                    sum += G[k - 1][j], sum %= mod;
                    G_ps[k][j] = sum;
                }
            }
            for (i32 t = 1; t <= N; t++) {
                auto it_to_a = cuttable_at_a[s].begin();
                auto it_to_b = cuttable_at_b[t].begin();

                for (; it_to_b != cuttable_at_b[t].end(); it_to_b++) {
                    while (it_to_a != cuttable_at_a[s].end() and it_to_a->first <= it_to_b->first)  it_to_a++;
                    // it_to_a 恰好指向 i0
                    if (it_to_a != cuttable_at_a[s].begin()) {
                        auto back = *std::prev(it_to_a);
                        // 排序在 back 及以前的 x，都可以统计 G[x][j]
                        G[s][t] = (G[s][t] + G_ps[back.second][it_to_b->second - 1]) % mod;
                    }
                }
            }
        }

        debug  for (i32 i = 0; i < N + 1; i++) {
            for (i32 j = 0; j < N + 1; j++) {
                std::cout << std::format("G[{}][{}] = {}", i, j, G[i][j]) << std::endl;
            }
        }

        auto ans = G[N][N];
        std::cout << ans << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7752660436032105::solve();
    return 0;
}
