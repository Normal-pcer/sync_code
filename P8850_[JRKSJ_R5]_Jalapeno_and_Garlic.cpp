/**
 * @link https://www.luogu.com.cn/problem/P8850
 */

#include "./lib_v4.hpp"


#include "./libs/fixed_int.hpp"
using i128 = __int128;
using namespace lib;

/**
 * 设 f[i] 表示：一个棋子距离目标为 i（逆时针距离），到达目标点的期望步数
 * f[0] = 0, f[n] = 0
 * f[i] = 1/2 * (f[i-1] + 1) + 1/2 * (f[i+1] + 1)
 * f[i+1] - 2f[i] + f[i-1] = -2
 * 设 f[i+1] + uf[i] = v(f[i] + uvf[i-1])
 * 解得 u = -1, v = 1
 * f[i+1] - f[i] = f[i] - f[i-1] - 2
 * 令 g[i] = f[i+1] - f[i]
 * g[i] = g[i-1] - 2
 *      = -2i + g0
 * f[i+1] = g[i] + g[i-1] + g[i-2] + ... + g[0] + f[0]
 *        = -i * (i + 1) - (i + 1) * g[0]
 * f[n] = 0 解得 g[0] = 1 - n
 * 故 f[i] = i * (n - i)
 * 
 * 接下来，需要找到一个点使得总期望最小。
 * 假设选择点 p 作为目标点。
 * f[i] 的图像是一个开口向下的二次函数，顶点在 x = n/2
 * 不妨使点 p 逆时针旋转，考虑此时的变化。
 * 小于 n/2 的点贡献会增大，大于 n/2 的点贡献会减小。
 * 
 * 增加的贡献好算吗？
 * (i + 1) * (n - i - 1) - i * (n - i) = -2i + n - 1
 * 那么其实不用考虑和 n/2 的大小关系了，这个式子的正负就能体现
 * 如何对于每个点快速算出这个增量？如果原先距离为 i 的位置上有 x 个点，这些点的贡献之和为 -2i * x + n * x - x。
 * 所有点的增量之和为 -sigma i*x[i] + (n - 1) * (sigma x[i]) 。
 * 需要特殊判断在 p 处的贡献。
 * 只需要记录 sigma i*x[i]。
 * 每次逆时针这个数的变化量都是可以算出来的，是 sigma x[i] - n * x[p]（p 点的距离变为 n-1，其他的距离均 -1）
 * 模拟 p 旋转的过程即可，处理每次旋转的复杂度是 O(1)，最终复杂度为 O(N)。
 * 
 * 模意义下不能判大小，这些数需要真正地算出来，令人忍俊不禁。
 */
namespace Solution_1510867167966491 {
    i32 constexpr mod = 1004535809;  // 1e9
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        i32 N;  std::cin >> N;  // 1e6
        std::vector<i32> a(N);  // 1e9
        for (auto &x: a)  std::cin >> x;

        // 需要记录的值：sigma i * a[i], sigma a[i]（后者只需要算一次，前者会持续变化）
        i64 sigma_ai = 0;  // sigma < 1e15
        i128 sigma_i_times_ai = 0;  // i * a[i] < 1e15, sigma < 1e21 感觉需要 int128
        for (i32 i = 0; i < N; i++) {
            sigma_ai += a[i];
            sigma_i_times_ai += static_cast<i64>(i) * a[i];
        }

        // 初始值（p = 0）
        // i * (n - i)
        i128 init = 0;  // 1e21
        for (i32 i = 0; i < N; i++) {
            init += static_cast<i128>(a[i]) * i * (N - i);
        }

        i128 cur = init;  // 现在的分数
        i128 ans = 0;  std::memset(&ans, 0x3f, sizeof(ans));  // 1e36 左右的极大值
        // -sigma i*a[i] + (n - 1) * (sigma a[i])

        for (i32 p = 0; p < N; p++) {  // p 转一圈
            chkMin(ans, cur);
            // p 从 p 变成 p+1
            // i 值减少，特别地，i = 0 的变成 i = N - 1
            // 一个点上的分数 f[i] -> f[i-1]
            // f[i] = n*i - i*i
            // f[i-1] = n*i - n - i*i + 2*i - 1
            // f[i-1] - f[i] = -n + 2*i - 1
            // f[n-1] - f[0] = n - 1，按照上面算是 -n - 1，所以需要额外加上 2*N * t[0]
            // t[i] 表示旧 i 值为 i 的个数
            // 2*i*t[i] - (n+1)*t[i] 求和
            // 维护 i*t[i]
            // 每次 i -> i-1 总共减少了 sigma t[i]
            // 特别地 0 -> N-1，算上刚才多减的，是 a[p] * N

            cur += 2 * sigma_i_times_ai;
            cur -= static_cast<i128>(N + 1) * sigma_ai;
            cur += static_cast<i64>(N * 2) * a[p];
            sigma_i_times_ai -= sigma_ai;
            sigma_i_times_ai += static_cast<i64>(a[p]) * N;
            debug {
                i128 tmp = 0;
                for (i32 i = 0; i < N; i++) {
                    auto j = (p - i + N) % N;
                    tmp += static_cast<i64>(j) * a[i];
                }
                assert(tmp == sigma_i_times_ai);
            }
        }

        std::cout << static_cast<i32>((ans % mod + mod) % mod) << endl;  // 输出答案才取模，令人忍俊不禁
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1510867167966491::solve();
    return 0;
}
