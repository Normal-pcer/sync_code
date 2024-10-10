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
value F[1<<_N];  // 状态码为 i 的路径的最大价值


namespace Solution {
    inline int __lg(int x) {
        if (x == 0) return -1;
        return std::__lg(x);
    }

    void solve() {
        int ans = 0;
        int cnt = 0;

        from(p, 1, (1<<(N))-1) {  // 遍历可能的路径
            // log("%d\n", p);
            if (p == 7) 
                int debugger = true;
            int high1 = __lg(p);  // 最新的岛屿
            F[p] = F[p^(1<<high1)] + w[high1];  //   (1)加上最新岛屿的价值
            int high2 = __lg(p^(1<<high1));  // 倒数第二个岛屿
            if (high2!=-1) { // 如果存在
                F[p] += w[high2] * w[high1];  // (2)加上相邻岛屿的价值之积
            }
            int high3 = __lg(p^(1<<high1)^(1<<high2));  // 倒数第三个岛屿
            if (high3!=-1) {  // 如果存在
                if (conn[high3][high1])  // 如果倒数第三个岛屿和最新岛屿之间有桥
                    F[p] += w[high3] * w[high2] * w[high1];  // (3)加上三个岛屿的价值之积
            }
            // int high4 = __lg(p^(1<<high1)^(1<<high2)^(1<<high3));
            // if (high4 != -1) {
                
            // }
            chkMax(ans, F[p]);
        }

        from(p, 0b10, 1<<(N+1)) { 
            if (F[p] == ans)  cnt++;
        }

        debug batchOutput(F, 1<<(N+1), "%3d ");

        printf("%d %d\n", ans, cnt);
    }
}

int main() {
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