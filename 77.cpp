#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

const int maxN = 5001;
int a[maxN];
long long dp[maxN];
int N;

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &N);
    upto(i, N)  scanf("%d", a+i);
    dp[1] = a[1];
    for (int i=2; i<=N; i++) {
        dp[i] = 2147483647;
        upto(j, i-1)  dp[i] = std::min(dp[j]+a[i-j], dp[i]);
        log("%4lld", dp[i]);
    }
    log("\n");
    printf("%lld\n", dp[N]);
    return 0;
}