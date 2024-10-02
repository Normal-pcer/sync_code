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


// 按位严格大于
#define bitGreater(a, b) ( (a) & (~(b)) )
typedef unsigned short encode;

const int _W=13, _H=13;
int W; int H;

std::vector<encode> singleLine; // 每一行可能的编码；0 为横排，1 为竖排

void dfs(int p /*最后一个横块右侧一点*/, encode left /*当前的编码*/) {
    // 不再放置横排块
    singleLine.push_back(left);
    if (p>=W-1)  return;  // 没有空间继续放横块
    from(i, p, W-2) {
        // 在 i, i+1 两点放置横排块
        dfs(i+2, left & ~(1<<i | 1<<(i+1)) /*将 i, i+1 位置 0，其他位置 1*/);
    }
}

void preProcess() {
    // 预处理每行的可能情况
    // 起初从 p=0 开始搜索，所有的 W 位均为 1
    dfs(0, (1<<W)-1);
}

ll F[_H][(1<<_W)];  // 前 i 行中，第 i 行编码为 j 时的方案数

int main() {scanf("%d", &H); scanf("%d", &W);
    initDebug;

    preProcess();

    for (auto k: singleLine) {
        F[1][k] = 1;  // 第 1 行的方案数均为 1
    }

    from(i, 2, H) {
        from (j, 0, 1<<W) {  // i-1 行的一种可能排列
            for (auto k: singleLine) {  // i 行的一种可能排列
                // 检查这种排列是否合法
                // j 的每一位 "1" 的下方必须对应一个 "1"，"0" 的下方无限制
                // 按位严格大于即可判断，bitGreater(j, k) 非零即为不合法
                if (bitGreater(j, k)) continue;
                // 把 "1"(j) 下方的 "1"(k) 替换成 "0"，因为此时不再有上述限制
                // 这需要通过按位严格小于
                update(k, bitGreater, j);
                F[i][k] += F[i-1][j];
            }
        }
    }

    ll ans = F[H][0];

    printf("%lld\n", ans);
    return 0;
}