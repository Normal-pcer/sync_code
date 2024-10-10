/**
 * https://neooj.com:8082/oldoj/problem.php?id=1668
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

// N*N 的国际象棋棋盘，要放 K 个马
// 求放法数，使得它们不会互相攻击

// 定义一个格子“已占用”(Occupied)——当且仅当它被马占据，或在其攻击范围内
// 用一个八位二进制数表示一行，低位对应靠左的格子，1 表示已占用，0 表示未占用
typedef uint8_t status;

namespace Solution {
    const int _N = 8; int N, K;
    int F[_N][1<<_N][1<<_N][20];  // 前 i 行中：第 i-1 行的状态为 j，第 i 行的状态为 k，其它行放满，总共放了 l 个马的方案数
    // std::vector<std::vector<status>> singleLine(10);  // 一行中放了 i 个马的所有可能方案
    // struct {
    //     inline int operator[] (int num) {
    //         return __builtin_popcount(num);
    //     }
    // } popcount;

    void pre() {
        // 预处理 popcount
        // for (status s=0; s<(1<<N); s++) {
        //     int cnt = std::bitset<_N>(s).count();
        //     __builtin_popcount(s) = cnt;
        //     singleLine[cnt].push_back(s);
        // }
    }
    
    void solve() {
        if (K>=10) {
            printf("0\n");
            return;
        }
        // pre();
        ll ans = 0;
        for (status j=0; j<(1<<N); j++)  F[1][j][0][__builtin_popcount(j)] = 1;
        from(i, 2, N) {  // 在第 i 行放置
            for (status j=0; j<(1<<N); j++) {  // 本行状态
                for (status l=0; l<(1<<N); l++) {  // 前行
                    if ((j & (l<<2)) || (j & (l>>2)))  continue;
                    for (status k=0; k<(1<<N); k++) {  // 前前行
                        if (!(
                            (j & (k<<1)) ||
                            (j & (k>>1)) ||
                            (l & (k<<2)) ||
                            (l & (k>>2))
                        )) {
                            for (status c=__builtin_popcount(j); c<=K; c++) {  // 本行马的数量
                                F[i][j][l][c] += F[i-1][l][k][c-__builtin_popcount(j)];
                            }
                        }
                    }
                }
            }
        }

        // 计算答案
        for (status i=0; i<(1<<N); i++) {
            for (status j=0; j<(1<<N); j++) {
                ans += F[N][j][i][K];
            }
        }
        printf("%lld\n", ans);
    }
}

using namespace Solution;

int main() {
    scanf("%d%d", &N, &K);
    initDebug;
    Solution::solve();
    return 0;
}
