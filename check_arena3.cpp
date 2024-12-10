#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll inf=1e18;
const int N=1e5+5;
struct node{
	ll L,R,sum,ans;
	node(){L=R=ans=-inf,sum=0;}
	node(ll l,ll r,ll s,ll a){
		L=l,R=r,sum=s,ans=a;
	}
}a[N<<2];
#define ls ((p)<<1)
#define rs ((p)<<1|1)
#define mid ((x+y)>>1)
int val[N],n,m;
node merge(node x,node y){
	return (node){
		max(x.L,x.sum+y.L),
		max(y.R,y.sum+x.R),
		x.sum+y.sum,
		max({x.ans,y.ans,x.R+y.L})
	};
}
void build(int x,int y,int p){
	if(x==y){
		a[p]={val[x],val[x],val[x],val[x]};
		return;
	}
	build(x,mid,ls);
	build(mid+1,y,rs);
	a[p]=merge(a[ls],a[rs]);
}
node qry(int x,int y,int l,int r,int p){
	if(l>r)return {};
	if(l<=x&&r>=y)return a[p];
	if(r<=mid)return qry(x,mid,l,r,ls);
	if(l>mid)return qry(mid+1,y,l,r,rs);
	return merge(qry(x,mid,l,r,ls),qry(mid+1,y,l,r,rs));
}
void solve(){
	int n,q;
	scanf("%d",&n);
	for(int i=1;i<=n;++i)
		scanf("%d",&val[i]);
	build(1,n,1);
	scanf("%d",&q);
	while(q--){
		int l1,r1,l2,r2;
		scanf("%d%d%d%d",&l1,&r1,&l2,&r2);
		if(r1<l2)
			printf("%lld\n",qry(1,n,l1,r1,1).R+qry(1,n,r1+1,l2-1,1).sum+qry(1,n,l2,r2,1).L);
		else{
			ll ans=-1e18;
			ans=max(ans,qry(1,n,l1,l2,1).R+qry(1,n,l2,r2,1).L-qry(1,n,l2,l2,1).sum);
			ans=max(ans,qry(1,n,l1,r1,1).R+qry(1,n,r1,r2,1).L-qry(1,n,r1,r1,1).sum);
			ans=max(ans,qry(1,n,max(l1,l2),min(r1,r2),1).ans);
			printf("%lld\n",ans);
		}
	}
}
int main(){
	int T;cin>>T;
	while(T--)solve();
}