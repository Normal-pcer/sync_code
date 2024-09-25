#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

const int maxN = 2005;
int dp[maxN][maxN];     // i步选择j组的最短时间
int t[maxN][maxN];
int N, M;

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &N, &M);
    upto(j, M) {
        upto(i, N) {
            scanf("%d", &t[i][j]);
        }
    }
    upto(i, N)
        upto(j, M) {
            dp[i][j] = std::min(dp[i-1][j], dp[i-1][j>1? j-1: M]) + t[i][j];
            if (dp[i-1][j] > dp[i-1][j-1])  log("[%d]%d->%d\n", i, j-1, j)
        }
    debug {
        upto(i, N) {
            upto(j, M) {
                log("%3d", dp[i][j]);
            }
            log("\n")
        }
    }
    int ans = 0x7fffffff;
    upto(j, M)  ans = std::min(ans, dp[N][j]);
    printf("%d\n", ans);
    return 0;
}

/*
4 3 
2 6 1 8
3 6 2 6
4 2 3 6
*/