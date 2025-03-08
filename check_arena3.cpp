#include <iostream>
#include <algorithm>
#define ll long long
#define lc (x<<1)
#define rc ((x<<1)|1)
#define mid ((l+r)>>1)
using namespace std;
const ll N=2e5+10;
const ll V=1e9+10;
struct node{
	ll a,b,t,id;
}bl[N];
inline bool cmp(node x,node y){return x.t<y.t;}
ll n,sum[N<<2],tag[N<<2],minn[N<<2],maxx[N<<2];
void push_up(ll x){
	sum[x]=sum[lc]+sum[rc];
	minn[x]=min(minn[lc],minn[rc]);
	maxx[x]=max(maxx[lc],maxx[rc]);
}
void push_down(ll x,ll l,ll r){
	if(tag[x]==-114514) return;
	tag[lc]=tag[rc]=tag[x];
	minn[lc]=minn[rc]=tag[x];
	maxx[lc]=maxx[rc]=tag[x];
	sum[lc]=tag[x]*(mid-l+1);
	sum[rc]=tag[x]*(r-mid);
	tag[x]=-114514;
}
void build(ll x,ll l,ll r){
	tag[x]=-114514;
	if(l==r){
		if(!l) sum[x]=minn[x]=maxx[x]=-114;
		else if(l>n) sum[x]=minn[x]=maxx[x]=V;
		else sum[x]=minn[x]=maxx[x]=bl[l].a-l;
		return;
	}
	build(lc,l,mid);build(rc,mid+1,r);
	push_up(x);
}
void chg(ll x,ll l,ll r,ll L,ll R,ll v){
	if(L<=l&&r<=R){
		sum[x]=v*(r-l+1);
		tag[x]=maxx[x]=minn[x]=v;
		return;
	}
	push_down(x,l,r);
	if(L<=mid) chg(lc,l,mid,L,R,v);
	if(R>mid) chg(rc,mid+1,r,L,R,v);
	push_up(x);
}
ll getMD(ll x,ll l,ll r,ll rank){
	if(l==r) return l;
	push_down(x,l,r);
	if(rank<=minn[rc]) return getMD(lc,l,mid,rank);
	return getMD(rc,mid+1,r,rank);
}
ll getOT(ll x,ll l,ll r,ll rank){
	if(l==r) return l;
	push_down(x,l,r);
	if(rank<maxx[lc]) return getOT(lc,l,mid,rank);
	return getOT(rc,mid+1,r,rank);
}
ll query(ll x,ll l,ll r,ll L,ll R){
	if(L<=l&&r<=R) return sum[x];
	push_down(x,l,r);
	ll ret=0;
	if(L<=mid) ret+=query(lc,l,mid,L,R);
	if(R>mid) ret+=query(rc,mid+1,r,L,R);
	return ret;
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(0),cout.tie(0);
	ll c,T;cin>>c>>T;
	while(T--){
		cin>>n;
		for(ll i=1;i<=n;i++){cin>>bl[i].a>>bl[i].b>>bl[i].t;bl[i].id=i;}
		build(1,0,n+1);
		sort(bl+1,bl+1+n,cmp);
		ll now=0;
		for(ll i=1;i<=n;i++){
			ll pos=query(1,0,n+1,bl[i].id,bl[i].id)+bl[i].id;
			if(pos==bl[i].b){
				if(now>bl[i].t) goto endthesol;
				continue;
			}
			ll L,R,calc;
			if(pos>bl[i].b){
				L=getMD(1,0,n+1,bl[i].b-bl[i].id+1);
				R=bl[i].id-L-1;
				calc=query(1,0,n+1,L+1,bl[i].id)+(L+1+bl[i].id)*(bl[i].id-L)/2;
				now+=calc-(2*bl[i].b-R)*(R+1)/2;
				if(now>bl[i].t) goto endthesol;
				chg(1,0,n+1,L+1,bl[i].id,bl[i].b-bl[i].id);
			}
			else{
				L=getOT(1,0,n+1,bl[i].b-bl[i].id-1);
				R=L-bl[i].id-1;
				calc=query(1,0,n+1,bl[i].id,L-1)+(bl[i].id+L-1)*(L-bl[i].id)/2;
				now+=(2*bl[i].b+R)*(R+1)/2-calc;
				if(now>bl[i].t) goto endthesol;
				chg(1,0,n+1,bl[i].id,L-1,bl[i].b-bl[i].id);
			}
		}
		cout<<"Yes\n";
		continue;
		endthesol:;
		cout<<"No\n";
	}
	return 0;
} 