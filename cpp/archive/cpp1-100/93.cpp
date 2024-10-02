#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

int M, N, T;
const int maxSize = 12;          // 数组开小了，绷
long long dp[maxSize][maxSize];  // 1, 2, ..., i 放进 1, 2, ..., j

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cin.tie(0);

    std::cin >> T;
    upto(t, T) {
        std::memset(dp, 0, sizeof(dp));
        std::cin >> M >> N;
        upto(j, N) { // 盘子
            for (int i=0; i<=M; i++) { // 苹果
                if (i<=1||j<=1) dp[i][j] = 1;
                else if (i<j)   dp[i][j] = dp[i][i];
                else            dp[i][j] = (dp[i][j-1] /*有空盘*/ + \
                                            dp[i-j][j] /*全放满，各自去掉一个*/);
                log("%4lld", dp[i][j]);
            }
            log("\n")
        }

        std::cout << dp[M][N] << std::endl;
    }
    return 0;
}
