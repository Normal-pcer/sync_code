#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
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
#define putInt(n) printf("%d\n",(n))
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }


const int _SZ=1e7+5;
const int _N = 1005; int N = 1005; int w[_N]; int s[_N]; int v[_N]; ll dp[_SZ];
struct obj {int w, s, v;} tmp[_N];
int main() {scanf("%d", &N); upto(i, N) scanf("%d%d%d", w+i,s+i,v+i);
    initDebug;
    int sum = 0;
    ll ans = 0;
    upto(i, N)  tmp[i] = {w[i], s[i], v[i]};
    std::sort(tmp+1, tmp+N+1, [](obj a, obj b){return a.s*a.w<b.s*b.w;});
    upto(i, N) {
        w[i]=tmp[i].w, s[i]=tmp[i].s, v[i]=tmp[i].v;
    }
    upto(i, N) {
        chkMax(sum, s[i]);
        rev(j, sum<<3, 1) {
            chkMax(dp[j], dp[j-1]);
            if (j-w[i]<=s[i] and j-w[i]>=0)  chkMax(dp[j], dp[j-w[i]]+v[i]);
            if (j>=w[i])  chkMax(dp[j], (ll)v[i]);
            chkMax(ans, dp[j]);
        }
        // never batchOutput(dp, sum);
    }
    printf("%lld\n", ans);
    return 0;
}