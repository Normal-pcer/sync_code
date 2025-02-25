#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 i128;

constexpr int mod=1004535809;

struct dbi{
	int val;
	long double _v;
	dbi(){
		val=_v=0;
	}
	dbi(ll x){
		val=_v=x%mod;
	}
	dbi(ll x,long double _x){
		val=x%mod;
		_v=_x;
	}
	auto operator <=> (const dbi& b) const{
		return _v<=>b._v;
	}
	dbi operator + (const dbi &b) const{
		return dbi(val+b.val,_v+b._v);
	}
	dbi operator - (const dbi &b) const{
		return dbi(val-b.val+mod,_v-b._v);
	}
	dbi operator * (const dbi &b) const{
		return dbi(ll(val)*b.val,_v*b._v);
	}
};

int n;
ll a[1000001];
dbi b[1000001],e[1000001],sum,tmp,ans;

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	cin>>n;
	for (int i=1;i<=n;i++){
		cin>>a[i];
		b[i]=b[i-1]+dbi(a[i]);
		e[i-1]=dbi(ll(i-1)*(n-i+1));
		sum=sum+e[i-1]*dbi(a[i]);
		if (i>1) tmp=tmp+(e[i-2]-e[i-1])*dbi(a[i]);
	}
	tmp=tmp+dbi(a[1]*(n-1));
	ans=sum;
	for (int i=2;i<=n;i++){
		sum=sum+tmp;
		tmp=tmp-b[n]*dbi(2);
		tmp=tmp+dbi(2*n*a[i]);
		ans=min(ans,sum);
	}
	cout<<ans.val;
	return 0;
}