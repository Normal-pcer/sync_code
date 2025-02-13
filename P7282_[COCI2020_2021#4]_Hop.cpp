/**
 * @link https://www.luogu.com.cn/problem/P7282
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/range.hpp"

using namespace lib;

/**
 * 考虑：对于任意两个可能产生联系的点（即 j > i 且 j % i == 0）连边。
 * 给边染色（最多三种颜色），希望同色的最长路径不超过 3 个节点。
 * 在染色前，最长的一条路径一定不会长于 60（1, 2, 4, 8, ..., 2**59）。
 * 
 * 二进制位数（__lg）相同的两个数之间一定不存在倍数关系。
 * 比较两个数的二进制位数，进行分组
 * 0 1 2 3; 4 5 6 7; 8 9 10 11...
 * 分为以上若干个小组
 * 0...16; 16...32; 32...48; 48...64
 * 分为四个大组
 * 如果两个数在同一个小组中，用 1 连接
 * 否则如果在同一个大组中，用 2 连接，
 * 否则用 3 连接。
 * 这样沿着一条链向上行走，显然至多有三段同色。
 */
namespace Solution_1024261437645003 {
    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        int N;  std::cin >> N;
        std::vector<ll> val(N);
        for (auto &x: val)  std::cin >> x;

        auto log = [](int x) -> int {
            if (x == 0)  return 0;
            else  return std::__lg(x);
        };
        auto color_of = [&](int x, int y) -> int {
            auto d0 = log(x), d1 = log(y);
            if ((d0 >> 2) == (d1 >> 2))  return 1;
            else if ((d0 >> 4) == (d1 >> 4))  return 2;
            else  return 3;
        };
        for (auto i = 1; i <= N - 1; i++) {
            for (auto j = 1; j <= i; j++) {
                std::cout 
                    << ([&]() -> int {
                            auto x = val[j - 1], y = val[i];
                            if (y % x == 0)  return color_of(x, y);
                            else  return 1;
                        }()) 
                    << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main(int dyy_forever, char const *swy_forever[]) {
    DEBUG_MODE = (dyy_forever - 1) and not strcmp("-d", swy_forever[1]);
    Solution_1024261437645003::solve();
    return 0;
}
