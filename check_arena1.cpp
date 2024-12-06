#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Node{
	int ls,rs,idx,maxn,cnt;
};

int n,m,cnt;
int a[100001],maxn[100001],minn[100001],f[100001];
int rt[100001];
Node tr[3200000];

void update(int &i,int l,int r,int x,int k){
	if (i==0){
		i=++cnt;
		tr[i].idx=x;
		tr[i].cnt=tr[i].maxn=k;
		return;
	}
	tr[i].maxn=max(tr[i].maxn,k);
	if (tr[i].idx==x){
		tr[i].cnt=max(tr[i].cnt,k);
		return;
	}
	if (l==r) return;
	int mid=(l+r)>>1;
	if (mid>=x) update(tr[i].ls,l,mid,x,k);
	else update(tr[i].rs,mid+1,r,x,k);
}

int query(int i,int l,int r,int ql,int qr){
	if (i==0) return 0;
	if (l>=ql and r<=qr) return tr[i].maxn;
	int mid=(l+r)>>1,ans=0;
	if (ql<=tr[i].idx and tr[i].idx<=qr) ans=tr[i].cnt;
	if (mid>=ql) ans=max(ans,query(tr[i].ls,l,mid,ql,qr));
	if (mid+1<=qr) ans=max(ans,query(tr[i].rs,mid+1,r,ql,qr));
	return ans;
}

inline int lowbit(int x){
	return x&(-x);
}

void add(int x,int y,int c){
	for (;x<=n;x+=lowbit(x)){
		update(rt[x],1,n,y,c);
	}
}

int query(int x,int y){
	int ans=0;
	for (;x;x-=lowbit(x)){
		ans=max(ans,query(rt[x],1,n,1,y));
	}
	return ans;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>n>>m;
	for (int i=1;i<=n;i++){
		cin>>a[i];
		maxn[i]=minn[i]=a[i];
	}
	for (int i=1;i<=m;i++){
		int x,y;
		cin>>x>>y;
		maxn[x]=max(maxn[x],y);
		minn[x]=min(minn[x],y);
	}
	for (int i=1;i<=n;i++){
		f[i]=query(minn[i],a[i])+1;
		add(a[i],maxn[i],f[i]);
	}
	cout<<*max_element(f+1,f+n+1);
	return 0;
}