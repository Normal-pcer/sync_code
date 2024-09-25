#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;

int primes[10][5] = {{0, 0}, {0, 11, 13, 17, 19}, {0, 23, 29}, {0, 31, 37}, {0, 41, 43, 47}, {0, 53, 59}, {0, 61, 67}, {0, 71, 73, 79}, {0, 83, 89}, {0, 97}};
int a[] = {0, 5, 0, 6, 0, 0, 0, 5, 0, 5};
int b[] = {0, 4, 2, 2, 3, 2, 2, 3, 2, 1};
int K;
const int maxK = 1002;
int dp[maxK][10];  // j 是末位

int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> K;
    upto(i, 9)  dp[2][i] = a[i];
    from(i, 2, K+1) {
        upto(j, 9) { // 上一个的末位
            upto(k, b[j]) { // 这一位的可选项
                log("dp[%d][%d] = %d + %d(dp[%d][%d])\n", i, primes[j][k] % 10, dp[i][primes[j][k] % 10], dp[i-1][j], i-1, j)
                dp[i][primes[j][k] % 10] += dp[i-1][j];
            }
            dp[i][j] %= 10007; 
        }
    }
    debug {upto(i, K) {
        upto(j, 9) {
            log("%4d", dp[i][j]);
        }
        log("[%d]\n", i);
    }}
    int ans = 0;
    upto(i, 9)  ans += dp[K][i];
    std::cout << ans % 10007 << std::endl;
    return 0;
}