#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define reverse(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp) base=std::max((base),(cmp))
#define chkMin(base,cmp) base=std::min((base),(cmp))
#define update(base,op,modify) base=op((base),(modify))
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? v: otw)
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
bool DEBUG_MODE=false;

template <class T=int>
inline T read() { T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

int N, X;
const int maxN = 1005;
int l[maxN], w[maxN], u[maxN];
int f[maxN][maxN];  // 前i个人，不多于j份药，最优方案

int main() {
    initDebug;

    optimizeIO;
    std::cin >> N >> X;
    upto(i, N) {
        std::cin >> l[i] >> w[i] >> u[i];
    }

    from(j, 0, X) {
        upto(i, N) {
            f[i][j] = f[i-1][j];  // 忽略
            chkMax(f[i][j], f[i-1][j] + l[i]);  // 输
            if (j>=u[i])  chkMax(f[i][j], f[i-1][j-u[i]] + w[i]); // 赢
            log("%4d(%2d,%2d)", f[i][j], f[i-1][j] + l[i], f[i-1][j-u[i]] + w[i])
        }
        log("\n")
    }

    std::cout << f[N][X] * 5 << std::endl;
    return 0;
}