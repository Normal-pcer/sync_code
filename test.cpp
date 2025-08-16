#include<bits/stdc++.h>
#define _ 0
using namespace std;

namespace {
int n, m, q;
int cnt, cnt1, cnt2;
const int N = 5e4 + 10;
const int B = 300;
int pa[N], st[N][19], dfn[N], ans[N], p[N], id[N], d[N], sz[N], a[N], dis[N*B], lazy[B], mx[N], l[B], r[B], pos[N];
struct node {
	int u, v, w;
	bool operator < (const node &x) const {
		return w < x.w;
	}
} e[N*4];
struct que {
	int l, r, k, c, id;
	bool operator < (const que& x) const {
		return k < x.k;
	}
} q1[N], q2[N];
struct node2 {
	int mx, lazy;
} tr[N*4];
vector<pair<int, int>> g[N], g2[N];

inline void pushup(int p) {
	tr[p].mx = max(tr[p<<1].mx, tr[p<<1|1].mx);
}

inline void pushdown(int p) {
	tr[p<<1].mx = max(tr[p<<1].mx, tr[p].lazy);
	tr[p<<1|1].mx = max(tr[p<<1|1].mx, tr[p].lazy);
	tr[p<<1].lazy = max(tr[p<<1].lazy, tr[p].lazy);
	tr[p<<1|1].lazy = max(tr[p<<1|1].lazy, tr[p].lazy);
	tr[p].lazy = 0; 
}

void build(int p, int l, int r) {
	if (l == r) {
		tr[p].mx = d[l];
		return ;
	}
	int mid = (l + r) >> 1;
	build(p<<1, l, mid); 
	build(p<<1|1, mid+1, r);
	pushup(p);
}

void update(int p, int l, int r, int L, int R, int k) {
	if (L <= l && r <= R) {
		tr[p].mx = max(tr[p].mx, k);
		tr[p].lazy = max(tr[p].lazy, k);
		return ;
	} 
	int mid = (l + r) >> 1;
	if (tr[p].lazy) pushdown(p);
	if (L <= mid) update(p<<1, l, mid, L, R, k);
	if (R > mid) update(p<<1|1, mid+1, r, L, R, k);
	pushup(p);
}

int query(int p, int l, int r, int L, int R) {
	if (L <= l && r <= R) return tr[p].mx;
	int mid = (l + r) >> 1, res = 0;
	if (tr[p].lazy) pushdown(p);
	if (L <= mid) res = query(p<<1, l, mid, L, R);
	if (R > mid) res = max(res, query(p<<1|1, mid+1, r, L, R));
	return res;
}

int get_fa(int x) {
	return pa[x] == x ? x : pa[x] = get_fa(pa[x]);
} 

inline void dfs(int u, int fa) {
	dfn[u] = ++cnt;
	st[dfn[u]][0] = fa;
	sz[u] = 1;
	for (auto it : g[u]) {
		int v = it.first, w = it.second;
		if (v == fa) continue;
		a[v] = w;
		dfs(v, u);
		sz[u] += sz[v];
	}
}

inline int get(int u, int v) {
	return dfn[u]<dfn[v]?u:v;
}

inline int lca(int u, int v) {
	if (u == v) return u;
	u = dfn[u], v = dfn[v];
	if (u > v) swap(u, v);
	u++;
	int x = __lg(v-u+1);
	return get(st[u][x], st[v-(1<<x)+1][x]);
}

void update2(int L, int R, int k) {
	int p = pos[L], q = pos[R];
	if (p == q) {
		for (int i = L; i <= R; i++) mx[i] = max(mx[i], k);
		return ;
	}
	for (int i = L; i <= r[p]; i++) mx[i] = max(mx[i], k);
	for (int i = p+1; i < q; i++) lazy[i] = max(lazy[i], k);
	for (int i = l[q]; i <= R; i++) mx[i] = max(mx[i], k);
}

void dfs2(int u, int fa) {
	for (auto it : g[u]) {
		int v = it.first;
		if (v == fa) continue;
		dfs2(v, u);
	}
	for (auto it : g2[u]) {
		int v = it.first, id = it.second;
		if (id > 0) ans[id] = max({ans[id], mx[dfn[v]], lazy[pos[dfn[v]]]});
		else id *= -1, dis[id] = max({dis[id], mx[dfn[v]], lazy[pos[dfn[v]]]});
	}
	update2(dfn[u], dfn[u]+sz[u]-1, a[u]);
}

}

