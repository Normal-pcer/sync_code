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

const int _N = 43; int N; const int _M = 43; int M; double p[_N*2];
const double R = 1, PI = 3.1415926535897932384626433832795028841971;
double dp[_N][_N][_M];  // [i, j] 区间内最大的 k 边形面积(k<=M)
double ans = 0.0;

double area(double A, double B, double C) {
    // 计算圆周上三点 A, B, C 围成三角形的面积
    // 传入为弧度，且 C>B>A
    return (pow(R, 2) / 2) * ( sin(B-A) + sin(C-B) - sin(C-A) );
}

void displayDp() {
    upto(i, N*2) {
        printf("\n-------i=%d---\n\n", i);
        upto(j, N*2) {
            upto(k, M) {
                printf("%10.2lf", dp[i][j][k]);
            }
            printf("\n");
        }
    }
}

int main() {scanf("%d", &N); scanf("%d", &M); upto(i, N) scanf("%lf", p+i);
    initDebug;
    upto(i, N) {
        p[i] *= 2 * PI;
    }
    std::memset(dp, 0xf0, sizeof(dp));
    // 初始化，预先计算所有三角形
    upto(i, N) {  // 最小
        from(j, i+2, N) {  // 最大
            from(k, i+1, j-1) {  // 中间
                chkMax(dp[i][j][3], area(p[i], p[k], p[j]));
                // chkMax(ans, dp[i][j][k]);
                chkMax(ans, dp[i][j][3]);
            }
        }
    }

    from(l, 4, N+1) {  // 区间长度
        upto(i, N) {  // 区间起点
            int j = i+l-1;  // 区间终点
            if (j > N)  break;

            from(k, i+1, j-1) {  // 对区间 [k, j] 上的原有 p-1 边形追加一个三角形(i, k, j)面积
                from(q, 1, M) {
                    chkMax(dp[i][j][q], dp[k][j][q-1] + area(p[i], p[k], p[j]));
                    chkMax(ans, dp[i][j][q]);
                }
            }
        }
    }
    never displayDp();

    printf("%.6f\n", ans);
    return 0;
}

/**
40
0.0
0.02
0.04
0.06
0.08
0.1
0.12
0.14
0.16
0.18
0.2
0.22
0.24
0.26
0.28
0.3
0.32
0.34
0.36
0.38
0.4
0.42
0.44
0.46
0.48
0.5
0.52
0.54
0.56
0.58
0.6
0.62
0.64
0.66
0.68
0.7
0.72
0.74
0.76
0.78
 */