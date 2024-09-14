#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
bool DEBUG_MODE=false;

long long dp[22][50][21];
int N;

int main(int argc, char const *argv[]) {
    initDebug;
    std::cin >> N;
    dp[1][24][0] = 1;
    upto(i, N) {
        debug printf("=====%02d=====\n", i);
        upto(r, 20) {
            upto(c, 48) {
                dp[r][c][i] = dp[r-1][c][i-1] + dp[r][c-1][i-1] + dp[r][c+1][i-1];
                if (c==24)  dp[r][c][i] -= 2 * dp[r][c][i-2];
                else if (i>2) dp[r][c][i] -= dp[r][c][i-2];
                debug if (r<=10 && 15<=c && c<=34) printf("%4lld", dp[r][c][i]);
            }
            debug if (r<=10) printf("\n");
        }
    }
    long long ans = 0;
    upto(r, 20) {
        upto(c, 48) {
            ans += dp[r][c][N];
        }
    }
    std::cout << ans << std::endl;
    return 0;
}