/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=1106
 */
#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)

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
#include <bits/stdc++.h>
namespace lib {
    namespace bit {
        template <class T, class Func>
        inline void traverse1(T x, Func&& f) {  // 按位遍历所有的 1
            for (; x; x&=x-1)  f(x&-x);
        }
        template <class T>
        inline bool greater(T x, T y) {  // 按位大于
            return x & ~y;
        }
    }
}
using namespace lib;

/*
　　给定一些岛屿和一些连接岛屿的桥梁，大家都知道汉密尔顿路是访问每个岛屿一次的路线，
    在我们这个地图中，每个岛屿有个正整数的权值，表示这个岛屿的观赏价值。假设一共有N个岛屿，
    用Vi表示岛屿Ci的价值，汉密尔顿路C1C2....Cn的价值是以下三部分的总和：

　　(1)所有岛屿的价值之和；

　　(2)对于路径中相邻的两个岛屿CiCi+1,把两个岛屿的价值之积加到总价值中；

　　(3)路径中连续三个岛屿CiCi+1Ci+2，如果Ci与Ci+2有桥直接相连，则把这三个岛屿价值之积加到总价值中。

　　要求计算汉密尔顿路最大的价值以及方案数。
*/

const int _Q = 20; int Q; const int _N = 15; int N; int M;

int w[_N];  // 第 i 个岛屿的权值
bool conn[_N][_N];  // 第 i 个岛和第 j 个岛之间是否有桥

typedef uint16_t path;  // 路径状态码；二进制位表示是否经过第 i 个岛屿
typedef int32_t value;  // 路径价值
value F[1<<_N][_N][_N];  // 状态码为 i 的路径的最大价值，并且这条路径的后两个点是 i-1 和 i-2
int C[1<<_N][_N][_N];  // 计数

namespace Solution {
    inline int lg2(int x) {
        if (x == 0) return -1;
        return std::__lg(x);
    }

    void solve() {
        int ans = 0;
        int cnt = 0;
        #define f(x) (w[lg2(x)])

        from(i, 0, N-1) {
            from(j, 0, N-1) {
                if (i==j)  F[1<<i][i][i] = w[i];
                else  F[1<<i|1<<j][j][i] = F[1<<i|1<<j][i][j] = w[i]+w[j] + w[i]*w[j];
                C[1<<i|1<<j][j][i] = C[1<<i|1<<j][i][j] = 1;
            }
        }

        from(p, 0, (1<<(N))-1) {  // 遍历可能的路径
            bit::traverse1(p, [&](int i) {
                bit::traverse1(p^i, [&](int j) {
                    // i, j 是倒数第一个和第二个到达的点（带位值）
                    bit::traverse1(~p&((1<<N)-1), [&](int n) {  // 新到达的点
                        int &np = F[p|n][lg2(n)][lg2(i)];
                        int tmp = F[p][lg2(i)][lg2(j)] + f(n);
                        tmp += f(n) * f(i);
                        if (conn[j][n])  tmp += f(n) * f(i) * f(j);
                        log("F[%d][%d][%d] <(%d)- F[%d][%d][%d]\n", p|n, lg2(n), lg2(i), tmp, p, lg2(i), lg2(j));

                        compare(np, tmp, 
                            0, 
                            C[p|n][lg2(n)][lg2(i)] += C[p][lg2(i)][lg2(j)],
                            (np = tmp, C[p|n][lg2(n)][lg2(i)] = C[p][lg2(i)][lg2(j)])
                        );
                        chkMax(ans, np);
                    });
                });
            });
        }

        from(p, 1, (1<<N)-1) {
            bit::traverse1(p, [&](int i){
                bit::traverse1(p^i, [&](int j) {
                    if (F[p][i][j] == ans)  cnt += C[p][i][j];
                });
            });
        }
        #undef f

        printf("%d %d\n", ans, cnt);
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    scanf("%d", &Q);

    while (Q --> 0) {
        std::memset(w, 0, sizeof(w));
        std::memset(conn, false, sizeof(conn));
        std::memset(F, 0, sizeof(F));

        scanf("%d", &N); scanf("%d", &M);

        from(i, 0, N-1) {
            scanf("%d", w+i);
        }

        int a, b;
        from(_, 0, M-1) {
            scanf("%d%d", & a, & b);
            conn[a-1][b-1] = conn[b-1][a-1] = true;
        }

        Solution::solve();
    }
    
    return 0;
}