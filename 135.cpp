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

#define ls ((p)<<1)
#define rs (((p)<<1)|1)
#define avg(a,b) (((a)+(b))>>1)
#define merge(parent, child1, child2)   parent.l1 = std::max(child1.l1, (child1.l1==child1.len? child2.l1+child1.len: 0)); \
                                        parent.t1 = std::max({child1.t1, child2.t1, child1.r1 + child2.l1}); \
                                        parent.r1 = std::max(child2.r1, (child2.r1==child2.len? child1.r1+child2.len: 0)); \
                                        parent.l0 = std::max(child1.l0, (child1.l0==child1.len? child2.l0+child1.len: 0)); \
                                        parent.t0 = std::max({child1.t0, child2.t0, child1.r0 + child2.l0}); \
                                        parent.r0 = std::max(child2.r0, (child2.r0==child2.len? child1.r0+child2.len: 0));
#define n1 tr[p].l1 = tr[p].r1 = tr[p].t1
#define n0 tr[p].l0 = tr[p].r0 = tr[p].t0
const int maxN = 5e5+5;
const int Infinity = 2147483647;
int N, M;
int init[maxN];

struct Node {
    int l, r, len;           // 左端点，右端点和长度
    int l1, t1, r1;     // 左起、任意和右起最大全1子段长度
    int l0, t0, r0;     // 左起、任意和右起最大全0子段长度
    bool tag;   // lazyTag，延迟进行取反操作
} tr[4*maxN];

void output(int l, int r) {
    from(i, l, r) {
        Node &n=tr[i];
        printf("[%d]{%2d, %2d, %2d, %2d, %2d, %2d, %2d, %2d, %c} ", i, n.l, n.r, n.l1, n.t1, n.r1, n.l0, n.t0, n.r0, n.tag?'T':'F');
    }
    printf("\n");
}

void pushUp(int p) {
    log("pushUp %d\n", p)
    merge(tr[p], tr[ls], tr[rs]);
}

void pushDown(int p) {
    if (tr[p].tag) {
        tr[ls].tag ^= true;
        tr[rs].tag ^= true;
        std::swap(tr[ls].l1, tr[ls].l0);
        std::swap(tr[ls].t1, tr[ls].t0);
        std::swap(tr[ls].r1, tr[ls].r0);
        std::swap(tr[rs].l1, tr[rs].l0);
        std::swap(tr[rs].t1, tr[rs].t0);
        std::swap(tr[rs].r1, tr[rs].r0);
        tr[p].tag = false;
    }
}

void build(int p, int l, int r) {
    log("build %d [%d, %d]\n", p, l, r);
    tr[p].l = l; tr[p].r = r; tr[p].len = r-l+1;
    if (l==r) {
        if (init[l]) { n1=1; n0=0; }
        else { n1=0; n0=1; }
        return;
    }
    int m = avg(l, r);
    build(ls, l, m);
    build(rs, m+1, r);
    pushUp(p);
}


void negate(int p, int l, int r) {
    log("Negate %d, [%d, %d]\n", p, l, r)
    if (tr[p].l>=l && tr[p].r<=r) {
        std::swap(tr[p].l1, tr[p].l0);
        std::swap(tr[p].t1, tr[p].t0);
        std::swap(tr[p].r1, tr[p].r0);
        tr[p].tag ^= true;
        return;
    }
    pushDown(p);
    if (tr[ls].r >= l)  negate(ls, l, r);
    if (tr[rs].l <= r)  negate(rs, l, r);
    pushUp(p);
}

Node query(int p, int l, int r) {
    log("query %d [%d, %d]\n", p, l, r);
    if (tr[p].l>=l && tr[p].r<=r)  return tr[p];
    pushDown(p);
    int m = avg(tr[p].l, tr[p].r);
    if (r<=m)   return query(ls, l, r);
    else if (l>m) return query(rs, l, r);
    else {
        Node res, a=query(ls, l, m), b=query(rs, m+1, r);
        merge(res, a, b);
        return res;
    }
}

int main() {
    initDebug;
    // optimizeIO;
    // printf("%d", true ^ false);
    // std::cin >> N >> M;

    never {
        freopen("135.in", "r", stdin);
        freopen("135.out", "w", stdout);
    }
    scanf("%d%d", &N, &M);
    upto(i, N) { 
        // std::cin >> init[i];
        scanf("%1d", init+i);
    }

    build(1, 1, N);

    int OP, A, B;
    upto(i, M) {
        std::cin >> OP >> A >> B;
        if (OP==1) {
            // std::cout << query( 1, std::min(A, B), std::max(A, B) ).t1 << std::endl;
            negate(1, A, B);
        } else if (OP==0) {
            output(A, B);
        } else {
            // modify(1, A, B);
            std::cout << query( 1, A, B ).t1 << std::endl;
        }
    }
    return 0;
}