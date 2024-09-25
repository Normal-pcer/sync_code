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
#define ensure(con,v) if(!(con))v=false;
bool DEBUG_MODE=false;


const int maxN = (int)2e5+5;
int M, N;
int A[maxN];
int days[maxN];

int main() {
    initDebug;
    optimizeIO;
    std::cin >> N >> M;
    upto(i, M)  std::cin >> A[i];
    int index = M;
    reverse(i, N, 1) {
        if (A[index-1] >= i)  index--;
        days[i] = A[index] - i;
    }
    upto(i,N)  printf("%d\n", days[i]);
    return 0;
}