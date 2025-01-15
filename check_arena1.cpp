#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Tree{
	int ls,rs,sum,maxn;
};

int n,m,cnt,tot;
int rt[100001],a[100001],b[100001];
int dep[100001],siz[100001],fa[100001],son[100001],dfn[100001],rnk[100001],tp[100001];
vector<int> e[100001];
Tree tr[10000000];

void pushup(int i){
	tr[i].sum=tr[tr[i].ls].sum+tr[tr[i].rs].sum;
	tr[i].maxn=max(tr[tr[i].ls].maxn,tr[tr[i].rs].maxn);
}

void update(int &i,int l,int r,int x,int k){
	if (i==0) i=++tot;
	if (l==r){
		tr[i].sum=tr[i].maxn=k;
		return;
	}
	int mid=(l+r)>>1;
	if (x<=mid) update(tr[i].ls,l,mid,x,k);
	else update(tr[i].rs,mid+1,r,x,k);
	pushup(i);
}

int qsum(int i,int l,int r,int ql,int qr){
	if (i==0) return 0;
	if (l>=ql and r<=qr) return tr[i].sum;
	int mid=(l+r)>>1;
	int ans=0;
	if (mid>=ql) ans+=qsum(tr[i].ls,l,mid,ql,qr);
	if (mid<qr) ans+=qsum(tr[i].rs,mid+1,r,ql,qr);
	return ans;
}

int qmax(int i,int l,int r,int ql,int qr){
	if (i==0) return 0;
	if (l>=ql and r<=qr) return tr[i].maxn;
	int mid=(l+r)>>1;
	int ans=0;
	if (mid>=ql) ans=max(ans,qmax(tr[i].ls,l,mid,ql,qr));
	if (mid<qr) ans=max(ans,qmax(tr[i].rs,mid+1,r,ql,qr));
	return ans;
}

void dfs1(int u,int f){
	siz[u]=1;
	dep[u]=dep[f]+1;
	fa[u]=f;
	for (auto i:e[u]){
		if (i!=f){
			dfs1(i,u);
			siz[u]+=siz[i];
			if (siz[i]>siz[son[u]]){
				son[u]=i;
			}
		}
	}
}

void dfs2(int u,int t){
	tp[u]=t;
	dfn[u]=++cnt;
	rnk[dfn[u]]=u;
	if (son[u]) dfs2(son[u],t);
	for (auto i:e[u]){
		if (i!=fa[u] and i!=son[u]){
			dfs2(i,i);
		}
	}
}

int qsum(int u,int v){
	int ans=0,x=b[u];
	while (tp[u]!=tp[v]){
		if (dep[tp[u]]<dep[tp[v]]) swap(u,v);
		ans+=qsum(rt[x],1,n,dfn[tp[u]],dfn[u]);
		u=fa[tp[u]];
	}
	if (dep[u]<dep[v]) swap(u,v);
	ans+=qsum(rt[x],1,n,dfn[v],dfn[u]);
	return ans;
}

int qmax(int u,int v){
	int ans=0,x=b[u];
	while (tp[u]!=tp[v]){
		if (dep[tp[u]]<dep[tp[v]]) swap(u,v);
		ans=max(ans,qmax(rt[x],1,n,dfn[tp[u]],dfn[u]));
		u=fa[tp[u]];
	}
	if (dep[u]<dep[v]) swap(u,v);
	ans=max(ans,qmax(rt[x],1,n,dfn[v],dfn[u]));
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin>>n>>m;
	for (int i=1;i<=n;i++){
		cin>>a[i]>>b[i];
	}
	for (int i=1;i<n;i++){
		int u,v;
		cin>>u>>v;
		e[u].push_back(v);
		e[v].push_back(u);
	}
	dfs1(1,0);
	dfs2(1,1);
	for (int i=1;i<=n;i++){
		update(rt[b[i]],1,n,dfn[i],a[i]);
	}
	for (int i=1;i<=m;i++){
		string op;
		int u,v;
		cin>>op>>u>>v;
		if (op=="CC"){
			update(rt[b[u]],1,n,dfn[u],0);
			b[u]=v;
			update(rt[b[u]],1,n,dfn[u],a[u]);
		}else if (op=="CW"){
			a[u]=v;
			update(rt[b[u]],1,n,dfn[u],a[u]);
		}else if (op=="QS"){
			cout<<qsum(u,v)<<"\n";
		}else{
			cout<<qmax(u,v)<<"\n";
		}
	}
	return 0;
}