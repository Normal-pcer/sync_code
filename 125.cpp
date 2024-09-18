#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define reverse(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(signed argc, char const *argv[])
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

template <class T=int>
inline T read() { T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }
#define int long long
const int maxN=105, maxW=100005;
int N, V;
int v[maxN], w[maxN], sumW, ans;
long long dp[maxN][maxW];  // 前i个物品达到至少j的价值所需的最小空间

signed main() {
    initDebug;
    N=read(), V=read();
    upto(i, N) { v[i]=read(); w[i]=read(); }
    std::memset(dp, 0x3f, sizeof(dp));
    from(i, 0, w[1])  {
        dp[1][i] = v[1];
        if (dp[1][i]<=V)
            chkMax(ans, i);
    }
    dp[0][0] = 0;
    from(i, 2, N) {
        sumW += w[i];
        from(j, 1, sumW) {
            dp[i][j] = dp[i-1][j];
            if (j>=w[i]) { chkMin(dp[i][j], dp[i-1][j-w[i]]+v[i]); never log("%lld", dp[i-1][j-w[i]]+v[i])}
            if (dp[i][j]<=V)
                chkMax(ans, j+1);
            log("%3lld ", std::min(999LL, dp[i][j]))
        }
        log("\n")
    }
    printf("%lld\n", ans);
    return 0;
}