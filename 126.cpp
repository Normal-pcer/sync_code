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

const int maxN=103, maxW=10002;
int N, T;
int w[maxN][2], v[maxN][2];
int f[maxN][maxW];  // 前i个物品，重量为j，最大价值

int main() {
    initDebug;
    N = read(); T = read();
    upto(i, N) {
        w[i][0] = read(); v[i][0] = read(); w[i][1] = read(); v[i][1] = read();
    }

    upto(j, T) {
        upto(i, N) {
            f[i][j] = f[i-1][j];  // 不装
            if (j>=w[i][0])  chkMax(f[i][j], f[i-1][ j-w[i][0] ] + v[i][0]);      // 装[0]
            if (j>=w[i][1])  chkMax(f[i][j], f[i-1][ j-w[i][1] ] + v[i][1]);      // 装[1]
            log("%4d", f[i][j])
        }
        log("\n")
    }

    printf("%d\n", f[N][T]);
    return 0;
}