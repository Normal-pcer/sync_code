/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=1101
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

const int _N = 21; int N;
double a[_N][_N];  // i 完成任务 j 的概率（单位：1%）
double F[1<<_N];    // 前 i 个人，使得任务分配状态(每一位用 1 表示已分配，0 表示未分配)
                        // 为 j 的总完成率最大。(单位：100%)

int LOG2[1<<_N];

int main() {
    initDebug;
    scanf("%d", &N);

    // 预处理 log2
    from(i, 0, N) {
        LOG2[1<<i] = i;
    }

    double t;
    upto(i, N) upto(j, N) {
        scanf("%lf", &t);
        a[i][j] = t / 100.0;
    }

    // 分配任务，使得总完成率最高。每个人只能做一件事
    F[0] = 1.00;
    from(i, 1, N) {
        for (status j=(1<<N)-1; j>0; j--) {
            // 给这个人分配其中的一个任务
            // for (int k=0; k<N; k++) {
            //     status mask = 1<<k;
            //     if (j & mask) {
            //         status prev = j ^ mask;  // 这个数必然小于 j
            //         chkMax(F[j], F[prev] * (a[i][k+1]));
            //     }
            // }

            status mask;  // x & -x
            for (status x=j; x; x -= mask) {
                mask = x & -x;
                status prev = j ^ mask;
                chkMax(F[j], F[prev] * (a[i][LOG2[mask]+1]));
            }
        }
    }

    printf("%.6lf\n", F[(1<<N)-1] * 100);
    return 0;
}