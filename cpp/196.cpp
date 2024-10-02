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
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define putInt(n) printf("%d\n",(n))
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) from(i,0,r){from(j,0,c)printf(fmt,b[i][j]);printf("\n");}
template <class T=int>inline T read(){ T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }
typedef unsigned short status;  // 低位->高位 对应 从左到右每一块是(1)否(0)被填充



const int _M = 15; int M; const int _N = 15; int N;

ll F[_M][1<<_N];  // 前 i 行，最后一行状态为 j，且其它行填满的方法数

// 对相邻的两行——上、下 进行检查
void dfs(
    int p,          // 0~(p-1) 的位置已经检查完毕
    status s,       // 上行经过目前填充之后的状态
    status t,       // 下行经过目前填充之后的状态
    status src,     // 整个过程的初始状态
    int tgt         // 下行的行号
) {
    if (p >= N) {   // 搜索完毕
        if (tgt==3 && t==0)
            int debugger = true;
log("F[%d][%d] += F[%d][%d] args:(%d,%hd,%hd,%hd,%d)\n", tgt, t, tgt-1, src, tgt, s, t, src, tgt);
        // 对于能够转移到 F[tgt-1][src] 的每一种方法，都能够转移到 F[tgt][t]
        F[tgt][t] += F[tgt-1][src];  // 记录本次方案
        return;  // 结束递归
    }
    
    if (s & 1<<p) {  // 当前遍历的格被占用
        dfs(p+1, s, t, src, tgt);  // 跳过这一格
    } else {
        // 尝试用不同方法填充
        // 1*2
        if(p+1<N && !(s&(1<<(p+1))))    dfs(p+2, s|(1<<p)|(1<<(p+1)), t, src, tgt);     // 横排
        if(!(t&(1<<p)))                 dfs(p+1, s|(1<<p), t|(1<<p), src, tgt);         // 竖排
        // 1*2+1
        if (p-1>=0 && !(t&(1<<p)) && !(t&(1<<(p-1))))
            dfs(p+1, s|(1<<p), t|(1<<p)|(1<<(p-1)), src, tgt);          // 缺左上 - 」
        if (p+1<N && !(t&(1<<p)) && !(t&(1<<(p+1))))
            dfs(p+1, s|(1<<p), t|(1<<p)|(1<<(p+1)), src, tgt);          // 缺右上 - L
        if (p+1<N && !(s&(1<<(p+1))) && !(t&(1<<(p+1))))
            dfs(p+2, s|(1<<p)|(1<<(p+1)), t|(1<<(p+1)), src, tgt);      // 缺左下 - 7
        if (p+1<N && !(s&(1<<(p+1))) && !(t&(1<<p)))
            dfs(p+2, s|(1<<p)|(1<<(p+1)), t|(1<<p), src, tgt);          // 缺右下 - 「
    }
}

// *弃用
// 专用于初始化 F[1][x] 的dfs
// 仅尝试放置横排的1*2，并记录每个结果(F[1][p])
void init_dfs(
    int p,          // 0~(p-1) 的位置已经检查完毕
    status s        // 经过目前填充之后的状态
) {
    // 记录当前状态
    F[1][s] = 1;
    if (p >= N)  return;  // 结束搜索
    // 找合适的位置放块
    from(i, p+1, N-2) {
        // 在 i 和 i+1 上放块
        init_dfs(i+2, s|(1<<i)|(1<<(i+1)));
    }
}

// *弃用
// 专用于收尾的 dfs
// 对于一个状态 j，尝试放置若干个 1*2 的块，使得最终状态为全满(0b111...)
// 返回可能的方案数
ll end_dfs(
    int p,          // 0~(p-1) 的位置已经检查完毕
    status s        // 经过目前填充之后的状态
) {
    if ((s == (1<<N)-1))  return 1;       // 如果已经全满，则返回 1
    if (p >= N)  return (s == (1<<N)-1);  // 搜索完毕，返回是否全满
    ll ans = 0;
    // 尝试放置块
    from(i, p+1, N-2) {
        // 在 i 和 i+1 上放块
        if (!(s&(1<<i)) && !(s&(1<<(i+1))))
            ans += end_dfs(i+2, s|(1<<i)|(1<<(i+1)));
    }
    return ans;
}

int main() {
    scanf("%d", &M); scanf("%d", &N);
    initDebug;

    // init_dfs(-1, 0);  // 初始化 F[1][x]
    F[1][0] = 1;
    from(i, 2, M+1) {
        from(j, 0, (1<<N)-1) {  // 第 (i-1) 行的状态为 j
            if (!F[i-1][j]) continue;  // 如果没有方案，则跳过
            dfs(0, j, 0, j, i);  // 更新第 i 行的状态
        }
    }

    debug batchOutput2d(F, M+1, 1<<N, "%5lld ");

    printf("%lld\n", F[M+1][0]);  // 输出：填充了全部 M 行，且最后一行状态为全满
    return 0;
}