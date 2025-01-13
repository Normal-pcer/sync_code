#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

#define ls (i<<1)
#define rs (i<<1|1)

int n,q,cnt;
int a[10],dis[10],dep[10],siz[10],son[10],fa[10],dfn[10],rnk[10],tp[10];
vector<pair<int,int> > e[10];

struct Seg{
	struct Tree{
		int l,r,len,sum,tag;
		Tree(){
			l=r=len=sum=tag=0;
		}
	};
	Tree tr[65536];
	void pushup(int i){
		tr[i].sum=tr[ls].sum+tr[rs].sum;
	}
	void pushdown(int i){
		if (tr[i].tag){
			tr[ls].sum=tr[ls].len-tr[ls].sum;
			tr[rs].sum=tr[rs].len-tr[rs].sum;
			tr[ls].tag^=1;
			tr[rs].tag^=1;
			tr[i].tag=0;
		}
	}
	void build(int i,int l,int r,int j){
		tr[i].l=l;
		tr[i].r=r;
		tr[i].len=r-l+1;
		if (l==r){
			tr[i].sum=((dis[rnk[l]]>>j)&1);
			return;
		}
		int mid=(l+r)>>1;
		build(ls,l,mid,j);
		build(rs,mid+1,r,j);
		pushup(i);
	}
	void update(int i,int l,int r){
		if (tr[i].l>=l and tr[i].r<=r){
			tr[i].sum=tr[i].len-tr[i].sum;
			tr[i].tag^=1;
			return;
		}
		pushdown(i);
		if (tr[ls].r>=l) update(ls,l,r);
		if (tr[rs].l<=r) update(rs,l,r);
		pushup(i);
	}
	int query(int i,int l,int r){
		if (tr[i].l>=l and tr[i].r<=r) return tr[i].sum;
		pushdown(i);
		int ans=0;
		if (tr[ls].r>=l) ans+=query(ls,l,r);
		if (tr[rs].l<=r) ans+=query(rs,l,r);
		return ans;
	}
}tr[10];

void dfs1(int u,int f){
	dep[u]=dep[f]+1;
	siz[u]=1;
	fa[u]=f;
	for (auto [i,w]:e[u]){
		if (i!=f){
			dfs1(i,u);
			a[i]=w;
			siz[u]+=siz[i];
            std::cout << std::format("{}dis[{}] = {}dis[{}] ^ {}", dis[i], i, dis[u], u, w) << std::endl;
			// dis[i]=dis[u]^w;
			if (siz[i]>siz[son[u]]) son[u]=i;
		}
	}
}

void dfs2(int u,int t){
	dfn[u]=++cnt;
	rnk[cnt]=u;
	tp[u]=t;
	if (son[u]) dfs2(son[u],t);
	for (auto [i,w]:e[u]){
		if (i!=son[u] and i!=fa[u]){
			dfs2(i,i);
		}
	}
}

ll query(int u,int v){
	ll len=dep[u]+dep[v],s[10]={0,0,0,0,0,0,0,0,0,0};
	while (tp[u]!=tp[v]){
		if (dep[tp[u]]<dep[tp[v]]) swap(u,v);
        std::cout << std::format("{} query({}, {})", __LINE__, dfn[tp[u]], dfn[u]) << std::endl;
		for (int i=0;i<10;i++) s[i]+=tr[i].query(1,dfn[tp[u]],dfn[u]);
		u=fa[tp[u]];
	}
	if (dep[u]<dep[v]) swap(u,v);
    std::cout << std::format("{} query({}, {})", __LINE__, dfn[v], dfn[u]) << std::endl;
	for (int i=0;i<10;i++) s[i]+=tr[i].query(1,dfn[v],dfn[u]);
	len+=-dep[v]*2+1;
	cout<<len<<"*\n";
	ll ans=0;
	for (int i=0;i<10;i++){
		cout<<s[i]<<" ";
		ans+=s[i]*(len-s[i])<<i;
	}
	cout<<"\n";
	return ans;
}

int main(){
	// ios::sync_with_stdio(false);
	// cin.tie(nullptr);
	// cout.tie(nullptr);
	cin>>n>>q;
	for (int i=1;i<n;i++){
		int u,v,w;
		cin>>u>>v>>w;
		e[u].push_back({v,w});
		e[v].push_back({u,w});
	}
	dfs1(1,0);
    std::cout << "dick";
    for (auto i = 1; i <= n; i++)  std::cout << dis[i] << ' ';
    std::cout << std::endl;
	dfs2(1,1);
	for (int i=0;i<10;i++) tr[i].build(1,1,n,i);
	while (q--){
		int op,u,v,k;
		cin>>op>>u>>v;
		if (op==1){
			cout<<query(u,v)<<"\n";
		}else{
			cin>>k;
			if (fa[v]==u) swap(u,v);
			for (int i=0;i<10;i++) if (((k^a[u])>>i)&1) tr[i].update(1,dfn[u],dfn[u]+siz[u]-1);
			a[u]=k;
		}
	}
	return 0;
}
