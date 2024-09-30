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

const int _N = 55; int N; const int _C = 55; int C;

struct Light {
    int pos, pow;
}p[_N];

int sigma[_N];  // 功率前缀和
int sumExcept(int l, int r) {
    assert(l<=r);
    return sigma[l-1] + sigma[N] - sigma[r];
}
int distance(int l, int r) {
    return p[r].pos - p[l].pos;
}

int F[_N][_N];
int G[_N][_N];

int main() {
    scanf("%d", &N); scanf("%d", &C);
    upto(i, N) {
        scanf("%d%d", &p[i].pos, &p[i].pow);
    }
    initDebug;

    memset(F, 0x3f, sizeof(F));
    memset(G, 0x3f, sizeof(G));

    // 维护前缀和
    upto(i, N) {
        sigma[i] = sigma[i-1] + p[i].pow;
    }

    F[C][C] = G[C][C] = 0;

    rev(i, C, 0) {
        from(j, i+1, N) {
            chkMin(F[i][j], F[i+1][j] + sumExcept(i+1, j) * distance(i, i+1));
            chkMin(F[i][j], G[i+1][j] + sumExcept(i+1, j) * distance(i, j));
            chkMin(G[i][j], F[i][j-1] + sumExcept(i, j-1) * distance(i, j));
            chkMin(G[i][j], G[i][j-1] + sumExcept(i, j-1) * distance(j-1, j));
        }
    }

    printf("%d\n", std::min(F[1][N], G[1][N]));
    return 0;
}