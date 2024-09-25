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
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int **begin, int rows, int cols, const char *format){upto(i, rows){upto(j, cols)printf(format, begin[i][j]);printf("\n");}} inline void batchOutput(int**begin, int rows, int cols) {batchOutput(begin,rows,cols,"%3d");}
template <class T=int>inline T read() { T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

const int maxN = 5005;
int M, N/*group*/;
int lvl1, lvl2;
int LMT;
int T[maxN], R[maxN], f[maxN];

int main() {
    initDebug;
    scanf("%d%d", &lvl1, &lvl2);
    scanf("%d%d", &M, &N);

    int scale = lvl2/lvl1;
    upto(i, N) {
        int temp = read();
        T[i] = temp / scale;
    }
    upto(i, M) {
        int group = read();
        int rewards = read();
        chkMax(R[group], rewards);
    }
    LMT = read();

    upto(i, N) {
        reverse(t, LMT, T[i])
            chkMax(f[t], f[t-T[i]]+R[i]);
    }

    printf("%d\n", f[LMT]);
    return 0;
}