/**
 * @link https://www.luogu.com.cn/problem/P7914
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

enum TypeCode {  // 类型编号
    asterisks,  // 若干个连续的星号
    single,  // 单个完整括号
    suffixed,  // 以左括号开头，若干个连续的星号结尾
    linked,  // 若干个括号用星号连接，类似(...)***(...)**(...)；包含单括号
    prefixed,  // 以右括号结尾，若干个连续的星号开头
    nodes,  // 星号开头结尾，中间若干个括号，类似**(...)***(...)**(...)****；包含星号串
};
const int _T = 6;

const int _N = 505; int N; const int _K = 505; int K; const int mod=1e9+7;
char s[_N];

ll F[_N][_N][_T];  // F[i][j][k] 表示区间 [i, j] 中类型为 k 的方案数

bool match(int i, int j) {
    return ( (s[i] == '(' || s[i] == '?') && (s[j] == ')' || s[j] == '?') );
}

int main(int argc, char const *argv[]) {
    initDebug;

    never {
        freopen("P7914_in.txt", "r", stdin);
        freopen("P7914_out.txt", "w", stdout);
    }

    scanf("%d", &N); scanf("%d", &K);
    scanf("%s", s+1);

    // S 仅包括连续的不超过 K 个 '*'
    // 如果 A, B 合法：
    // (), (S) 合法
    // A+B, A+S+B 合法
    // (SA), (AS) 合法

    // 特别地，空字符串不合法

    // 星号串需要初始化
    from(i, 1, N)  F[i][i-1][TypeCode::asterisks] = 1;

    from(l, 1, N) {  // 区间长度
        from(i, 1, N) {  // 区间左端点
            int j = i+l-1;  // 区间右端点
            if (j > N) break;
            // 星号串
            // 如果可以，在 [i, j-1] 的基础上添加一个星号
            if (l <= K)
                F[i][j][TypeCode::asterisks] = F[i][j-1][TypeCode::asterisks] && 
                        (s[j] == '*' || s[j] == '?');

            // 单括号
            // 不能直接嵌套括号，即 (()) 形式不合法
            // 不能括号式的两边都是星号，即 nodes 不合法
            F[i][j][TypeCode::single] = match(i, j) * (
                F[i+1][j-1][TypeCode::asterisks] +
                F[i+1][j-1][TypeCode::suffixed] +
                F[i+1][j-1][TypeCode::linked] +
                F[i+1][j-1][TypeCode::prefixed]
            ); F[i][j][TypeCode::single] %= mod;

            // 后缀形式
            // 链接形式添加若干个后续星号
            from(k, i, j-1) {  // [i, k] 为一个链接式
                F[i][j][TypeCode::suffixed] += F[i][k][TypeCode::linked] * F[k+1][j][TypeCode::asterisks];
                F[i][j][TypeCode::suffixed] %= mod;
            } 

            // 前缀形式
            // 前缀式或者节点式追加一个单括号
            from(k, i, j-1) {  // [i, k] 为一个前缀形式
                F[i][j][TypeCode::prefixed] += F[i][k][TypeCode::prefixed] * F[k+1][j][TypeCode::single];
                F[i][j][TypeCode::prefixed] += F[i][k][TypeCode::nodes] * F[k+1][j][TypeCode::single];
                F[i][j][TypeCode::prefixed] %= mod;
            } 

            // 链接形式
            // 后缀形式或链接形式追加一个单括号
            // 直接一个单括号的情况一并加入
            from(k, i, j-1) {  // [i, k] 为一个后缀形式
                F[i][j][TypeCode::linked] += F[i][k][TypeCode::suffixed] * F[k+1][j][TypeCode::single];
                F[i][j][TypeCode::linked] += F[i][k][TypeCode::linked] * F[k+1][j][TypeCode::single];
                F[i][j][TypeCode::linked] %= mod;
            } 
            F[i][j][TypeCode::linked] += F[i][j][TypeCode::single];
            F[i][j][TypeCode::linked] %= mod;

            // 节点形式
            // 前缀形式追加一串星号
            // 星号串一并加入
            from(k, i, j-1) {  // [i, k] 为一个前缀形式
                F[i][j][TypeCode::nodes] += F[i][k][TypeCode::prefixed] * F[k+1][j][TypeCode::asterisks];
                F[i][j][TypeCode::nodes] %= mod;
            }
            F[i][j][TypeCode::nodes] += F[i][j][TypeCode::asterisks];
            F[i][j][TypeCode::nodes] %= mod;
        }
    }

    never {
        from(t, 0, _T-1) {
            printf("----T=%d----\n", t);
            upto(i, N) {
                upto(j, N) {
                    printf("%5lld", F[i][j][t]);
                }
                printf("\n");
            }
        }
    }

    printf("%lld\n", F[1][N][TypeCode::linked]);

    return 0;
}
