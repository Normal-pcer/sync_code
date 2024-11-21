/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3141
 * @link https://www.luogu.com.cn/problem/P2831
 */

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


typedef int status;

// 可以使用若干条起点为 (0, 0) 的抛物线 y=ax^2+bx
// 给定 N 个点，求至少需要多少条抛物线（鸟的轨迹）能覆盖所有点（创死所有猪）

int T = read();
const int _N = 20;
const double EPS = 1e-8;

namespace Solution {
    int N, M /* M 没用 */;
    struct Point { double x,y; } p[_N];
    int F[1<<_N]; // i 的每个为 1 的位表示对应的猪亖了

    status covered[_N][_N];  // 点 i 和点 j 之间的抛物线覆盖的所有猪

    bool equals(double a, double b) { return fabs(a-b) < EPS; }

    // 获取题意中的抛物线参数
    // f(x) = ax^2 + bx + 0
    inline std::pair<double, double> parabolaArgs(Point p, Point q) {
        double x1=p.x, y1=p.y, x2=q.x, y2=q.y;
        // 特别地，如果存在两个点在竖直直线上，则舍去（避免除以 0）
        if ( equals(x1, x2) ) return std::make_pair(0, 0);
        double a = (x2*y1 - x1*y2) / ( (x1-x2) * (x1*x2) );
        double b = (x2*x2*y1 - x1*x1*y2) / ( (x1*x2) * (x2-x1) );
        return std::make_pair(a, b);
    }

    void init() {
        memset(p, 0, sizeof(p));
        memset(covered, 0, sizeof(covered));
        memset(F, 0x3f, sizeof(F));
    }

    void read() {
        scanf("%d%d", &N, &M);
        double x, y;
        from(i, 0, N-1) {
            scanf("%lf%lf", &x, &y);
            p[i].x = x, p[i].y = y;
        }        
    }

    void pre() {
        // 预先计算抛物线参数相关
        from(i, 0, N-1) {
            from(j, i+1, N-1) {
                auto args = parabolaArgs(p[i], p[j]);
                // 特别地，开口向上的抛物线或直线要舍去
                if ( args.first >= -EPS ) continue;
                from(k, 0, N-1) {
                    if ( equals(
                        args.first * pow(p[k].x, 2) + args.second * p[k].x,
                        p[k].y
                    ) ) {
                        covered[i][j] |= 1<<k;
                    }
                }
                covered[i][j] |= 1<<i | 1<<j;
            }
            covered[i][N] = 1<<i;   // 提供另外一种选择——用一条只经过 i 的抛物线
                                    // j=N 不会影响常规解
        }
    }

    void solve() {
        F[0] = 0;  // 没有任何猪被创死；自然不需要鸟
        from(i, 0, (1<<N)-1) {  // 可能的状态
            // 这个状态下可以再添一条抛物线
            from(p, 0, N-1) {  // 选定两点确定出一条抛物线
                from(q, p+1, N) {
                    status future = i | covered[p][q];  // 把路径上的猪都创死
                    chkMin(F[future], F[i] + 1);  // 更新状态
                }
            }
        }
        printf("%d\n", F[(1<<N)-1]);
    }
}

int main() {
    initDebug;
    while (T --> 0) {
        Solution::init();
        Solution::read();
        Solution::pre();
        Solution::solve();
    }
    return 0;
}