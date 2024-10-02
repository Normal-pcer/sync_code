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


const int _N = 100005; int N;
struct obj {ll s, c;
    bool operator<(const obj &o) const {
        return this->s < o.s;
    }
    bool operator==(const obj &o) const {
        return this->s == o.s;
    }
};// c[i] 个 size=s[i]
int main() {
    scanf("%d", &N);
    std::set<obj> p;
    ll s, c;
    from(i, 0, N-1) {
        scanf("%lld%lld", &s, &c);
        p.insert(obj{s, c});
    }
    initDebug;
    // combine: i, i -> 2i (no order)
    // -> least last amount
    // combine if possible
    int ans = 0;
    while (p.size()) {    
        auto t = *p.begin();
        if (t.c > 1) {
            ll n = t.c / 2;
            t.c -= n*2;
            auto tetr = obj{t.s*2, n};
            auto pos = p.find(tetr);
            if (pos != p.end()) {
                auto tc = pos->c;
                p.erase(pos);
                p.insert(obj{tetr.s, n+tc});
            } else {
                // insert new
                p.insert(obj{tetr.s, n});
            }
        }
        ans += t.c;
        // erase old
        p.erase(t);
    }

    for (auto x:p)  ans += x.c;
    putInt(ans);
    return 0;
}