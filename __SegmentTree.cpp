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

const int maxN = (int)1e5+5;

template <
    const int sz,
    class dat_t=int, 
    class tag_t=dat_t
>
class SegmentTree {
public:
    int ls(int p) { return p<<1; }
    int rs(int p) { return p<<1|1; }
    struct Node {
        int l, r;
        dat_t data;
        tag_t tag;
    } tr[sz];
    const std::function<void(Node&,Node&,Node&)> pushUpCallback;
    const std::function<void(Node&,Node&)> pushDownCallback;
    const dat_t emptyData;
    const tag_t emptyTag;
    template <class T, class F1, class F2>
    SegmentTree(
        F1&& upCallback,
        F2&& downCallback,
        dat_t&& emptyData = 0,
        tag_t&& emptyTag = 0
    ): pushUpCallback(upCallback), pushDownCallback(downCallback), 
        emptyData(emptyData), emptyTag(emptyTag)
    {
        build(1, 0, sz-1);
    }

    void pushUp(int p) {
        pushUpCallback(tr[p], tr[ls(p)], tr[rs(p)]);
    }
    void pushDown(int p) {
        pushDownCallback(tr[p], tr[ls(p)]);
        pushDownCallback(tr[p], tr[rs(p)]);
    }

    template <class initializer_t>
    void build(int p, int l, int r, initializer_t&& initializer) {
        tr[p].l = l, tr[p].r = r;
        if (l==r) {
            tr[p].data = initializer(l);
            return;
        }
        int m = (l+r) >> 1;
        build((p<<1), l, m);
        build((p<<1|1), m+1, r);
    }

    void build(int p, int l, int r) {
        tr[p].l = l, tr[p].r = r;
        if (l==r) {
            return;
        }
        int m = (l+r) >> 1;
        build((p<<1), l, m);
        build((p<<1|1), m+1, r);
    }

    dat_t query(int p, int l, int r) {
        if (tr[p].l >= l && tr[p].r <= r)  return tr[p].data;
        pushDown(p);
        dat_t res = emptyData;
        if (tr[ls(p)].r >= l)   res = pushUpCallback(res, res, query(ls(p), l, r));
        if (tr[rs(p)].l <= r)   res = pushUpCallback(res, res, query(rs(p), l, r));
        return res;
    }
};

int N, M;
ll init[maxN];

struct Node {
        int l, r, len;
        ll sum=0;  // 区间内求和
        ll tag=0;  // 需要下传的更改
    } tr[maxN<<2];

void pushUp(int i) {  // 向上合并信息
    tr[i].sum = tr[(i<<1)].sum + tr[(i<<1|1)].sum;
}

void pushDown(int i) {  // 向下推送更改
    if (tr[i].tag){
        tr[(i<<1)].sum += tr[i].tag * tr[(i<<1)].len;
        tr[(i<<1|1)].sum += tr[i].tag * tr[(i<<1|1)].len;
        tr[(i<<1)].tag += tr[i].tag;
        tr[(i<<1|1)].tag += tr[i].tag;
        tr[i].tag = 0;
    }
}

void build(int p, int l, int r) {
    tr[p].l = l, tr[p].r = r, tr[p].len = r-l+1;
    if (l==r) { tr[p].sum = init[l]; return; }
    int m = (l+r) >> 1;
    build((p<<1), l, m);
    build((p<<1|1), m+1, r);
    pushUp(p);
}

ll sum(int p, int l, int r) {
    if (tr[p].l >= l && tr[p].r <= r)  return tr[p].sum;
    pushDown(p);
    ll res = 0;
    if (tr[(p<<1)].r >= l)   res += sum((p<<1), l, r);
    if (tr[(p<<1|1)].l <= r)   res += sum((p<<1|1), l, r);
    return res;
}

void add(int p, int l, int r, int val) {
    if (tr[p].l >= l && tr[p].r <= r)  { tr[p].sum += val * tr[p].len;tr[p].tag += val; return; }
    pushDown(p);
    if (tr[(p<<1)].r >= l)   add((p<<1), l, r, val);
    if (tr[(p<<1|1)].l <= r)   add((p<<1|1), l, r, val);
    pushUp(p);
}

int main(int argc, char const *argv[]) {
    initDebug;

    scanf("%d%d", &N, &M);
    upto(i, N)  scanf("%lld", init+i);

    build(1, 1, N);

    int OP, x, y, k;
    upto(_, M) {
        scanf("%d", &OP);
        if (OP == 1) {  // 加k
            scanf("%d%d%d", &x, &y, &k);
            add(1, x, y, k);
        } else {  // 查询
            scanf("%d%d", &x, &y);
            printf("%lld\n", sum(1, x, y));
        }
    }
    return 0;
}