int main() {
	scanf("%d%d%d", &n, &m, &q);
	for (int i = 1; i <= n; i++) pa[i] = i;
	for (int i = 1; i <= m; i++) scanf("%d%d%d", &e[i].u, &e[i].v, &e[i].w);
	sort(e+1, e+m+1);
	for (int i = 1; i <= m; i++) {
		int u = get_fa(e[i].u);
		int v = get_fa(e[i].v);
		if (u != v) {
			pa[u] = v;
			g[e[i].u].push_back({e[i].v, e[i].w});
			g[e[i].v].push_back({e[i].u, e[i].w});
		}
	}
	dfs(1, 0);
	for (int j = 1; j <= 18; j++) {
		for (int i = 1; i+(1<<j)-1 <= n; i++) {
			st[i][j] = get(st[i][j-1], st[i+(1<<(j-1))][j-1]);
		}
	}
	const int B = sqrt(n);
	int len = n/B+!!n%B;
	for (int i = 1; i <= len; i++) {
		l[i] = r[i-1] + 1;
		r[i] = min(i*B, n);
	} 
	int j = 1;
	for (int i = 1; i <= n; i++) (i > r[j] ? ++j : j), pos[i] = j;
	for (int i = 1; i <= q; i++) {
		int l, r, k, c;
		scanf("%d%d%d%d", &l, &r, &k, &c);
		if (k <= B) {
			q1[++cnt1] = {l, r, k, c, i};
		} else {
			q2[++cnt2] = {l, r, k, c, i};
		}
	}
	sort(q1+1, q1+cnt1+1);
	for (int i = 1, t = 0, num = 0; i <= cnt1; i++) {
		if (t < q1[i].k) {
			t = q1[i].k;
			int cnt = 0;
			for (int j = 0; j < t; j++) {
				for (int k = !j?t:j; k <= n; k += t) p[++cnt] = k, id[k] = cnt;
			}
			for (int i = 2; i <= n; i++) {
				++num;
				int l = lca(p[i], p[i-1]);
				g2[l].push_back({p[i], -num});
				g2[l].push_back({p[i-1], -num});
			}
		}
	}
	for (int i = 1; i <= cnt2; i++) {
		int l = q2[i].l, r = q2[i].r, k = q2[i].k, c = q2[i].c;
		for (int u = l-l%k+c-k, v = 0; u <= r; u += k) {
			if (u < l) continue;
			if (v) {
				int p = lca(u, v);
				g2[p].push_back({u, q2[i].id});
				g2[p].push_back({v, q2[i].id}); 
			}
			v = u;
		}
	}
	dfs2(1, 0);
	for (int i = 1, t = 0, num = 0; i <= cnt1; i++) {
		if (t < q1[i].k) {
			t = q1[i].k;
			int cnt = 0;
			for (int j = 0; j < t; j++) {
				for (int k = !j?t:j; k <= n; k += t) p[++cnt] = k, id[k] = cnt;
			}
			for (int i = 2; i <= n; i++) d[i] = dis[++num];
			build(1, 1, n);
		}
		int l = q1[i].l, r = q1[i].r, k = q1[i].k, c = q1[i].c;
		l = l-l%k+c-k, r = r-r%k+c;
		while (l < q1[i].l) l += k;
		while (r > q1[i].r) r -= k;
		l = id[l], r = id[r];
		ans[q1[i].id] = query(1, 1, n, l+1, r);
	}
	for (int i = 1; i <= q; i++) printf("%d\n", ans[i]);
	return ~~(0^_^0);
}
