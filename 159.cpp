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


const int _N = 1005; int N = 1000; const int _W = 1005; int W = 1000; const int _K = 1005; int K = 1000; int p[_N]; int w[_N];

int g[_N], gp=1;
std::vector<int> stuff[_N];

struct Point {
    std::vector<int> targets;
    bool vis = false;
} pts[_N];

int dp[_W], tmp[_W];

int cnt = 0;
void dfs(int p) {
    if (pts[p].vis)  return;
    pts[p].vis = true;
    stuff[cnt].push_back(p);
    for (auto e: pts[p].targets) {
        dfs(e);
    }
}

int main() {scanf("%d", &N); scanf("%d", &W); scanf("%d", &K); upto(i, N) scanf("%d%d", p+i,w+i);
    initDebug;
    upto(i, K) {
        int a = read(), b = read();
        pts[a].targets.push_back(b);
        pts[b].targets.push_back(a);
    }

    debug {
        upto(i, N) {
            for (auto j: (pts[i].targets)) {
                printf("%4d ", j);
            }
            putchar('\n');
        }
    }

    upto(i, N) {
        if (!pts[i].vis) {
            ++cnt;
            dfs(i);
        }
    }

    gp = cnt;

    upto(i, gp) {
        rev(j, W, 1) {
            int best = 0;
            for (auto t: stuff[i])
                if (j>=w[t]) {
                    log("%d %d %d %d\n", i, j, t,  dp[j-w[t]] + p[t])
                    chkMax(best, dp[j-w[t]] + p[t]);
                }
            chkMax(dp[j], best);
        }
    }

    debug {
        upto(i, gp) {
            for (auto j: (stuff[i])) {
                printf("%4d ", j);
            }
            putchar('\n');
        }
    }

    printf("%d\n", dp[W]);
    return 0;
}