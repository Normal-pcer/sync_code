#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

const int maxN = 53;
int dp[maxN][maxN][maxN];  // 路径1走到(i, j)，路径2走到(k, l); 0.56MB
int map[maxN][maxN];
int M, N;
int x, y, v;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> M >> N;
    upto(r, M)  upto(c, N)  std::cin >> map[r][c];
    upto(i, M) {
        upto(j, N) {
            upto(k, M) {
                int l = i + j - k;  // 通过两个路径已走过的长度相等直接计算 l
                if (l<=0)  continue;
                dp[i][j][k] = std::max({
                    dp[i-1][j][k-1],
                    dp[i][j-1][k],
                    dp[i][j-1][k-1],
                    dp[i-1][j][k]
                });
                if ((i!=k && j!=l) || (i==1 && j==1) || (i==N && j==N)) {  // 不在同一格，或是开头或结尾
                    dp[i][j][k] += map[k][l] + map[i][j];
                    log("%d %d %d %d\n", i, j, k, l);
                }
            }
        }
    }

    std::cout << dp[M][N][M] << std::endl;
    return 0;
}