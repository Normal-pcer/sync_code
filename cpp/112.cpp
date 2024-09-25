#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<=e;i++)
#define reverse(i,e,b) for(int i=e;i>=b;i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp) base=std::max(base,cmp)
#define chkMin(base,cmp) base=std::min(base,cmp)
#define update(base,op,modify) base=op(base,modify)
bool DEBUG_MODE=false;


const int maxN = 300005;
int N, K;
int a[maxN];
int tail[maxN];  // 数值最小的、长度为 i 的最长上升子序列的尾部
int top = 0;

int main() {
    initDebug;
    optimizeIO;
    std::cin >> N >> K;
    upto(i, N)  std::cin >> a[i];
    upto(i, N) {
        if (i<K && a[i]>=a[K])  continue;
        if (i>K && a[i]<=a[K])  continue;
        if (a[i] >= tail[top]) {
            tail[++top] = a[i];
        } else {
            int *j = std::lower_bound(tail+1, tail+top+1, a[i]);
            *j = a[i];
        }
    }
    upto(i, N) {
        log("%3d", tail[i]);
    }
    log("\n")
    printf("%d\n", top);
    return 0;
}