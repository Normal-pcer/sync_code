#include "lib"

#define ls (p<<1)
#define rs ((p<<1)|1)
#define avg(a,b) ((a+b)>>1)
#define merge(parent, child1, child2)   parent.sum = child1.sum + child2.sum; \
                                        parent.lc = child1.lc + ((child1.lc == child1.r-child1.l+1)? child2.lc : 0); \
                                        parent.tc = std::max({child1.tc, child2.tc, child1.rc + child2.lc}); \
                                        parent.rc = child2.rc + ((child2.rc == child2.r-child2.l+1)? child1.rc : 0);
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
        printf("[%d]{%2d, %2d, %2d, %2d, %2d} ", i, n.l, n.r, n.lc, n.tc, n.rc);
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
        tr[p].lc = tr[p].rc = tr[p].tc = init[l];
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
        tr[p].lc = tr[p].rc = tr[p].tc = val;
        return;
    }
    if (tr[ls].r>=pos) modify(ls,pos,val);
    else modify(rs,pos,val);
    pushUp(p);
}

Node query(int p, int l, int r) {
    log("query %d [%d, %d]\n", p, l, r);
    if (tr[p].l>=l && tr[p].r<=r)  return tr[p];
    Node res;
    res.l=l, res.r=r;
    bool empty=false;
    if (tr[ls].r >= l)  res = query(ls, l, r), empty = true;
    if (tr[rs].l <= r) {
        if (empty) {
            merge(res, res, query(rs, l, r));
        } else {
            res = query(rs, l, r);
        }
    }
    return res;
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