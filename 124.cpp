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

const int maxN = 1005;
int N;
std::bitset<maxN> X, Y;
char line[maxN];

int main() {
    initDebug;
    N = read();
    scanf("%s", line);
    from(i, 0, N-1) { X[i]=(line[i]=='H'); };
    scanf("%s", line);
    from(i, 0, N-1) { Y[i]=(line[i]=='H'); };

    debug from(i,0,N-1)   log("%d", X[i]?1:0) log("\n")
    debug from(i,0,N-1)   log("%d", Y[i]?1:0) log("\n")
    X=X^Y;
    debug from(i,0,N-1)   log("%d", X[i]?1:0) log("\n")
    int count=0;
    from(i, 0, N-1) {
        if (X[i] && !(i && X[i-1]))    count++;
    }
    printf("%d\n", count);
    return 0;
}