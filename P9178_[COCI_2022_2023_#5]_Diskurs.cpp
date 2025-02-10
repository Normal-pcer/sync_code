/**
 * @link https://www.luogu.com.cn/problem/P9178
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * hamming(x, y) 即 popcount(x xor y)
 * 以下的 ~ 符号均表示 M 位数按位取反
 * 对于一个 M 位数 x，显然有 ham(x, y) + ham(~x, y) = M
 * H[x] 表示 x 与另一个数的最小距离
 * 
 * 初始值：
 * x 在数组中 - H[x] = 0
 * 否则 - H[x] = inf
 * 转移：
 * x 的任意一位反转，可以从反转后的数转移，距离加一
 * H[x] <= H[x xor (1<<i)] + 1 (chkMin)
 * 
 * 转移顺序：i 递增，其次 x 递增
 * 首先可以断定，x -> y -> x 这样的转移不可能是最优
 * 枚举到最高位的时候，y 试图从 x 转移，而 x 是通过修改更低位（或不修改）转移来的。
 * 这是合理的。如果 x 必须要修改最高位，那就会是从 y 转移，一定不是最优。
 * 更低的位同理。
 * 
 * 答案：
 * 对于原序列每一个数 a[i]
 * 答案即为 M - H[~a[i]]
 */
namespace Solution_1422241632498212 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        int N, M;  std::cin >> N >> M;

        auto inv = lam(x, x ^ ((1 << M) - 1));
        std::vector<int> a(N);
        for (auto &x: a)  std::cin >> x;

        std::vector<int> H(1 << M, inf);
        for (auto x: a)  H[x] = 0;
        for (auto i: range(M)) {
            for (auto x: range(1 << M)) {
                chkMin(H[x], H[x ^ (1<<i)] + 1);
            }
        }
        for (auto x: a)  std::cout << (M - H[inv(x)]) << " ";
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1422241632498212::solve();
    return 0;
}
