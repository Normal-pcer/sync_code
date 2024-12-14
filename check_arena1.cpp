#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

#define ls (i<<1)
#define rs (i<<1|1)

struct Tree{
	int l,r;
	ll maxn,tag;
};

int n,m;
ll s[1000001],f[1000001];
Tree tr[2097153];
vector<pair<int,int> > q[1000001];

void pushup(int i){
	tr[i].maxn=max(tr[ls].maxn,tr[rs].maxn);
}

void pushdown(int i){
	if (tr[i].tag){
		tr[ls].maxn+=tr[i].tag;
		tr[rs].maxn+=tr[i].tag;
		tr[ls].tag+=tr[i].tag;
		tr[rs].tag+=tr[i].tag;
		tr[i].tag=0;
	}
}

void build(int i,int l,int r){
	tr[i].l=l;
	tr[i].r=r;
	if (l==r){
		tr[i].maxn=s[l-1];
		return;
	}
	int mid=(l+r)>>1;
	build(ls,l,mid);
	build(rs,mid+1,r);
	pushup(i);
}

void update(int i,int l,int r,ll k){
	if (tr[i].l>=l and tr[i].r<=r){
		tr[i].maxn+=k;
		tr[i].tag+=k;
		return;
	}
	pushdown(i);
	if (tr[ls].r>=l) update(ls,l,r,k);
	if (tr[rs].l<=r) update(rs,l,r,k);
	pushup(i);
}

ll query(int i,int l,int r){
	if (tr[i].l>=l and tr[i].r<=r) return tr[i].maxn;
	pushdown(i);
	ll ans=-1e18;
	if (tr[ls].r>=l) ans=query(ls,l,r);
	if (tr[rs].l<=r) ans=max(ans,query(rs,l,r));
	return ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>n>>m;
	for (int i=1;i<=n;i++){
		cin>>s[i];
		s[i]+=s[i-1];
	}
	for (int i=1;i<=m;i++){
		int l,r,w;
		cin>>l>>r>>w;
		q[r].push_back({l,w});
	}
	build(1,1,n);
	for (int i=1;i<=n;i++){
		for (auto [l,w]:q[i]){
			update(1,1,l,w);
		}
		f[i]=max(f[i-1],query(1,1,i)-s[i]);
		if (i<n) update(1,i+1,i+1,f[i]);
	}
	cout<<f[n];
	return 0;
}