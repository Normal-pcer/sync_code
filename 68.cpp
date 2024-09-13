#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)

bool DEBUG_MODE;
long long dp[22]={0, 0, 1, 3}, n;
int main(int argc, char const *argv[]) {
    initDebug;
    std::cin >> n;
    for (int i=3; i<=n; i++)
        dp[i] = (i-1) * (dp[i-1] + dp[i-2]);
    std::cout << dp[n] << std::endl;
    return 0;
}