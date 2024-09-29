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


const int _N = 2005; int N; const int _K = 505; int K; int cost[_N]; int F[_N][_N]; int G[_N][_N]; int maxIn[_N][_N]; int sigma[_N];
int ans = Infinity;

int sumIn(int l, int r) {
    if (l>r)  return 0;
    assert(l>=0 && r>=0 && l<=N && r<=N);
    return sigma[r] - sigma[l-1];
}

void display(int arr[_N][_N]) {
    upto(i, N*2) {
        upto(j, N*2) {
            printf(arr[i][j]<1000000?"%9d":"      INF", arr[i][j]);
        }
        printf("\n");
    }
}

int main() {scanf("%d", &N); scanf("%d", &K); upto(i, N) scanf("%d", cost+i); std::memset(F, 0x3f, sizeof(F)); std::memset(G, 0x3f, sizeof(G));
    initDebug;
    K = K+1;
    cost[N+1] = cost[1];
    // 预处理区间最值与前缀和
    upto(i, N+1) {
        from(j, i, N+1) {
            maxIn[i][j] = std::max(maxIn[i][j-1], cost[j]);
        }
        sigma[i] = sigma[i-1] + cost[i];
    }

    int init = 0;
    from(i, 1, K)  init += cost[i];
    from(i, N-K+2, N)  init += cost[i];
    F[K][N-K+1] = G[K+1][N-K+2] = init;
    from(i, K+1, N) {
        rev(j, N-K+2, i) {
            // if (i-K>=0)      chkMin(F[i][j], F[i-K][j] + maxIn[i-K+1][j] + sumIn(i-(K-1)-(K-1), i));
            // if (i-(K-1)>=0)  chkMin(F[i][j], G[i-(K-1)][j+1] + maxIn[i-(K-1)][j] + sumIn(i-(K-1)-(K-1), i));
            // if (j+K<=N)     chkMin(G[i][j], G[i][j+K] + maxIn[i][j+K-1] + sumIn(j+(K-1)+(K-1), j));
            // if (j+K-1<=N)   chkMin(G[i][j], F[i-1][j+K-1] + maxIn[i][j+K-1] + sumIn(j+(K-1)+(K-1), j));
            // if (j-i<=K) {
            //     chkMin(ans, F[i][j] + maxIn[i+1][j] + sumIn(i+1, j));
            //     chkMin(ans, G[i][j] + maxIn[i][j-1] + sumIn(i, j-1));
            // }
            chkMin(F[i][j], F[i-1][j] + maxIn[i][j] + cost[i]);
            chkMin(G[i][j], F[i-1][j] + maxIn[i][j] * (N-j+i-2*K+1) + cost[j]);  // (N-j+i-2*K+1) 推错了 [开始为(N-j+i-2*K+2)]
            chkMin(G[i][j], G[i][j+1] + maxIn[i][j] + cost[j]);
            chkMin(F[i][j], G[i][j+1] + maxIn[i][j] * (N-j+i-2*K+1) + cost[i]);
            log("F[%d][%d] = %d\nG[%d][%d] = %d\n", i, j, F[i][j], i, j, G[i][j]);
            // if (j-i<K) {
            //     chkMin(ans, F[i][j], G[i][j]);
            // }
        }
    }
    debug {
        // display(maxIn);
        display(F);
        puts("");
        display(G);
    }
    upto(i, N+1)
        chkMin(ans, F[i][i], G[i][i]);
    printf("%d\n", ans);
    return 0;
}