#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

const int maxN = 11;
long long dp[maxN][maxN][maxN][maxN];  // 路径1走到(i, j)，路径2走到(k, l)
int map[maxN][maxN];
int N;
int x, y, v;

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> N;
    for (;;) {
        std::cin >> x >> y >> v;
        map[x][y] = v;
        if (x==0 && y==0)
            break;
    }
    upto(i, N) {
        upto(j, N) {
            upto(k, N) {
                upto(l, N) {
                    dp[i][j][k][l] = std::max({
                        dp[i-1][j][k-1][l],
                        dp[i][j-1][k][l-1],
                        dp[i][j-1][k-1][l],
                        dp[i-1][j][k][l-1]
                    });
                    if (i!=k && j!=l) {  // 不在同一格
                        dp[i][j][k][l] += map[i][j];
                    }
                    dp[i][j][k][l] += map[k][l];
                }
            }
        }
    }
    std::cout << dp[N][N][N][N] << std::endl;
    return 0;
}