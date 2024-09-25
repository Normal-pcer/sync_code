#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define reverse(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
#define update(base,op,modify...) base=op((base),##modify)
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? (v): (otw))
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }


const int _N = 1003, _M = 10003, MOD = 1014;
int N = read(), M = read();
int V[_N];
int dp[_N][_M], revDp[_N][_M];

int main() {
    initDebug;
    upto(i, N) {
        V[i] = read();
    }
    dp[0][0] = revDp[N+1][0] = 1;
    upto(i, N)  from(j, 0, M) {
        dp[i][j] = dp[i-1][j];
        if (j>=V[i])  dp[i][j] += dp[i-1][j-V[i]];
        if (dp[i][j]>>28)  dp[i][j] %= MOD;
    }
    reverse(i, N, 1)  from(j, 0, M) {
        revDp[i][j] = revDp[i+1][j];
        if (j>=V[i])  revDp[i][j] += revDp[i+1][j-V[i]];
        if (revDp[i][j]>>28)  revDp[i][j] %= MOD;
    }
    debug batchOutput2d(dp, N, M, "%d ");
    debug batchOutput2d(revDp, N, M, "%d ");

    upto(i, N) {
        int result = 0;
        from(j, 0, M) { result = (result + dp[i-1][j] * revDp[i+1][M-j]) % 1014; }
        printf("%d ", result);
    }
    printf("\n");
    return 0;
}