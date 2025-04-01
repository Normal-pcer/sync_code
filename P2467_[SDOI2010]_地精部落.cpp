/**
 * @link https://www.luogu.com.cn/problem/P2467
 */
#include "./lib_v6.hpp"
#include "./libs/fixed_int.hpp"
#include "./libs/null_number.hpp"
using namespace lib;

/*
即每一个点，要么是「峰」，要么是「谷」。
我会 log 查询区间「峰」和「谷」的数量。

考虑到高度较小（和 N 同级），可能会把高度信息设在状态中。
但是想要保证高度恰好为一个排列，似乎并不好弄。
我会状压，能获得 40 分。
一点都不优。

题解给出了以下的几个性质：
- 如果值为 i 和 i + 1 的数不相邻，交换它们一定仍然合法。
- 一个序列通过对称一定可以获得另一个序列，其中一个的「峰」对应另一个的「谷」。

这两个性质似乎都是显然的。

于是可以设以下的状态：
F[i][j] 表示：使用前 i 个数，第一个数为 j 且作为「峰」的方案数。

首先，如果 j 已经出现在这个序列里，且 j - 1 为开头，可以通过交换使得 j 为开头。
由于规定第一个数为「峰」，所以打头的 j - 1 和 j 一定不可能相邻。
F[i][j] <- F[i][j - 1] (a)

现在希望放入一个新的数字 j。
加入现在已经把 j 放在开头作为第一个「峰」，需要在后面加上一个「谷」。
后面拼上的区间以 k 为第一个数，由于性质 2，k 作为「谷」的方案数可以通过某个数作为峰求出来。
k 作为谷的方案数应该和 k' 作为峰相等。
最终需要遍历所有可能的 k 求和。
错误的，需要去重。
我还是觉得是处于去重因素考量的。接下来，我们模拟 F[5][4] 的计算过程。
如果希望进行转移 (a)，实际意义是通过交换 4 和处于开头的 3，获得一个新序列。考虑以下序列：
3 2 5 1 4
这应该是一个合法的序列。这个情况被统计了。最终变为：
4 2 5 1 3
另外一种思考方法。在 4 的后面拼上一个序列：
2 5 1 3
这个序列满足 2 为谷。所以出现了重复。
能够正常进行转移 (b) 只有一种情况，例如如下合法序列：
4 3 5 1 2
它不可能由交换 3 和 4 得到（交换前的不是合法序列）。
但是 3 5 1 2 是一个正常的开头为谷的序列。
可以得到结论：k = j - 1。
那么如何求它的对称序列呢？发现值域中少了一个 4，可以大于 4 的数字统一向前挪动。
3 4 1 2
对应地：
2 1 5 3
此时 4 其实是 j，所以 k < j，有 k' = i - k = i - j + 1。
是否存在一种可能，这个 k' 需要被重映射回去？
例如 i = 5，j = 2，k' = 4，好像确实有可能。
2 后面想要拼上 1 5 3 4 这样的序列。
哦，我是错误的。类似 1 5 3 4 的数量等于类似 1 4 2 3 的数量，进一步地，等于类似 4 1 3 2 的数量，即 F[4][4]。
所以不需要重映射。
综上所述，当且仅当 k = j - 1 时有效，所以实际不用求和，只需单次转移：
F[i][j] <- F[i - 1][i - j + 1] (b)

只需对两种转移求和。

最终求答案，只需要对 F[N][j] 求和乘以 2。
*/
namespace Solution_2200216507842435 {
    i32 P = 1;
    auto addMod(i32 a, i32 b) -> i32 {
        a += b;
        if (a >= P) a -= P;
        return a;
    }
    auto mulMod(i32 a, i32 b) -> i32 {
        return static_cast<i64>(a) * b % P;
    }
    auto nearMod(i32 a) -> i32 {
        if (a < 0) a += P;
        if (a >= P) a -= P;
        return a;
    }
    auto solveForce() -> void {
        i32 N;
        std::cin >> N >> P;

        i32 ans = 0;
        std::vector<i32> heights(N);
        std::iota(heights.begin(), heights.end(), 1);

        do {
            auto check = [&]() -> bool {
                for (i32 i = 0; i < N; i++) {
                    auto left = (i != 0? heights[i - 1]: null);
                    auto right = (i != N - 1? heights[i + 1]: null);
                    auto cur = heights[i];

                    auto flag = false;
                    if ((left == null or left > cur) and (right == null or cur < right)) flag = true;
                    if ((left == null or left < cur) and (right == null or cur > right)) flag = true;
                    if (not flag) return false;
                }
                return true;
            };
            if (check()) {
                ans++;
                debug {
                    for (auto x: heights) std::cout << x << " ";
                    std::cout << endl;
                }
            }
        } while (std::next_permutation(heights.begin(), heights.end()));

        std::cout << ans % P << endl;
    }
    auto solve() -> void {
        i32 N;
        std::cin >> N >> P;

        // F[i][j] 表示：使用前 i 个数，第一个数为 j 且作为「峰」的方案数。
        std::vector F(N + 1, std::vector<i32>(N + 1, 0));
        F[1][1] = 1;
        for (i32 i = 2; i <= N; i++) {
            for (i32 j = 1; j <= i; j++) {
                if (j != 1) F[i][j] = addMod(F[i][j], F[i][j - 1]);
                F[i][j] = addMod(F[i][j], F[i - 1][i - j + 1]);
            }
        }

        i32 ans = std::accumulate(F[N].begin(), F[N].end(), 0, addMod);
        ans = addMod(ans, ans);
        std::cout << ans << endl;
    }
}

auto main(int cyy, char const *gzh[]) -> int {
    DEBUG_MODE = (cyy != 1) and (std::strcmp("-d", gzh[1]) == 0);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    Solution_2200216507842435::solve();
    return 0;
}
