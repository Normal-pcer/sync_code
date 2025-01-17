#include<bits/stdc++.h>
#define int int_fast64_t
#define lson(root) (root << 1)
#define rson(root) (root << 1 | 1)
using namespace std;
struct node
{
	int to , nxt , w;
}e[400010];
struct edge
{
	int sum , mn , mx , lazy; // lazy = 0 --> \times 1  else --> \times -1 
}tree[800010];
struct dick
{
	int u , v;
}ee[200010];
int n , m , a[200010] , fa[200010] , son[200010] , sz[200010] , dep[200010] , dfn[200010] , rk[200010] , tp[200010] , cnt , tot , head[200010];
void add(int u , int v , int w)
{
	++ tot;
	e[tot].to = v;
	e[tot].nxt = head[u];
	e[tot].w = w;
	head[u] = tot;
}
void dfs1(int u , int pa)
{
	sz[u] = 1 , fa[u] = pa , dep[u] = dep[pa] + 1;
	for(int i = head[u] ; i != 0 ; i = e[i].nxt)
	{
		int v = e[i].to;
		if(v == pa) continue;
		dfs1(v , u);
		a[v] = e[i].w;
		sz[u] += sz[v];
		if(sz[son[u]] < sz[v]) son[u] = v;
	}
}
void dfs2(int u , int tp_fa)
{
	dfn[u] = ++ cnt , rk[cnt] = u , tp[u] = tp_fa;
	if(son[u]) dfs2(son[u] , tp_fa);
	for(int i = head[u] ; i != 0 ; i = e[i].nxt)
	{
		int v = e[i].to;
		if(v == fa[u] || v == son[u]) continue;
		dfs2(v , v);
	}
}
void pushup(int root)
{
	tree[root].sum = tree[lson(root)].sum + tree[rson(root)].sum;
	tree[root].mn = min(tree[lson(root)].mn , tree[rson(root)].mn);
	tree[root].mx = max(tree[lson(root)].mx , tree[rson(root)].mx);
}
void pushdown(int root , int l , int r)
{
	int mid = (l + r) >> 1;
	if(tree[root].lazy == 0) return;
	tree[lson(root)].sum = -tree[lson(root)].sum , tree[rson(root)].sum = -tree[rson(root)].sum;
	int x , y;
	x = tree[lson(root)].mn , y = tree[lson(root)].mx , tree[lson(root)].mx = -x , tree[lson(root)].mn = -y;
	x = tree[rson(root)].mn , y = tree[rson(root)].mx , tree[rson(root)].mx = -x , tree[rson(root)].mn = -y;
	tree[lson(root)].lazy ^= tree[root].lazy , tree[rson(root)].lazy ^= tree[root].lazy;
	tree[root].lazy = 0;
}
void update1(int root , int l , int r , int pos , int val)
{
	if(l == r) return tree[root].sum = tree[root].mn = tree[root].mx = val , void();
	int mid = (l + r) >> 1;
	pushdown(root , l , r);
	if(pos <= mid) update1(lson(root) , l , mid , pos , val);
	else update1(rson(root) , mid + 1 , r , pos , val);
	pushup(root);
}
void update2(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r)
	{
		tree[root].sum = -tree[root].sum , tree[root].lazy ^= 1;
		int x = tree[root].mn , y = tree[root].mx; tree[root].mx = -x , tree[root].mn = -y;
		return;
	}
	int mid = (l + r) >> 1;
	pushdown(root , l , r);
	if(L <= mid) update2(lson(root) , l , mid , L , R);
	if(R > mid) update2(rson(root) , mid + 1 , r , L , R);
	pushup(root);
}
int query_sum(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r) return tree[root].sum;
	int mid = (l + r) >> 1 , ans = 0;
	pushdown(root , l , r);
	if(L <= mid) ans += query_sum(lson(root) , l , mid , L , R);
	if(R > mid) ans += query_sum(rson(root) , mid + 1 , r , L , R);
	return ans;
}
int query_min(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r) return tree[root].mn;
	int mid = (l + r) >> 1 , ans = 9e18;
	pushdown(root , l , r);
	if(L <= mid) ans = min(ans , query_min(lson(root) , l , mid , L , R));
	if(R > mid) ans = min(ans , query_min(rson(root) , mid + 1 , r , L , R));
	return ans;
}
int query_max(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r) return tree[root].mx;
	int mid = (l + r) >> 1 , ans = -9e18;
	pushdown(root , l , r);
	if(L <= mid) ans = max(ans , query_max(lson(root) , l , mid , L , R));
	if(R > mid) ans = max(ans , query_max(rson(root) , mid + 1 , r , L , R));
	return ans;
}
void build(int root , int l , int r)
{
	if(l == r)
	{
		tree[root].sum = tree[root].mn = tree[root].mx = a[rk[l]];
		return;
	}
	int mid = (l + r) >> 1;
	build(lson(root) , l , mid);
	build(rson(root) , mid + 1 , r);
	pushup(root);
}
void uv_update(int u , int v)
{
	if (u==v) return;
	while(tp[u] != tp[v])
	{
		if(dep[tp[u]] < dep[tp[v]]) swap(u , v);
		update2(1 , 1 , n , dfn[tp[u]] , dfn[u]);
		u = fa[tp[u]];
	}
	if(dep[u] < dep[v]) swap(u , v);
	update2(1 , 1 , n , dfn[v] +1, dfn[u]);
}
int uv_query_sum(int u , int v)
{
	if (u==v) return 0;
	int ans = 0;
	while(tp[u] != tp[v])
	{
		if(dep[tp[u]] < dep[tp[v]]) swap(u , v);
		ans += query_sum(1 , 1 , n , dfn[tp[u]] , dfn[u]);
		u = fa[tp[u]];
	}
	if(dep[u] < dep[v]) swap(u , v);
	ans += query_sum(1 , 1 , n , dfn[v] +1, dfn[u]);
	return ans;
}
int uv_query_max(int u , int v)
{
	if (u==v) return 0;
	int ans = -9e18;
	while(tp[u] != tp[v])
	{
		if(dep[tp[u]] < dep[tp[v]]) swap(u , v);
		ans = max(ans , query_max(1 , 1 , n , dfn[tp[u]] , dfn[u]));
		u = fa[tp[u]];
	}
	if(dep[u] < dep[v]) swap(u , v);
	ans = max(ans , query_max(1 , 1 , n , dfn[v] +1, dfn[u]));
	return ans;
}
int uv_query_min(int u , int v)
{
	if (u==v) return 0;
	int ans = 9e18;
	while(tp[u] != tp[v])
	{
		if(dep[tp[u]] < dep[tp[v]]) swap(u , v);
		ans = min(ans , query_min(1 , 1 , n , dfn[tp[u]] , dfn[u]));
		u = fa[tp[u]];
	}
	if(dep[u] < dep[v]) swap(u , v);
	ans = min(ans , query_min(1 , 1 , n , dfn[v]+1 , dfn[u]));
	return ans;
}
signed main()
{
	scanf("%lld" , &n);
	for(int i = 1 , u , v , w ; i < n ; i ++) scanf("%lld%lld%lld" , &u , &v , &w) , u ++ , v ++ , add(u , v , w) , add(v , u , w) , ee[i] = (dick){u , v};
	dfs1(1 , 0) , dfs2(1 , 1);
	build(1 , 1 , n);
	scanf("%lld" , &m);
	while(m --)
	{
		string op; int u , v; cin >> op , scanf("%lld%lld" , &u , &v);
		if(op == "C")
		{
			int x = ee[u].u , y = ee[u].v;
			if(dep[x] < dep[y]) swap(x , y);
			update1(1 , 1 , n , dfn[x] , v);
		}
		u ++ , v ++;
		if(op == "N") uv_update(u , v);
		if(op == "SUM") printf("%lld\n" , uv_query_sum(u , v));
		if(op == "MAX") printf("%lld\n" , uv_query_max(u , v));
		if(op == "MIN") printf("%lld\n" , uv_query_min(u , v)); 
	}
	return 0;
}