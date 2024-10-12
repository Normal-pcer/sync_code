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

#define ls(i) (i<<1)
#define rs(i) (i<<1|1)

const int maxN = (int)1e5+5;

template <class T, const int sz>
class SegmentTree {
public:
    const std::function<void(int)> pushUpCallback;
    SegmentTree(std::function<void(int)> callback) pushUpCallback(callback) {}
    template <const int grp=1>
    struct Node {
        int l, r, len;
        T data[grp];
        T tag[grp];
    } tr[maxN<<2];
};

int N, M;
ll init[maxN];

struct Node {
        int l, r, len;
        ll sum=0;  // 区间内求和
        ll tag=0;  // 需要下传的更改
    } tr[maxN<<2];


void pushUp(int i) {  // 向上合并信息
    tr[i].sum = tr[ls(i)].sum + tr[rs(i)].sum;
}

void pushDown(int i) {  // 向下推送更改
    if (tr[i].tag){
        tr[ls(i)].sum += tr[i].tag * tr[ls(i)].len;
        tr[rs(i)].sum += tr[i].tag * tr[rs(i)].len;
        tr[ls(i)].tag += tr[i].tag;
        tr[rs(i)].tag += tr[i].tag;
        tr[i].tag = 0;
    }
}

void build(int p, int l, int r) {
    tr[p].l = l, tr[p].r = r, tr[p].len = r-l+1;
    if (l==r) { tr[p].sum = init[l]; return; }
    int m = (l+r) >> 1;
    build(ls(p), l, m);
    build(rs(p), m+1, r);
    pushUp(p);
}

ll sum(int p, int l, int r) {
    if (tr[p].l >= l && tr[p].r <= r)  return tr[p].sum;
    pushDown(p);
    ll res = 0;
    if (tr[ls(p)].r >= l)   res += sum(ls(p), l, r);
    if (tr[rs(p)].l <= r)   res += sum(rs(p), l, r);
    return res;
}

void add(int p, int l, int r, int val) {
    if (tr[p].l >= l && tr[p].r <= r)  { tr[p].sum += val * tr[p].len;tr[p].tag += val; return; }
    pushDown(p);
    if (tr[ls(p)].r >= l)   add(ls(p), l, r, val);
    if (tr[rs(p)].l <= r)   add(rs(p), l, r, val);
    pushUp(p);
}

int main() {
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

#undef rs
#undef ls