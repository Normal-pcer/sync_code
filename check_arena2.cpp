#include<bits/stdc++.h>
#define lson(root) (root << 1)
#define rson(root) (root << 1 | 1)
using namespace std;
struct edge
{
	int to , nxt;
}e[200010];
struct node
{
	int mn , lazy;
}tree[400010];
int n , m , tot = 0 , head[100010] , a[100010] , id , dep[100010] , sz[100010] , son[100010] , fa[100010] , dfn[100010] , cnt = 0 , rk[100010] , tp[100010];
void add(int u , int v)
{
	++ tot;
	e[tot].to = v;
	e[tot].nxt = head[u];
	head[u] = tot;
}
void dfs1(int u , int pa)
{
	dep[u] = dep[pa] + 1 , sz[u] = 1 , fa[u] = pa;
	for(int i = head[u] ; i != 0 ; i = e[i].nxt)
	{
		int v = e[i].to;
		if(v == pa) continue;
		dfs1(v , u);
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
	tree[root].mn = min(tree[lson(root)].mn , tree[rson(root)].mn);
}
void pushdown(int root , int l , int r)
{
	if(tree[root].lazy == 0) return;
	tree[lson(root)].mn = tree[root].lazy;
	tree[lson(root)].lazy = tree[root].lazy;
	tree[rson(root)].mn = tree[root].lazy;
	tree[rson(root)].lazy = tree[root].lazy;
	tree[root].lazy = 0;
}
void update(int root , int l , int r , int L , int R , int val)
{
	if(L <= l && R >= r)
	{
		tree[root].mn = tree[root].lazy = val;
		return;
	}
	int mid = (l + r) >> 1;
	pushdown(root , l , r);
	if(L <= mid) update(lson(root) , l , mid , L , R , val);
	if(R > mid) update(rson(root) , mid + 1 , r , L , R , val);
	pushup(root);
}
int query(int root , int l , int r , int L , int R)
{
	if(L <= l && R >= r) return tree[root].mn;
	int mid = (l + r) >> 1 , ans = INT_MAX;
	pushdown(root , l , r);
	if(L <= mid) ans = min(ans , query(lson(root) , l , mid , L , R));
	if(R > mid) ans = min(ans , query(rson(root) , mid + 1 , r , L , R));
	return ans;
}
void build(int root , int l , int r)
{
	if(l == r)
	{
		tree[root].mn = a[rk[l]];
		return;
	}
	int mid = (l + r) >> 1;
	build(lson(root) , l , mid);
	build(rson(root) , mid + 1 , r);
	pushup(root);
}
void uv_update(int u , int v , int val)
{
	while(tp[u] != tp[v])
	{
		if(dep[tp[u]] < dep[tp[v]]) swap(u , v);
		update(1 , 1 , n , dfn[tp[u]] , dfn[u] , val);
		u = fa[tp[u]];
	}
	if(dep[u] < dep[v]) swap(u , v);
	update(1 , 1 , n , dfn[v] , dfn[u] , val);
}
int dfs(int u , int v)
{
	while(tp[u] != tp[v])
	{
		if(fa[tp[u]] == v) return tp[u];
		u = fa[tp[u]];
	}
	return son[v];
}
int uv_query(int u , int v)
{
	if(rk[u] == id) return tree[1].mn;
	int ans = INT_MAX;
	if(dfn[id] < u || dfn[id] > v)
	{
//		while(tp[u] != tp[v])
//		{
//			if(dep[tp[u]] < dep[tp[v]]) swap(u , v);
//			ans = min(ans , query(1 , 1 , n , dfn[tp[u]] , dfn[u])); 
//			u = fa[tp[u]];
//		}
//		if(dep[u] < dep[v]) swap(u , v);
//		ans = min(ans , query(1 , 1 , n , dfn[v] , dfn[u]));
		return query(1 , 1 , n , u , v);
	}
	int k = dfs(id , rk[u]);
	ans = min(ans , query(1 , 1 , n , 1 , dfn[k] - 1));
	if(dfn[k] + sz[k] <= n) ans = min(ans , query(1 , 1 , n , dfn[k] + sz[k] , n));
	return ans;
}
int main()
{
	scanf("%d%d" , &n , &m);
	for(int i = 1 ; i < n ; i ++)
	{
		int u , v;
		scanf("%d%d" , &u , &v);
		add(u , v) , add(v , u); 
	}
	for(int i = 1 ; i <= n ; i ++) scanf("%d" , &a[i]);
	scanf("%d" , &id);
	dfs1(id , 0) , dfs2(id , id);
	build(1 , 1 , n);
	while(m --)
	{
		int op , x , y , z;
		scanf("%d%d" , &op , &x);
		if(op == 1) id = x;
		if(op == 2)
		{
			scanf("%d%d" , &y , &z);
			uv_update(x , y , z);
		}
		if(op == 3) printf("%d\n" , uv_query(dfn[x] , min(dfn[x] + sz[x] - 1 , n)));
	}
	return 0;
}