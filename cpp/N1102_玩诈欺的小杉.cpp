#include "lib"
#include "libs/io.hpp"
using namespace lib;

/*
一次操作可以把如下的区域取反
00100
00100
01110
00100
00100
T组数据：N 行 M 列的棋盘，初始全 0，至多进行 N*M 次操作，求能够使得棋盘变为给定排列的解法数
N <= 20, M <= 20; T <= 5
*/

int T = io.read();

namespace Solution {
    const int _N = 20, _M = 20;
    int N, M;
    // 用 32 位整数表示一列的状态
    // 小位代表小行号
    typedef uint32_t status;
    int F[_M][1<<_N];  // 前 i 行中，第 i 列状态为 j 的方案数
    std::bitset<_N> target[_M];  // 目标状态；先行后列

    void init() {
        io >> N >> M;

        from(r, 0, N-1) {
            from(c, 0, M-1) {
                char ch = getchar();
                if (ch == '1') target[c][r] = 1;
            }
        }
    }
    void solve() {
    }
}


int main() {
    initDebug;
    while (T --> 0)
        Solution::solve();
    return 0;
}