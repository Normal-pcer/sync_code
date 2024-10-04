/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=2171
 */

#include <bits/stdc++.h>
#pragma GCC optimize(3, "Ofast", "inline")
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
在 N(<=10000) 个价值为 v 的葡萄中选择若干个，使得取出的总价值减去剩余的总价值的差值最高。
要求：连续的 K(<=10) 个中必须选择至少 A 个，至多 B 个。
*/


typedef uint16_t status;

namespace Solution {
    const int _N = 10005, _K=10;
    int N, K, A, B;
    int v[_N];  // !从 0 开始存
    ll sum = 0;

    // 考虑状态压缩，用一个整数的二进制位表示对应的葡萄是否被选择
    // 另外，由于和一定，价值差最大只需取走的价值最高
    // 78.16 MiB
    ll F[_N][1<<_K];  // F[i][j] 表示第 i 组连续的葡萄中，状态为 j 的最大价值
    int8_t one[1<<_K];  // 二进制表示中 "1" 的个数
    // 大位代表的葡萄编号更高

    void init() {
        from(i, 0, (1<<K)-1) {
            one[i] = std::bitset<10>(i).count();
        }
    }

    void dfs(int l, int r, status s, ll sum) {
        log("dfs(%d, %d, %d, %lld)\n", l, r, s, sum);
        F[0][s] = sum;
        if (r<l)  return;
        from(i, l, r) {
            dfs(i+1, r, s|(1<<i), sum+v[i]);
        }
    }

    void solve() {
        // Your code here...
        optimizeIO;
        std::cin >> N >> K >> A >> B;
        from(i, 0, N-1) {
            std::cin >> v[i];
            sum += v[i];
        }
        
        init();
        std::memset(F, -0x3f, sizeof(F));  // 规避不合法情况——设成负无穷

        // 初始化
        dfs(0, B-1, 0, 0);
        ll max = 0;

        debug {
            from(i, 0, N) {
                from(j, 0, (1<<K)-1) {
                    if (F[i][j] > -1000) {
                        printf("%6lld", F[i][j]);
                    } else {
                        printf("  -INF");
                    }
                }
                printf("\n");
            }
        }

        from(i, 0, N-K) {
            for(status j=0; j<(1<<K); j++) {  // 一种可能的情况
                chkMax(max, F[i][j]);
                if (A <= one[j] && one[j] < B) {
                    // 可以再取一个
                    // 易知 至多取最靠后（大端）的 几 个
                    // one[i] = B-1 时 取 1 个
                    // x = B-one[i]
                    from(k, K-B+one[i], K-1) {  // 取第 k 个
                        if ( !(j & (1<<k)) ) {  // 没有被取过
                            log("(%lld)F[%d][%d]+(%d)v[%d] -> (%lld)F[%d][%d]  {%d %d %d}\n", F[i][j], i, j, v[i+k], 
                                i+k, F[i][j | (1<<k)], i, j|(1<<k), i, j, k);
                            chkMax(F[i][j | (1<<k)], F[i][j] + v[i+k]);
                        }
                    }
                    
                }
                if (A<= one[j] && one[j] <= K) {
                    // 转移到下一组
                    // 舍弃最前面的葡萄，即小位，故按位右移
                    status next = j>>1;
                    log("(%lld)F[%d][%d] -> (%lld)F[%d][%d]  {%d %d}\n" , F[i][j], i, j, F[i+1][next], i+1, next, i, j);
                    chkMax(F[i+1][next], F[i][j]);
                }
            }
        }

        debug {
            from(i, 0, N) {
                from(j, 0, (1<<K)-1) {
                    if (F[i][j] > -1000) {
                        printf("%6lld", F[i][j]);
                    } else {
                        printf("  -INF");
                    }
                }
                printf("\n");
            }
        }

        // printf("%lld\n", ans);
        // 从最大值计算最大差值
        ll remain = sum - max;
        printf("%lld\n", max - remain);
    }
}

int main() {
    initDebug;
    Solution::solve();
    return 0;
}