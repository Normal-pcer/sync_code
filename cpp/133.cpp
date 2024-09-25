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
typedef long long ll; typedef unsigned long long ull; typedef __int128 lll;
inline void batchOutput(int **begin, int rows, int cols, const char *format){upto(i, rows){upto(j, cols)printf(format, begin[i][j]);printf("\n");}} inline void batchOutput(int**begin, int rows, int cols) {batchOutput(begin,rows,cols,"%3d");}
template <class T=int>inline T read() { T x=0;int f=1;char c;while((c=getchar())<'0'||c>'9')if(c=='-')f=-1;do{x=(((x<<2)+x)<<1)+c-'0';}while((c=getchar())>='0'&&c<='9');return x*f; }

#define lc(i) (i<<1)
#define rc(i) (i<<1|1)
#define mid(a,b) ((a+b)>>1)

const int maxN = 1000005;
const int Infinity = 0x7fffffff;
int init[maxN];
int N, Q;

struct Node {
    int l, r, len; ll max, add=0, let=Infinity;
} tr[maxN<<2];

void output(int l, int r) {
    from(i, l, r) {
        Node &n=tr[i];
        printf("[%d]{%2d, %2d, %2d, %2lld, %2lld, %2lld} ", i, n.l, n.r, n.len, n.max, n.add, n.let);
    }
    printf("\n");
}

void pushUp(int p) {
    tr[p].max = std::max(tr[lc(p)].max, tr[rc(p)].max);
}

void pushDown(int p) {
    if (tr[p].let != Infinity) {
        tr[lc(p)].let = tr[p].let;
        tr[rc(p)].let = tr[p].let;
        tr[lc(p)].max = tr[p].let;
        tr[rc(p)].max = tr[p].let;
        tr[lc(p)].add = 0;
        tr[rc(p)].add = 0;
        tr[p].let = Infinity;
    }
    if (tr[p].add) {
        tr[lc(p)].add += tr[p].add;
        tr[rc(p)].add += tr[p].add;
        tr[lc(p)].max += tr[p].add;
        tr[rc(p)].max += tr[p].add;
        tr[p].add = 0;
    }
}

void build(int p, int l, int r) {
    tr[p].l = l, tr[p].r = r, tr[p].len = r - l + 1;
    if (l==r) {
        tr[p].max = init[l];
        return;
    }
    int m = mid(l, r);
    build(lc(p), l, m);
    build(rc(p), m+1, r);
    pushUp(p);
}

ll query(int p, int l, int r) {
    if (tr[p].l>=l && tr[p].r<=r)  return tr[p].max;
    pushDown(p);
    ll res = -0x7fffffffffffffff;
    if (tr[lc(p)].r >= l)  chkMax(res, query(lc(p), l, r) );
    if (tr[rc(p)].l <= r)  chkMax(res, query(rc(p), l, r) );
    return res;
}

void add(int p, int l, int r, ll val) {
    if (tr[p].l>=l && tr[p].r<=r) {
        tr[p].add += val;
        tr[p].max += val;
        return;
    }
    pushDown(p);
    if (tr[lc(p)].r >= l)  add(lc(p), l, r, val);
    if (tr[rc(p)].l <= r)  add(rc(p), l, r, val);
    pushUp(p);
}

void let(int p, int l, int r, ll val) {
    if (tr[p].l>=l && tr[p].r<=r) {
        tr[p].let = val;
        tr[p].add = 0;
        tr[p].max = val;
        return;
    }
    pushDown(p);
    if (tr[lc(p)].r >= l)   let(lc(p), l, r, val);
    if (tr[rc(p)].l <= r)   let(rc(p), l, r, val);
    pushUp(p);
}

int main() {
    initDebug;
    scanf("%d%d", &N, &Q);
    upto(i, N) {
        scanf("%d", init+i);
    }

    build(1, 1, N);

    int OP, L, R, X;
    upto(_, Q) {
        scanf("%d%d%d", &OP, &L, &R);
        if (OP==1) {  // let
            scanf("%d", &X);
            let(1, L, R, X);
        } else if (OP==2) {
            scanf("%d", &X);
            add(1, L, R, X);
        } else {
            printf("%lld\n", query(1, L, R));
        }
        never output(1, N*3);
    }
    return 0;
}