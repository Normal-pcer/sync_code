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
#define rs (p<<1|1)
#define mid(a,b) ((a+b)>>1)

const int maxN = (int)1e5+5;

int N, M;
ll init[maxN];

struct SegTree
{    
    struct Node {
        int l, r;
        int sum;
        int tag = -1;  // 覆盖，-1 为无
    } t[maxN<<2];

    void push_up(int p) {
        t[p].sum = t[ls].sum + t[rs].sum;
    }

    void push_down(int p) {
        if (t[p].tag != -1) {
            t[ls].tag = t[rs].tag = t[p].tag;
            t[ls].sum = (t[ls].r - t[ls].l + 1) * t[p].tag;
            t[rs].sum = (t[rs].r - t[rs].l + 1) * t[p].tag;
            t[p].tag = -1;
        }
    }

    void build(int p, int l, int r) {
        t[p].l = l; t[p].r = r;
        if (l == r) {
            t[p].sum = init[l];
            return;
        }
        build(ls, l, mid(l, r));
        build(rs, mid(l, r) + 1, r);
        push_up(p);
    }

    void modify(int p, int l, int r, int val) {
        log("modify %d %d %d %d\n", p, l, r, val);
        if (t[p].l >= l && t[p].r <= r) {
            t[p].tag = val;
            t[p].sum = (t[p].r - t[p].l + 1) * val;
            return;
        }
        push_down(p);
        if (t[ls].r >= l) modify(ls, l, r, val);
        if (t[rs].l <= r) modify(rs, l, r, val);
        push_up(p);
    }

    int query(int p, int l, int r) {
        if (t[p].l >= l && t[p].r <= r) return t[p].sum;
        push_down(p);
        int res = 0;
        if (t[ls].r >= l) res += query(ls, l, r);
        if (t[rs].l <= r) res += query(rs, l, r);
        return res;
    }

} trees, grass;

int main() {
    initDebug;

    scanf("%d%d", &N, &M);
    upto(i, N)  init[i] = 1;

    int OP, x, y, k;
    trees.build(1, 1, N);
    grass.build(1, 1, N-1);
    upto(_, M) {
        scanf("%d", &OP);
        scanf("%d%d", &x, &y);
        if (OP==1) {
            // 清除 x+1 到 y-1 的所有树
            // 清除 x 到 y-1 的所有草
            if (x+1<=y-1)
            trees.modify(1, x+1, y-1, 0);
            grass.modify(1, x, y-1, 0);
        } else {
            // 清除 x 到 y 的所有树
            // 清除 x 到 y-1 的所有草
            trees.modify(1, x, y, 0);
            grass.modify(1, x, y-1, 0);
        }
    }

    // 查询树和草
    printf("%d %d\n", trees.query(1, 1, N), grass.query(1, 1, N-1));
    return 0;
}