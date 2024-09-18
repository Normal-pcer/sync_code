#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<e;i++)
bool DEBUG_MODE=false;
 
int N, V;
const int maxN = 32, maxV=20005;
int v[maxN];
int w[maxN];
int dp[maxN][maxV];
 
 
int main(int argc, char const *argv[]) {
    initDebug;
    std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
    std::cin >> V >> N;
    upto(i, N) { std::cin >> v[i]; w[i]=v[i]; }
    upto(j, V) {
        upto(i, N) {
            dp[i][j] = std::max(dp[i-1][j], j>=v[i]?dp[i-1][j-v[i]]+w[i]:0);
            log("%3d(%2d, %2d)", dp[i][j], dp[i-1][j], dp[i-1][j-v[i]]+w[i])
        }
        log("\n")
    }
    std::cout << V-dp[N][V] << std::endl;
    return 0;
}