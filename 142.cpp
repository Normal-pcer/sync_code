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
#define chkMaxEx(base,exchange,other,cmp...) auto __b__=base;if(__b__!=chkMax(base,##cmp))exchange else other;
#define chkMinEx(base,exchange,other,cmp...) auto __b__=base;if(__b__!=chkMin(base,##cmp))exchange else other;
#define update(base,op,modify) base=op((base),(modify))
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? v: otw)
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int **begin, int rows, int cols, const char *format){upto(i, rows){upto(j, cols)printf(format, begin[i][j]);printf("\n");}} inline void batchOutput(int**begin, int rows, int cols) {batchOutput(begin,rows,cols,"%4d");}
template <class T=int>inline T read() { T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

const int maxS=1e4+5, maxN=43;
int N=read(), S=read();
int A[maxN];
int f[maxS];  // 表示S的最小张数
// int from[maxS];
int g[maxS];

int main() {
    initDebug;
    // std::memset(from, -0x3f, sizeof(f));
    std::memset(f, 0x3f, sizeof(f));
    upto(i, N) {
        A[i] = read();
        g[A[i]] = 1;
        f[A[i]] = 1;
    }
    f[0]=0;
    upto(i, N) {
        from(j, A[i], S) {
            // chkMinEx(f[j], {g[j]=g[j-A[i]];} ,{
            //     if (f[j-A[i]]+1 == f[j])
            //         g[j]+=g[j-A[i]];
            // } ,f[j-A[i]]+1);
            g[j] += g[j-A[i]];
            log("[%d]%d(%d) ", j, g[j], g[j-A[i]])
        }log("\n")
    }
    printf("%d\n", g[S]);
    return 0;
}