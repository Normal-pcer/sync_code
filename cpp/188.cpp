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



#define ls (p<<1)
#define rs (ls|1)
#define mid ((l+r)>>1)

const int _N = 2e5+5;
int N = read(), M = read();

struct node {
    int l, r;
    int cnt;  // 打开的数量
    bool tag;
} tr[_N*4];

void push_down(int p) {
    if (tr[p].tag) {
        tr[ls].cnt = tr[ls].r - tr[ls].l + 1 - tr[ls].cnt;
        tr[rs].cnt = tr[rs].r - tr[rs].l + 1 - tr[rs].cnt;
        tr[ls].tag ^= true;
        tr[rs].tag ^= true;
        tr[p].tag = false;
    }
}

void push_up(int p) {
    tr[p].cnt = tr[ls].cnt + tr[rs].cnt;
}

void build(int p, int l, int r) {
    tr[p].l = l; tr[p].r = r;
    if (l==r) { tr[p].cnt = 0; return; }
    build(ls, l, mid);
    build(rs, mid+1, r);
    push_up(p);
}


int query(int p, int l, int r) {
    if (tr[p].l>=l && tr[p].r<=r)  return tr[p].cnt;
    push_down(p);
    int res = 0;
    if (tr[ls].r >= l)  res += query(ls, l, r);
    if (tr[rs].l <= r)  res += query(rs, l, r);
    return res;
}

void reverse(int p, int l, int r) {
    push_down(p);
    if (tr[p].l>=l && tr[p].r<=r) {
        tr[p].cnt = tr[p].r - tr[p].l + 1 - tr[p].cnt;
        tr[p].tag = true;
        return;
    }
    if (tr[ls].r >= l)  reverse(ls, l, r);
    if (tr[rs].l <= r)  reverse(rs, l, r);
    push_up(p);
}

int main() {
    initDebug;

    build(1, 1, N);

    while (M-->0) {
        int c=read(), a=read(), b=read();
        if (c==0) {
            reverse(1, a, b);
        } else {
            printf("%d\n", query(1, a, b));
        }
    }

    return 0;
}