#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int MAX_SIZE = 1002;
int M, N;
int a[MAX_SIZE], b[MAX_SIZE];
int dp[MAX_SIZE][MAX_SIZE];
int ans = 0;

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d%d", &M, &N);
    upto(i, M)  scanf("%d", a+i);
    upto(i, N)  scanf("%d", b+i);
    upto(i, M) {
        upto(j, N) {
            if (a[i] == b[j])   dp[i][j] = dp[i-1][j-1] + 1;
            dp[i][j] = std::max( {dp[i][j], dp[i-1][j], dp[i][j-1]} );
            ans = std::max(ans, dp[i][j]);
        }
    }
    printf("%d\n", ans);
    return 0;
}