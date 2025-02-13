#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int MAXN = 2e5 + 10;
const int MAXM = 6e5 + 10;
const ll inf = ~0ull >> 1;

struct node {
	int u, v;
	ll w;
	bool operator < (const node &rhs) const { return w < rhs.w; }
} e[MAXM];

int fa[MAXN], tot;

int find(int k) {
	return k == fa[k] ? k : fa[k] = find(fa[k]);
}

inline 
bool merge(int x, int y) {
	return (x = find(x)) ^ (y = find(y)) ? fa[x] = y : 0;
}

int n, m, s1, s2;

inline 
ll kruskal(bool x, bool y) {
	ll res = 0, cnt = 0;
	for (int i = 1; i <= n + 2; i++) fa[i] = i;
	for (int i = 1; i <= tot; i++) {
		if (x && (e[i].u == s1 || e[i].v == s1)) continue;
		if (y && (e[i].u == s2 || e[i].v == s2)) continue;
		if (merge(e[i].u, e[i].v)) res += e[i].w, cnt++;
	}
	return cnt == n + 1 - x - y ? res : inf;
}

int main() {
	scanf("%d%d", &n, &m), s1 = n + 1, s2 = n + 2;
	for (int i = 1, x; i <= n; i++) scanf("%d", &x), e[++tot] = { s1, i, x };
	for (int i = 1, x; i <= n; i++) scanf("%d", &x), e[++tot] = { s2, i, x };
	for (int i = 1, u, v, w; i <= m; i++) scanf("%d%d%d", &u, &v, &w), e[++tot] = { u, v, w };
	sort(e + 1, e + tot + 1);
	printf("%lld", min({ kruskal(0, 0), kruskal(1, 0), kruskal(0, 1), kruskal(1, 1) }));
}