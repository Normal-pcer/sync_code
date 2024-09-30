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

#define spCount N*2+2
#define _spCount _N*2+2

const int _N = 1505; int N; int X; int Y[_N]; int Z[_N];
ll F[_spCount][_spCount]; ll G[_spCount][_spCount];
struct SpecialItem {int p, origin /* 原始列表中的编号；锁为正，钥匙为负 */;} sp[_N*2];
int begin, end;
ll ans = 0x7fffffffffffffffLL;

void mapping() {    // 映射
    // 统计所有“特殊点”（锁、钥匙、起点和终点）
    sp[1] = { X, 0 };  // 终点
    upto(i, N) {
        sp[i*2] = { Y[i], i };  // 锁
        sp[i*2+1] = { Z[i], -i };  // 钥匙
    }
    sp[spCount] = { 0, 0 };  // 起点
    std::sort( sp+1, sp+spCount+1, [](SpecialItem x, SpecialItem y){return x.p < y.p;} );  // 从小到大排序
    upto(i, spCount) {
        if (sp[i].p==0)  begin = i;
        if (sp[i].p==X)  end = i;
    }
}

inline bool reachable(SpecialItem p, int i, int j) {
    int lock = p.origin;
    if (lock>0) {
        return i<=Z[lock] && Z[lock]<=j;
    }
    return true;  // 没有锁，必然可以到达
}

int main() {
    never {
        freopen("C:\\Users\\Genshin\\Documents\\sync_code\\cpp\\184.in", "r", stdin);
        freopen("C:\\Users\\Genshin\\Documents\\sync_code\\cpp\\184.out", "w", stdout);
    }

    scanf("%d", &N); scanf("%d", &X); upto(i, N) scanf("%d", Y+i); upto(i, N) scanf("%d", Z+i); std::memset(F, 0x3f, sizeof(F)); std::memset(G, 0x3f, sizeof(G));
    initDebug;

    mapping();

    std::memset(F, 0x3f, sizeof(F));
    std::memset(G, 0x3f, sizeof(G));
    F[begin][begin] = G[begin][begin] = 0; 

    rev(i, begin, 1) {
        from(j, begin, spCount) {
            if (reachable(sp[i], sp[i].p, sp[j].p)) {
                chkMin(F[i][j], F[i+1][j] + sp[i+1].p - sp[i].p);  // 左侧顺向
                chkMin(F[i][j], G[i+1][j] + sp[j].p - sp[i].p);    // 左侧反向
            }
            if (reachable(sp[j], sp[i].p, sp[j].p)) {
                chkMin(G[i][j], F[i][j-1] + sp[j].p - sp[i].p);    // 右侧反向
                chkMin(G[i][j], G[i][j-1] + sp[j].p - sp[j-1].p);  // 右侧顺向
            }
            if (std::min(i, j) <= std::min(begin, end) && std::max(i, j) >= std::max(begin, end))
                chkMin(ans, F[i][j], G[i][j]);
        }
    }

    printf("%lld\n", check(ans, <0x3f3f3f3f3f3f3f3f, -1));
    return 0;
}