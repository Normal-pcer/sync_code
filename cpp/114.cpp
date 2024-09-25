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


const int maxN = 102;
char S[maxN], T[maxN];
int N, M;

int main() {
    initDebug;
    scanf("%d%d", &N, &M);
    scanf("%s", S+1);
    scanf("%s", T+1);
    bool isPrefix = true;
    bool isSuffix = true;
    upto(i, N)  ensure(S[i]==T[i], isPrefix);
    from(i, 0, N-1)  ensure(S[N-i]==T[M-i], isSuffix);
    printf("%d\n", (isPrefix?0:2)+(isSuffix?0:1));
    return 0;
}