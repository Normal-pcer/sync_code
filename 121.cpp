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


const int maxN = 1005;
int N, M;
int H[maxN];
int f[maxN][2005];  // 前i块，高度差为j，较矮的一个的最高高度
int sum;


int main() {
    initDebug;
    optimizeIO;
    std::cin >> N;
    upto(i, N)  std::cin >> H[i];
    std::memset(f, -0x3f, sizeof(f));
    f[0][0] = 0;
    from(i, 1, N) {
        sum += H[i];
        from(j, 0, sum) {
            f[i][j] = f[i-1][j];  // 不放
            int h1 = j - H[i];  // 高的继续添
            if (h1>=0)  chkMax(f[i][j], f[i-1][h1]);
            int h2 = j + H[i];  // 放在低的，不改变相对高度关系
            if (h2<=sum)  chkMax(f[i][j], f[i-1][h2] + H[i]);
            int h3 = H[i] - j;  // 改变
            if (h3>=0)  chkMax(f[i][j], f[i-1][h3] + h3);
        }
    }
    debug upto(i, N+1) {
        from(j, 0, sum) {
            if (-9999<=f[i][j])
                printf("%5d", f[i][j]);
            else
                printf(" -INF");
        }
        printf("\n");
    }
    if (f[N][0])
        printf("%d\n", f[N][0]);
    else
        printf("Impossible\n");
    return 0;
}