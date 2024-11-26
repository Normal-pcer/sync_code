#include <bits/stdc++.h>
using namespace std;

#define int long long

const int maxn = 3e5 + 7;

// 考虑 a = p 时 b 是确定的
// 同时 c 应当在 a, b 链所挂的子树上
// a 或 b 应当在对方的子树里

// 考虑以深度为轴构造前缀和

// 进一步考虑同一子树内 DFN 连续

// 需要求解 dfn, dep 在一定范围内的计数

// 考虑主席树在构造时维护 dep 的权值树
// 再以 节点 构造根

// 看错题了
// 考虑 dfn 树维护他之前各深度的 子树 总数
// 那么 可直接从 dfn-1 继承并只更改一次
// 询问时树上差分求 树 dfn, dfm 之间 序号 dep+1, dep+k 之间的和
// 也可以直接求
// 点为 dfn(B), 值为 C

int n, q;

vector<int> es[maxn];

struct no {
    int fa;
    int dep, dfn, dfm;
    int siz;
} S[maxn];

int dfcnt;
int depes;

namespace HJT {
struct tt {
    int ls, rs;
    int sum;
} t[maxn * 30];
int rt[maxn], cnt;
int cntt;

int ne(int tt = 0) {
    t[++cntt] = t[tt];
    return cntt;
}

int build(int u, int l, int r) {
    if (l == r) {
        return u;
    }
    int mid = (l + r) >> 1;
    t[u].ls = build(++cntt, l, mid);
    t[u].rs = build(++cntt, mid + 1, r);
    return u;
}

void add(int u, int v, int l, int r, int dep, int siz) {
    if (l == r && r == dep) {
        t[u].sum = t[v].sum + siz;
        return ;
    }
    int mid = (l + r) >> 1;
    if (dep <= mid) {
        t[u].ls = ne(t[v].ls);
        add(t[u].ls, t[v].ls, l, mid, dep, siz);
    } else {
        t[u].rs = ne(t[v].rs);
        add(t[u].rs, t[v].rs, mid + 1, r, dep, siz);
    }
    t[u].sum = t[t[u].ls].sum + t[t[u].rs].sum;
}

int query(int u, int v, int l, int r, int x, int y) {
    if (x <= l && r <= y) {
        return t[u].sum - t[v].sum;
    }
    int mid = (l + r) >> 1;
    int res = 0;
    if (x <= mid) {
        res += query(t[u].ls, t[v].ls, l, mid, x, y);
    }
    if (mid + 1 <= y) {
        res += query(t[u].rs, t[v].rs, mid + 1, r, x, y);
    }
    return res;
}
} // HJT

void dfsn(int u, int fa) {
    S[u] = (no){fa, S[fa].dep + 1, ++dfcnt, 0, 1};
    depes = max(depes, S[u].dep);
    for(auto v : es[u]) {
        if (fa == v) continue;
        dfsn(v, u);
        S[u].siz += S[v].siz;
    }
    S[u].dfm = dfcnt;
}

void bu(int u, int fa) {
    // cerr << u << endl;
    HJT::rt[S[u].dfn] = HJT::ne(S[u].dfn - 1);
    HJT::add(HJT::rt[S[u].dfn], HJT::rt[S[u].dfn - 1], 1, n, S[u].dep, S[u].siz - 1);
    for(auto v : es[u]) {
        if (fa == v) continue;
        bu(v, u);
    }
    // cerr << u << " " << S[u].siz << endl;
}

signed main() {
    ios::sync_with_stdio(false);

    cin >> n >> q;
    for(int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        es[u].emplace_back(v);
        es[v].emplace_back(u);
    }
    dfsn(1, 1);
    HJT::build(HJT::rt[0] = 0, 1, n);
    bu(1, 1);

    for(int i = 1, p, k; i <= q; i++) {
        cin >> p >> k;
        int ans = min(k, S[p].dep - 1) * (S[p].siz - 1);
        ans += HJT::query(HJT::rt[S[p].dfm], HJT::rt[S[p].dfn - 1], 1, n, S[p].dep + 1, S[p].dep + k);
        cout << ans << endl;
    }

    return 0;
}