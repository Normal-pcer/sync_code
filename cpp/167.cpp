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
#define logpts debug {upto(i, N)  points[i].display(); printf("\n");}
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

const int _N = 53; int N;
const double PI = 3.14159265358979323846;

struct Point {
    double x, y;
    double theta = 0;

    void display() {
        printf("(%.2lf, %.2lf) ", this->x, this->y);
    }

    // void updateTheta() {
    //     this->theta = atan2(this->y, this->x);
    //     if (this->theta < 0.0) {
    //         this->theta = this->theta + PI * 2;  // x轴正半轴为0，逆时针，弧度制
    //     }
    // }

    void operator -= (Point sub) {
        this->x -= sub.x;
        this->y -= sub.y;
        // this->updateTheta();
    }

    Point operator - (Point sub) const {
        return { this->x - sub.x, this->y - sub.y };
    }
} points[_N];
Point base;

double abs(Point p) {
    return sqrt( pow(p.x, 2) + pow(p.y, 2) );
}

double areaSquare(Point A, Point B, Point C) {  // 三角形面积的平方
    double a = abs(B-C);
    double b = abs(C-A);
    double c = abs(A-B);
    double p = (a+b+c) / 2.0;
    debug {
        A.display(); B.display(); C.display();
        printf("%lf\n", p * (p-a) * (p-b) * (p-c));
    }
    return p * (p-a) * (p-b) * (p-c);
}

double dp[_N][_N];  // 区间[i, j]的最小面积平方和

void displayDp() {
    upto(i, N) {
        upto(j, N) {
            if (dp[i][j] > 1000000000)         printf("        INF");
            else if (dp[i][j] < -1000000000)   printf("       -INF");
            else  printf("%11.0lf", dp[i][j]);
        }
        printf("\n");
    }
}

int main() {
    never {
        freopen("168.in", "r", stdin);
        freopen("167.out", "w", stdout);
    }

    scanf("%d", &N);
    initDebug;
    double averageX=0.0, averageY=0.0;
    upto(i, N) {
        scanf("%lf%lf", &points[i].x, &points[i].y);
        averageX += points[i].x;
        averageY += points[i].y;
    }
    
    averageX /= (double)N; averageY /= (double)N;
    base = {averageX, averageY};
    upto(i, N)  points[i] -= base;

    std::sort( points+1, points+1+N, [](Point a, Point b){ return atan2(a.y, a.x) < atan2(b.y, b.x); } );
    std::memset( dp, 127, sizeof(dp) );
    logpts;

    double averageArea = 0;
    from(i, 2, N-1) {
        averageArea += sqrt( areaSquare(points[1], points[i], points[i+1]) );
        log("%lf ", averageArea);
    }
    averageArea /= N-2;
    log("%lf\n", averageArea);

    upto(j, N) {
        if (j+2 > N)  break;
        dp[j][j+2] = areaSquare(points[j], points[j+1], points[j+2]);
        dp[j][j] = dp[j][j+1] = 0;
    }
    dp[N-1][N] = 0; // 绷

    from(i, 3, N-1) {  // 区间长度
        upto(j, N) {  // 区间起点
            if (j+i > N)  break;
            from(k, j+1, j+i-1) {
                // chkMinEx(dp[j][j+i], {log("%d %d %d\n", i, j, k); debug displayDp();} ,;, dp[j][k] + dp[k][j+i] + areaSquare(points[j], points[k], points[j+i]));
                chkMin(dp[j][j+i], dp[j][k] + dp[k][j+i] + areaSquare(points[j], points[k], points[j+i]));
            }
        }
    }
    debug displayDp();

    log("%lf\n", averageArea);
    printf( "%.2lf\n", sqrt( fabs((dp[1][N] / (N-2)) - pow(averageArea, 2)) ) );
    return 0;
}

/*
6
1.414 0
0.707 0.707
-0.707 0.707
-1.414 0
-0.707 -0.707
0.707 -0.707
*/