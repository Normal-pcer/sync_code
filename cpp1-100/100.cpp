#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;
 
int N, M;
const int maxN = 550;
const int maxM = (int)3e5+5;
short values[(int)6e5+10];  // 1.14MB
int columns[maxM];
long long dp[maxM];
long long ans;
 
// int sumRect(int T, int L, int B, int R) {
//     return sigma[B][R] - sigma[B][L-1] - sigma[T-1][R] + sigma[T-1][L-1];
// }
 
int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &N, &M);
    // 保证N<=M
    if (N>=M) {
        std::swap(N, M);
        upto(c, M)  upto(r, N)  scanf("%hd", &values[r*M+c]);
    } else {
        upto(r, N)  upto(c, M)  scanf("%hd", &values[r*M+c]);
    }
    debug {
        upto(r, N) {
            upto(c, M)  printf("%4d", values[r*M+c]);
            printf("\n");
        }
    }
    // 维护求和数组
    // upto(r, N) { 
    //     upto(c, M) {
    //         sigma[r][c] = sigma[r-1][c] + sigma[r][c-1] + values[r][c] - sigma[r-1][c-1];
    //         log("%2d ", sigma[r][c]);
    //     }
    //     log("\n")
    // }
    upto(top, N) {                                      // 上边线
        log("\n\n\nt=%d", top)
        std::fill(columns+1, columns+1+M, 0);
        for (int bottom=top; bottom<=N; bottom++) {     // 下边线
            log("\nb=%d\n", bottom)
            upto(i, M) {
                columns[i] += values[bottom*M+i];
                dp[i] = std::max(dp[i-1], 0LL) + columns[i];
                ans = std::max(ans, dp[i]);
                log("%5d(%d)", dp[i], columns[i]);
            }
        }
    }
    log("\n")
    printf("%lld\n", ans);
    return 0;
}