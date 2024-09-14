#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int N;
int dp[251] = {1, 1, 3};
const int MOD = 100007;

int main(int argc, char const *argv[]) {
    initDebug;
    std::cin >> N;
    upto(i, N)  dp[i]=(dp[i-1]+2*dp[i-2])%100007;
    std::cout << dp[N] << std::endl;
    return 0;
}