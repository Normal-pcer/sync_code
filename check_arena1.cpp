#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

#define ls (i<<1)
#define rs (i<<1|1)

struct Tree{
	int l,r,len,sum,maxn;
	Tree(){
		l=r=0;
		len=1;
		sum=0;
		maxn=-1e9;
	}
};

int n,q,cnt;
int a[30001],fa[30001],siz[30001],dep[30001],son[30001],dfn[30001],rnk[30001],tp[30001];
Tree tr[65537];
vector<int> e[30001];

void meg(Tree &rt,const Tree &l,const Tree &r){
	rt.sum=l.sum+r.sum;
	rt.maxn=max(l.maxn,r.maxn);
}

void pushup(int i){
	meg(tr[i],tr[ls],tr[rs]);
}

void build(int i,int l,int r){
	tr[i].l=l;
	tr[i].r=r;
	tr[i].len=r-l+1;
	if (l==r){
		tr[i].sum=tr[i].maxn=a[rnk[l]];
		return;
	}
	int mid=(l+r)>>1;
	build(ls,l,mid);
	build(rs,mid+1,r);
	pushup(i);
}

void update(int i,int x,int k){
	if (tr[i].l==tr[i].r){
		tr[i].sum=tr[i].maxn=k;
		return;
	}
	if (tr[ls].r>=x) update(ls,x,k);
	else update(rs,x,k);
	pushup(i);
}

Tree query(int i,int l,int r){
	if (tr[i].l>=l and tr[i].r<=r) return tr[i];
	Tree ans;
	if (tr[ls].r>=l) ans=query(ls,l,r);
	if (tr[rs].l<=r) meg(ans,ans,query(rs,l,r));
	return ans;
}

Tree query(int u,int v){
	Tree ans;
	while (tp[u]!=tp[v]){
		if (dep[tp[u]]<dep[tp[v]]) swap(u,v);
		meg(ans,ans,query(1,dfn[tp[u]],dfn[u]));
		u=fa[tp[u]];
	}
	if (dep[u]<dep[v]) swap(u,v);
	meg(ans,ans,query(1,dfn[v],dfn[u]));
	return ans;
}

void dfs1(int u,int f){
	fa[u]=f;
	dep[u]=dep[f]+1;
	siz[u]=1;
	for (auto i:e[u]){
		if (i!=f){
			dfs1(i,u);
			siz[u]+=siz[i];
			if (siz[i]>siz[son[u]]) son[u]=i;
		}
	}
}

void dfs2(int u,int t){
	dfn[u]=++cnt;
	rnk[cnt]=u;
	tp[u]=t;
	if (son[u]) dfs2(son[u],t);
	for (auto i:e[u]){
		if (i!=fa[u] and i!=son[u]){
			dfs2(i,i);
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin>>n;
	for (int i=1;i<n;i++){
		int u,v;
		cin>>u>>v;
		e[u].push_back(v);
		e[v].push_back(u);
	}
	for (int i=1;i<=n;i++){
		cin>>a[i];
	}
	dfs1(1,0);
	dfs2(1,1);
	build(1,1,n);
	cin>>q;
	while (q--){
		int u,v;
		string op;
		cin>>op>>u>>v;
		if (op=="CHANGE"){
			update(1,dfn[u],v);
		}else{
			Tree ans=query(u,v);
			if (op=="QMAX") cout<<ans.maxn<<"\n";
			else cout<<ans.sum<<"\n";
		}
	}
	return 0;
}