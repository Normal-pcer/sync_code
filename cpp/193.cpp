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



const int _N = 105; int N; const int _M = 105; int M; char s[_N][_M];
struct obj{int origin; ll v;} a[_M];
ll score[_N];
int main() {scanf("%d", &N); scanf("%d", &M);
    initDebug;
    upto(i, M)  scanf("%lld", &a[i].v);
    upto(i, N)  scanf("%s", s[i]+1);
    upto(i, M) {
        a[i].origin = i;
    }

    // S[i][j] = 'o' - i solved j
    upto(i, N) {
        upto(j, M) {
            if (s[i][j] == 'o') score[i] += a[j].v;
        }
        score[i] += i;
    }

    std::sort(a+1, a+M+1, [](obj a, obj b){return a.v > b.v;});

    ll max_s = 0;
    upto(i, N)  chkMax(max_s, score[i]);

    upto(i, N) {
        ll sc = score[i];
        // until sc > max_s
        int cnt = 0, index = 0;;
        if (sc != max_s)
            while (sc <= max_s) {
                if (s[i][a[++index].origin] != 'o') 
                    sc += a[index].v, cnt++;
            }
        printf("%d\n", cnt);
    }
    return 0;
}