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
typedef long long ll; typedef unsigned long long ull; typedef unsigned ui;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }


int const _N=11; int N; const int _K = 85; int K;
ull F[20][170][2048];

std::vector<ui> singleLine[20];
void dfs(int p /*最右侧选点*/, int count /*已有点数*/, ui left /*左侧编码*/) {  // 向右搜索
    singleLine[count].push_back(left);  // 不再选
    from(i, p+2, N-1) {  // 选择点 i
        dfs(i, count+1, left + (1<<i));
    }
}

void pre_process() {
    dfs(-2, 0, 0);  // 预处理单行的可能选择
}

int main() {
    initDebug;

    debug {
        freopen("186.in", "r", stdin);
        freopen("186.out", "w", stdout);
    }

    scanf("%d", &N); scanf("%d", &K);

    pre_process();

    F[0][0][0] = 1;
    upto(i, N) {  // 行号
        from(j, 0, K) {  // 已经放置过的棋子数
            from(t, 0, K-j) {  // 这次要放置的棋子数
                from(p, 0, 1<<N) {  // 上一次的放置方案
                    for (auto q: singleLine[t]) {  // 这次的放置方案
                        bool unavailable = false;
                        unavailable |= (bool)( q & p );
                        unavailable |= (bool)( q & (p<<1) );
                        unavailable |= (bool)( q & (p>>1) );
                        if (!unavailable)
                            F[i][j+t][q] += F[i-1][j][p];
                    }
                }
            }
        }
    }

    ull ans = 0;
    from(i, 0, 1<<N)  ans += F[N][K][i];

    printf("%lld\n", ans);
    return 0;
}