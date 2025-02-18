/**
 * @link https://www.luogu.com.cn/problem/AT_abc261_g
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

#include "./libs/fixed_int.hpp"

using namespace lib;

/**
 * F[i][j][c] 表示从单个字符 c 变为 T[i...j] 的最小代价
 * 
 * 如果存在 len(A[i]) == 0，那么必然可以用一步的代价使得 F[i][j][c] + 1 -> F[i][j][A[i][0]]
 * 
 * G[i][j][x][y] 表示 A[x][1...y] 变为 T[i...j]
 * G[i][j][x][y] <- G[i][k][x][y-1] + F[k+1][j][A[i][y]]（ A[i][y] 单个字符变为 T[k+1...j] ）
 * G[i][j][x][1] <- F[i][j][A[x][1]]
 * F[i][j][x] <- F[i][j][y] + chardis[y][x]
 * E[i][j] <- E[i-1][k] + F[k+1][j][s[i]]
 * F[i][j][C[x]] <- G[i][j][x][len(A)]
 */
namespace Solution_3748218870117994 {
    
    void solve() {
        i32 N;
        std::string s, t;
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);
        
        std::cin >> s >> t >> N;
        s.insert(s.begin(), '_');
        t.insert(t.begin(), '_');  // 从 1 开始编号
        std::vector chardis(26, std::vector<i32>(26, inf));
        std::vector<std::pair<std::string, std::string>> ops(N);
        for (auto &[x, y]: ops) {
            std::cin >> x >> y;

            if (x.size() == 1 and y.size() == 1) {
                chardis[x[0] - 'a'][y[0] - 'a'] = 1;
            }
        }

        for (i32 k = 1; k <= N; k++) {
            for (i32 i = 1; i <= N; i++) {
                for (i32 j = 1; j <= N; j++) {
                    chkMin(chardis[i][j], chardis[i][k] + chardis[k][j]);
                }
            } 
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_3748218870117994::solve();
    return 0;
}
