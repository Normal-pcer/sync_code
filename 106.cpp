#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

const int maxN = 102;
int M, N;
long long dp[maxN][maxN];

int main(int argc, char const *argv[]) {
    initDebug;
    std::cin >> M >> N;
    dp[1][1] = 1;
    upto(r, M) {
        upto(c, N) {
            dp[r][c] += dp[r-1][c] + dp[r][c-1];
        }
    }
    std::cout << dp[M][N] << std::endl;
    return 0;
}