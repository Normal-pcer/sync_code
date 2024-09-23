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
inline void batchOutput2d(int **begin, int rows, int cols, const char *format){upto(i, rows){upto(j, cols)printf(format, begin[i][j]);printf("\n");}} inline void batchOutput2d(int**begin, int rows, int cols) {batchOutput2d(begin,rows,cols,"%3d ");}
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

const int _N = 1005, _V = 1005;
int N = read(), V = read();
int v[_N], w[_N];
int dp[_V], res[_V], prev[_V], cur[_V];

int main() {
    initDebug;
    std::memset(prev, 0x3f, sizeof(prev));
    std::memset(cur, 0x3f, sizeof(cur));
    upto(i, N)  scanf("%d%d", v+i, w+i);
    upto(i, N) {
        reverse(j, V, 1) {
            if (j>=v[i]) 
                chkMaxEx(dp[j], 
                    chkMin(prev[j],cur[j-v[i]]); cur[j]=i, 
                    if(dp[j]==dp[j-v[i]]+w[i])
                        chkMinEx(prev[j], cur[j]=i, {;}, cur[j-v[i]]), 
                    dp[j-v[i]]+w[i]
                );
        }
    }
    always batchOutput(prev, V);
    int ptr = V;
    while (ptr!=0) {
        printf("%d ", ptr);
        update(ptr, [](int index){return prev[index];});
    }
    putchar('\n');
    return 0;
}