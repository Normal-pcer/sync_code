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
#define val(x) ((x).v*(x).p)
struct obj {int v, p;};
const int _N = 32005; int N = 32005; const int _M = 65; int M = 65; int f[_N], c[_N][_M] /*必包含j的最好取法*/;
std::vector<obj> s[_M];
int main() {scanf("%d", &N); scanf("%d", &M); 
    upto(i, M) {
        int v, p, q;
        scanf("%d%d%d", &v, &p, &q);
        q? s[q].push_back({v, p}): s[i].push_back({v, p});
    }
    initDebug;
    memset(c, -0x3f, sizeof(c));
    upto(i, M) {
        if (s[i].empty())  continue;
        rev(j, N, 1) {
            if (j>=s[i][0].v)
                c[j][i] = f[j-s[i][0].v] + val(s[i][0]);
        }
        never log("i=%d\n", i);
        never batchOutput2d(c, N, M, "%4d")
        from(t, 1, (int)s[i].size()-1) {
            rev(j, N, 1) {
                if (j>=s[i][t].v)
                    chkMax(c[j][i], c[j-s[i][t].v][i] + val(s[i][t]));
            }
        }
        rev(j, N, 1)  chkMax(f[j], c[j][i]);
    }
    printf("%d\n", f[N]);
    return 0;
}

/*
10 5
8 2 0
4 5 1
3 5 1 
4 3 0
5 2 0
*/