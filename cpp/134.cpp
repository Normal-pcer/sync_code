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

#define lc(p) (p<<1)
#define rc(p) ((p<<1)|1)
#define mid(a,b) ((a+b)>>1)

const int maxN = 100005;
int init[maxN];
int d[maxN];
int N, M;
struct Node {
    int l, r, len;
    ll sum, add;
} tr[maxN*4];

void pushUp(int p) {
    tr[p].sum = tr[lc(p)].sum + tr[rc(p)].sum;
}

void pushDown(int p) {
    if (tr[p].add) {
        tr[lc(p)].add += tr[p].add;
        tr[rc(p)].add += tr[p].add;
        tr[lc(p)].sum += tr[p].add * tr[lc(p)].len;
        tr[rc(p)].sum += tr[p].add * tr[rc(p)].len;
        tr[p].add = 0;
    }
}

void build(int p, int l, int r) {
    tr[p].l = l; tr[p].r = r; tr[p].len = r - l + 1;
    if (l==r) {
        tr[p].sum = d[l];
        return;
    }
    int m = mid(l, r);
    build(lc(p), l, m);
    build(rc(p), m+1, r);
    pushUp(p);
}

ll query(int p, int l, int r) {
    if (tr[p].l>=l && tr[p].r<=r)   return tr[p].sum;
    pushDown(p);
    ll res = 0;
    if (tr[lc(p)].r >= l)  res += query(lc(p), l, r);
    if (tr[rc(p)].l <= r)  res += query(rc(p), l, r);
    return res;
}

void add(int p, int l, int r, ll val) {
    if (tr[p].l>=l && tr[p].r<=r) {
        tr[p].add += val;
        tr[p].sum += val * tr[p].len;
        return;
    }
    pushDown(p);
    if (tr[lc(p)].r >= l)  add(lc(p), l, r, val);
    if (tr[rc(p)].l <= r)  add(rc(p), l, r, val);
    pushUp(p);
}

int main() {
    initDebug;
    scanf("%d%d", &N, &M);
    upto(i, N) {
        scanf("%d", init+i);
        d[i] = init[i] - init[i-1];
    }
    debug upto(i, N) log("%3d", init[i])
    log("\n")

    build(1, 1, N);

    int OP, L, R, K, D;
    upto(i, M) {
        scanf("%d%d", &OP, &L);
        if (OP == 2) {
            printf("%lld\n", query(1, 1, L));
        } else {
            scanf("%d%d%d", &R, &K, &D);
            add(1, L, L, K);
            if (L<N)  add(1, L+1, R, D);
            if (R<N)  add(1, R+1, R+1, -K-D*(R-L));
        }
    }
}