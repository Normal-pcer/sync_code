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
#define ensure(con,v) if(!(con))v=false;
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
#define rollback(v, bak) ((v)?(v):(bak))
bool DEBUG_MODE=false;
 
 
const int maxN = 100005;
long long h[maxN];
int t[maxN], top=1;
int N;
 
int main() {
    optimizeIO;
    initDebug;
    std::cin >> N;
    upto(i, N)  std::cin >> h[i];
    reverse(i, N, 1) {
        if (h[i] >= t[top])     t[++top] = h[i];
        else    *std::lower_bound(t+1, t+top+1, h[i]) = h[i];
    }
    printf("%d\n", top-1);
    top = 0; std::memset(t, 0, sizeof(t));
    upto(i, N) {
        if (h[i] > t[top])     t[++top] = h[i];
        else    *std::lower_bound(t+1, t+top+1, h[i]) = h[i];
    }
    printf("%d\n", top);
    return 0;
}