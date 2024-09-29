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

const int _N = 1005; int N;
struct Point {
    double x, y;

    Point operator- (Point const other) const {
        return {this->x - other.x, this->y - other.y};
    }
} tempPoints[_N], p[_N];
auto abs(Point const &p) {
    return sqrt(p.x*p.x + p.y*p.y);
}

double F[_N][_N], G[_N][_N];
char FF[_N][_N], GG[_N][_N];
int origin[_N];

void display(double arr[_N][_N]) {
    upto(i, N) {
        upto(j, N) {
            printf(arr[i][j]<1000000?"%11.2lf":"        INF", arr[i][j]);
        }
        printf("\n");
    }
}

int main() {
    scanf("%d", &N);
    initDebug;

    double tx, ty, maxY; int maxYOwner;
    upto(i, N) {
        scanf("%lf%lf", &tx, &ty);
        tempPoints[i] = {tx, ty};
        chkMaxEx(maxY, maxYOwner=i, {}, ty);
    }

    std::memset(F, 0x70, sizeof(F));
    std::memset(G, 0x70, sizeof(G));
    debug {
        std::memset(FF, '.', sizeof(FF));
        std::memset(GG, '.', sizeof(GG));
    }

    int pIndex = 0;
    from(i, maxYOwner, N) {
        p[++pIndex] = tempPoints[i];
        origin[pIndex] = i;
    }
    from(i, 1, maxYOwner-1) {
        p[++pIndex] = tempPoints[i];
        origin[pIndex] = i;
    }

    // 初始化
    F[1][N] = 0;
    G[2][N] = abs(p[1]-p[N]);

    from(i, 1, N) {
        rev(j, N, i) {
            FF[i][j] = GG[i][j] = '.';
            chkMinEx(F[i][j], FF[i][j]='<', {}, F[i-1][j] + abs(p[i-1]-p[i]));
            chkMinEx(G[i][j], GG[i][j]='<', {}, G[i][j+1] + abs(p[j+1]-p[j]));
            chkMinEx(G[i][j], GG[i][j]='$', {}, F[i-1][j] + abs(p[i-1]-p[j]));
            chkMinEx(F[i][j], FF[i][j]='$', {}, G[i][j+1] + abs(p[j+1]-p[i]));
        }
    }

    double ans = 1e305;
    double ansIndex = 0;
    char in = '.';
    upto(i, N) {
        chkMinEx(ans, {ansIndex = i; in='F';}, {}, F[i][i]);
        chkMinEx(ans, {ansIndex = i; in='G';}, {}, G[i][i]);
    }

    debug batchOutput2d(FF, N, N, "%c");
    debug batchOutput2d(GG, N, N, "%c");
    debug display(F);
    debug display(G);
    debug batchOutput(origin, N);

    int i = ansIndex, j = ansIndex;
    std::vector<int> results;
    while (true) {
        if ((ll)results.size() == N)  break;
        if (in == 'F') {
            results.push_back(i);
            if (FF[i][j] == '<') {
                i--;
            } else if (FF[i][j] == '$') {
                in = 'G';
                j++;
            }
        } else if (in == 'G') {
            results.push_back(j);
            if (GG[i][j] == '<') {
                j++;
            } else if (FF[i][j] == '$') {
                in = 'F';
                i--;
            }
        }
    }

    std::reverse(results.begin(), results.end());
    for (auto item: results) {
        printf("%d ", origin[item]);
    }
    printf("\n");
    // printf("\n%lf\n", ans);
    return 0;
}