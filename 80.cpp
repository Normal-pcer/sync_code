#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int maxSize = 1502;
bool map[maxSize][maxSize];
int N, M;
int dp[maxSize][maxSize];
int ans[maxSize];

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &N);
    M=N;
    upto(r, N)  upto(c, M)  scanf("%1d", &map[r][c]);
////upto(r, N)  upto(c, M)  if ((r+c)%2)  map[r][c] = !map[r][c];
    upto(r, N)  upto(c, M)  if (map[r][c]) {
        dp[r][c] = std::min({dp[r-1][c], dp[r][c-1], dp[r-1][c-1]}) + 1;
    ////ans = std::max(ans, dp[r][c]);
        // int t = dp[r][c];
        for (int i=dp[r][c]; i>=2; i--)     ans[i]++;
    
    }
    for (int i=2; i<=N; i++) {
        if (ans[i]>0)   printf("%d %d\n", i, ans[i]);
        log("%d %d\n", i, ans[i]);
    }
////printf("%d\n", ans);
    return 0;
}