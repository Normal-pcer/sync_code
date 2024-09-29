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


const int _N = 725; int N;
struct Point {
    double x;
    double y;

    Point operator- (Point const other) const {
        return { this->x - other.x, this->y - other.y };
    }
}p[_N];
double abs(Point p) { return sqrt( p.x * p.x + p.y * p.y ); }

double F[_N*2][_N*2], G[_N*2][_N*2];

void display(double arr[_N*2][_N*2]) {
    upto(i, N*2) {
        upto(j, N*2) {
            printf(arr[i][j]<1000000?"%11.2lf":"        INF", arr[i][j]);
        }
        printf("\n");
    }
}

auto main() -> int {scanf("%d", &N);
    initDebug;
    upto(i, N) {
        scanf("%lf%lf", &p[i].x, &p[i].y);
        p[i+N] = p[i];
    }

    std::memset(F, 0x59, sizeof(F));
    std::memset(G, 0x59, sizeof(G));

    F[1][N] = 0;
    G[2][N] = abs(p[1]-p[N]);

    rev(l, N-1, 1) {
        from(i, 2, N*2) {
            auto j = l+i-1;
            if (j>N*2)  break;

            // chkMin( F[i][j], F[i-1][j] + abs(p[i]-p[i-1]), G[i-1][j] + abs(p[j]-p[i-1]) );
            // chkMin( G[i][j], F[i][j+1] + abs(p[i]-p[j+1]), G[i][j+1] + abs(p[j]-p[j+1]) );

            chkMin(F[i][j], F[i-1][j] + abs(p[i]-p[i-1]));
            chkMin(G[i][j], G[i][j+1] + abs(p[j]-p[j+1]));
            chkMin(G[i][j], F[i-1][j] + abs(p[i-1]-p[j]));
            chkMin(F[i][j], G[i][j+1] + abs(p[i]-p[j+1]));
        }
    }

    double ans = 1e300;
    upto(i, N) {
        chkMin(ans, F[i][i], G[i][i]);
    }
    // chkMin(ans, F[N][N], G[N][N]);

    debug display(F);
    debug puts("");
    debug display(G);

    printf("%.3lf\n", ans);
    return 0;
}

/*
5
0 0
0 1
1 1
2 1
1 0

4
0 0
2 2
4 2
2 0
*/