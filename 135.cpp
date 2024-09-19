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

#define ls (p<<1)
#define rs ((p<<1)|1)
#define avg(a,b) ((a+b)>>1)
#define merge(parent, child1, child2)   parent.sum = child1.sum + child2.sum; \
                                        parent.lc = std::max(child1.lc, child2.lc+child1.sum); \
                                        parent.tc = std::max({child1.tc, child2.tc, child1.rc + child2.lc}); \
                                        parent.rc = std::max(child2.rc, child1.rc+child2.sum);
const int maxN = (int)5e5+5;
const int Infinity = 2147483647;
int N, M;
int init[maxN];

struct Node {
    int l, r;           // 左端点，右端点
    int lc, tc=-Infinity, rc;     // 左起、任意和右起最大子段和
    int sum;
} tr[4*maxN];

void output(int l, int r) {
    from(i, l, r) {
        Node &n=tr[i];
        printf("[%d]{%2d, %2d, %2d, %2d, %2d, %2d} ", i, n.l, n.r, n.lc, n.tc, n.rc, n.sum);
    }
    printf("\n");
}

void pushUp(int p) {
    log("pushUp %d\n", p)
    merge(tr[p], tr[ls], tr[rs]);
}

void build(int p, int l, int r) {
    log("build %d [%d, %d]\n", p, l, r);
    tr[p].l = l; tr[p].r = r;
    if (l==r) {
        tr[p].lc = tr[p].rc = tr[p].tc = tr[p].sum = init[l];
        return;
    }
    int m = avg(l, r);
    build(ls, l, m);
    build(rs, m+1, r);
    pushUp(p);
}

void modify(int p, int pos, int val) {
    log("modify %d %d %d\n", p, pos, val);
    if (tr[p].l==pos && tr[p].r==pos) {
        tr[p].lc = tr[p].rc = tr[p].tc = tr[p].sum = val;
        return;
    }
    if (tr[ls].r>=pos) modify(ls,pos,val);
    else modify(rs,pos,val);
    pushUp(p);
}

Node query(int p, int l, int r) {
    log("query %d [%d, %d]\n", p, l, r);
    if (tr[p].l>=l && tr[p].r<=r)  return tr[p];
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
    optimizeIO;
    std::cin >> N >> M;
    upto(i, N) { 
        std::cin >> init[i];
    }

    build(1, 1, N);

    int OP, A, B;
    upto(i, M) {
        std::cin >> OP >> A >> B;
        if (OP==1) {
            std::cout << query( 1, std::min(A, B), std::max(A, B) ).tc << std::endl;
        } else if (OP==0) {
            output(A, B);
        } else {
            modify(1, A, B);
        }
    }
    return 0;
}