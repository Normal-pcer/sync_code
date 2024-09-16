#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

int N, M;
const int maxN = 303;
int values[maxN][maxN];
long long sigma[maxN][maxN];      // (1,1)到(i,j)矩形范围求和
// int columns[maxN];
long long dp[maxN];
long long ans;

long long sumRect(int T, int L, int B, int R) {
    return sigma[B][R] - sigma[B][L-1] - sigma[T-1][R] + sigma[T-1][R-1];
}

int main(int argc, char const *argv[]) {
    initDebug;
    initDebug;
    scanf("%d%d", &N, &M);
    upto(r, N)  upto(c, M)  scanf("%d", &values[r][c]);
    upto(r, N)  upto(c, M)  if (values[r][c] == 0)  values[r][c] = -2147483647;
    // 维护求和数组
    upto(r, N) { 
        upto(c, M) {
            sigma[r][c] = sigma[r-1][c] + sigma[r][c-1] + values[r][c] - sigma[r-1][c-1];
            log("%2lld ", sigma[r][c]);
        }
        log("\n")
    }
    upto(top, N) {                                      // 上边线
        log("\n\n\nt=%d", top)
        for (int bottom=N; bottom>=top; bottom--) {     // 下边线
            log("\nb=%d\n", bottom)
            upto(i, M) {
                dp[i] = std::max(dp[i-1], 0LL) + sumRect(top, i, bottom, i);
                ans = std::max(ans, dp[i]);
                log("%12lld", dp[i]);
            }
        }
    }
    log("\n")
    printf("%lld\n", ans);
    return 0;
}

/*
3 4
1 2 3 4
5 0 6 3
10 3 4 0
*/