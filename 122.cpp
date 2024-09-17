#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define reverse(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp) base=std::max((base),(cmp))
#define chkMin(base,cmp) base=std::min((base),(cmp))
#define update(base,op,modify) base=op((base),(modify))
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? v: otw)
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
bool DEBUG_MODE=false;

typedef long long ll;


const int mod = 998244353;

int N, M;
int dp[5002][5002];
int main() {
    initDebug;
    scanf("%d%d", &N, &M);
    dp[1][1] = 1;
    upto(i, N)  upto(j, M)
    dp[i][j] += ((long long)dp[i][j-1]/*空一格*/+(long long)dp[i-j][j]/*填充一行*/)%mod;
    int ans=dp[N][M];
    // upto(j, M)  ans=(ans+dp[N][j])%mod;
    printf("%d\n", ans);
    return 0;
}