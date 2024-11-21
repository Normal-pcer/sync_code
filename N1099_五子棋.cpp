/*
@link https://neooj.com:8082/oldoj/problem.php?id=1099
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

#define bitGreater(a, b) ( (a) & (~(b)) )

typedef uint16_t status;

int T = read();
const int _N = 14;
int N;

int     xp[_N][_N],     /* 选手 i 在和 j 比赛中必然获得经验 */
        score[_N][_N],  /* 积分 */
        init[_N];       /* 初始经验 */

int F[1<<_N];           // 选手 1 获得的最高积分；其中，i 表示是否与对应选手比赛过
int e[1<<_N];           // i 状态下必然会获得的经验

int main() {
    initDebug;

    while (T --> 0) {
        std::memset(F, 0, sizeof(F));
        std::memset(e, 0, sizeof(e));
        std::memset(xp, 0, sizeof(xp));
        std::memset(score, 0, sizeof(score));
        std::memset(init, 0, sizeof(init));
        N = read();
        upto(i, N)  upto(j, N)  scanf("%d", &xp[i][j]);
        upto(i, N)  upto(j, N)  scanf("%d", &score[i][j]);
        upto(i, N)  scanf("%d", &init[i]);

        // 预处理 e[i]
        from(p, 0, 1<<N) {
            from(i, 0, N-2) {
                status mask = 1<<i;
                if (p & mask) {
                    e[p] += xp[1][i+2];
                }
            }
        }

        // 始终经验较多的人胜利
        // 希望选手 1 获得更高积分
        // 如果对手拥有的经验尽可能少，则自己更可能获得胜利
        // 而随着时间推移，所有人的经验单调不减
        // 故断言：选手 1 获取的积分最多，只需他最先和其他人打一遍
        // 只需考虑选手 1 和其他人比赛的次序
        // 并且此时选手 i (i>1) 的经验均为初始值 init[i]
        // 由于选手 0 和 1 没有意义，可以使用第 0 位表示第二个选手

        int ans = 0;

        for(status i=0; i <= (1<<(N-1))-1; i++) {  // 是否比赛过
            from(p, 0, N-2) {
                status mask = 1<<p;
                if (i & mask) {
                    status prev = i ^ mask;
                    int expHost = e[prev] + init[1];
                    int expGuest = init[p+2];
                    if (expHost > expGuest) {
                        chkMax(F[i], F[prev] + score[1][p+2]);
                    } else {
                        chkMax(F[i], F[prev]);
                    }
                }
            }
            chkMax(ans, F[i]);
        }

        debug batchOutput(F, 1<<N);

        putInt(ans);  // 去掉占位的选手 0 和 选手 1 自己
    }

    return 0;
}