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


const int _N = 4; int N; const int _M = 4; int M;
int F[_N][1<<_M];

void dfs(int p/*最右侧未被遍历的块*/, 
    int left/*前面的块的状态*/, 
    int next/*下一行的状态*/,
    int src/*最初状态*/,
    int tgt/*下一行的行号*/) {
    never log("dfs (%d, %d, %d, %d, %d)", p, left, next, src, tgt)

    F[tgt][next] += F[tgt-1][src];
    // 所有块都经过填充
    if (p >= M) {
        return;
    }
    if (left & (1<<p)) {
        // 被占用
        dfs(p+1, left, next, src, tgt);
    }
    else {
        dfs(p+1, left|(1<<p), next|(1<<p), src, tgt);  // 放一个1*2
        if (p+1<M && ~(next&((1<<p) | (1<<(p+1)))))  dfs(p+1, left|(1<<p), next|(1<<p)|(1<<(p+1)), src, tgt);  // _|
        if (p-1>1 && ~(next&((1<<p) | (1<<(p-1)))))  dfs(p+1, left|(1<<p), next|(1<<p)|(1<<(p-1)), src, tgt);  // |_
        if (p+1<M && ~(left&(1<<(p+1))))  dfs(p+2, left|(1<<p)|(1<<(p+1)), next, src, tgt);  // 2*1
        if (p+1<M && ~(next&((1<<p))) && ~(left&((1<<p) | (1<<(p+1)))))  dfs(p+2, left|(1<<p)|(1<<(p+1)), next|(1<<p), src, tgt);  // |-
        if (p+1<M && ~((1<<(p+1))) && ~(left&((1<<(p+1)) | (1<<p))))  dfs(p+1, left|(1<<p)|(1<<(p+1)), next|(1<<(p+1)), src, tgt);  // -|
    }
}

int main() {scanf("%d", &N); scanf("%d", &M);
    initDebug;

    F[0][0] = 1;
    upto(i, N) {  // 对于前 i 行
        from(j, 0, 1<<M) {   // 第 i 行的一种状态
            if (F[i-1][j]) {  // 可能出现
                dfs(0, 0, 0, 0, i);  // 搜索可能的情况，向下转移
            }
        }
    }

    debug batchOutput2d(F, N+1, 1<<M, "%2d ");

    printf("%d\n", F[N][(1<<M)-1]);
    return 0;
}