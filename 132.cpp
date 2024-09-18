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
#define prt(i) (i>>1)
#define mid(a,b) ((a+b)>>1)

const int maxN = (int)1e5+5;

int N, M, mod = 571373;
ll init[maxN];

struct Node {
    int l, r, len;
    ll sum=0;  // 区间内求和
    ll add=0;  // 需要下传的更改
    ll mul=1;  // 乘法更改
} tr[maxN<<2];

void output(int l, int r) {
    always return;
    from(i, l, r) {
        Node &n=tr[i];
        printf("[%d]{%2d, %2d, %2d, %2lld, %2lld, %2lld} ", i, n.l, n.r, n.len, n.sum, n.add, n.mul);
    }
    printf("\n");
}

void pushUp(int i) {  // 向上合并信息
    tr[i].sum = (tr[lc(i)].sum + tr[rc(i)].sum) % mod;
}

void pushDown(int i) {  // 向下推送更改
    if (tr[i].add || tr[i].mul != 1){
        log("Push Down: %d\n", i)
        tr[lc(i)].sum = (tr[i].add * tr[lc(i)].len + tr[i].mul * tr[lc(i)].sum) % mod;
        tr[rc(i)].sum = (tr[i].add * tr[rc(i)].len + tr[i].mul * tr[rc(i)].sum) % mod;
        tr[lc(i)].mul = (tr[lc(i)].mul * tr[i].mul) % mod;
        tr[rc(i)].mul = (tr[rc(i)].mul * tr[i].mul) % mod;
        tr[rc(i)].add = (tr[rc(i)].add * tr[i].mul + tr[i].add) % mod; //! 乘tr[i].mul
        tr[lc(i)].add = (tr[lc(i)].add * tr[i].mul + tr[i].add) % mod;
        tr[i].add = 0;
        tr[i].mul = 1;
        debug output(1, 4*N);
    }
}

void build(int p, int l, int r) {
    log("Build: %d, [%d, %d]\n", p, l, r)
    tr[p].l = l, tr[p].r = r, tr[p].len = r-l+1;
    if (l==r) { tr[p].sum = init[l] % mod; return; }
    int m = mid(l, r);
    build(lc(p), l, m);
    build(rc(p), m+1, r);
    pushUp(p);
}

ll sum(int p, int l, int r) { // !返回值开long long
    log("Sum: %d, [%d, %d]\n", p, l, r)
    if (tr[p].l >= l && tr[p].r <= r)  return tr[p].sum;
    pushDown(p);
    ll res = 0;
    if (tr[lc(p)].r >= l)   res += sum(lc(p), l, r);
    if (tr[rc(p)].l <= r)   res += sum(rc(p), l, r);
    return res % mod;
}

void add(int p, int l, int r, long long val) { // !val 开long long
    log("Add: %d, [%d, %d], %lld\n", p, l, r, val);
    if (tr[p].l >= l && tr[p].r <= r)  { tr[p].sum += val * tr[p].len % mod; tr[p].add = (tr[p].add + val) % mod; return; }
    pushDown(p);
    if (tr[lc(p)].r >= l)   add(lc(p), l, r, val);
    if (tr[rc(p)].l <= r)   add(rc(p), l, r, val);
    pushUp(p);
}

void mul(int p, int l, int r, long long val) { // !val 开long long
    log("Mul: %d, [%d, %d], %lld\n", p, l, r, val);
    if (tr[p].l >= l && tr[p].r <= r)  {
        tr[p].sum = (tr[p].sum * val) % mod;  // !取模
        tr[p].add = (tr[p].add * val) % mod;
        tr[p].mul = (tr[p].mul * val) % mod;
        return; 
    }
    pushDown(p);
    if (tr[lc(p)].r >= l)   mul(lc(p), l, r, val);
    if (tr[rc(p)].l <= r)   mul(rc(p), l, r, val);
    pushUp(p);
}

int main() {
    initDebug;

    scanf("%d%d", &N, &mod);
    upto(i, N)  scanf("%lld", init+i);
    scanf("%d", &M);
    build(1, 1, N);

    int OP, x, y; long long k; //! k 开 long long
    upto(_, M) {
        scanf("%d", &OP);
        if (OP == 2) {  // 加k
            scanf("%d%d%lld", &x, &y, &k);
            add(1, x, y, k);
            debug output(1, 4*N);
        } else if (OP == 1) { // 乘
            scanf("%d%d%lld", &x, &y, &k);
            mul(1, x, y, k);
            debug output(1, 4*N);
        } else {  // 查询
            scanf("%d%d", &x, &y);
            printf("%lld\n", sum(1, x, y) % mod);
        }
    }
    return 0;
}