/**
 * @link https://www.luogu.com.cn/problem/AT_abc261_g
 */
#include "libs/debug_macros.hpp"
#include "lib"
#include "libs/fixed_int.hpp"
using namespace lib;

/**
 * F[i][j][c] 表示从单个字符 c 变为 T[i...j] 的最小代价
 * 
 * 如果存在 len(A[i]) == 0，那么必然可以用一步的代价使得 F[i][j][c] + 1 -> F[i][j][A[i][0]]
 * 
 * G[i][j][x][y] 表示 A[x][1...y] 变为 T[i...j]
 * G[i][j][x][y] <- G[i][k][x][y-1] + F[k+1][j][A[i][y]]（ A[i][y] 单个字符变为 T[k+1...j] ）
 * 
 */
namespace Solution_3748218870117994 {
    void solve() {

    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3748218870117994::solve();
    return 0;
}
